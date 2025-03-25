// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponComponent.h"
#include "Character/PlayableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameState.h"
#include "Project_GameGarage.h"
#include "Math/UnrealMathUtility.h"
#include "EngineUtils.h"
#include "MeleeWeaponActor.h"

// Sets default values for this component's properties
UMeleeWeaponComponent::UMeleeWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	SetIsReplicated(true);  // 
}

bool UMeleeWeaponComponent::AttachWeapon(APlayableCharacter* TargetCharacter)		//�÷��̾�� ���⸦ �����ϴ� �޼ҵ�
{
	Character = TargetCharacter;
	Character->SetHasWeapon(true);				//Weapon���� ������ ����.

	UE_LOG(LogTemp, Warning, TEXT("Attach"));
	TArray<AActor*> ChildActors;
	Character->GetAttachedActors(ChildActors);	//�پ��ִ� �ڽ� �����ͼ� 

	for (AActor* ChildActor : ChildActors)
	{
		// �� ChildActor�� ���� �۾� ����
		ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		//�� ����
		ChildActor->Destroy();			//����
		// ī�޶� ��ġ ���󺹱�
		TargetCharacter->MoveCameraToDefaultPosition();
	}

	if (Character == nullptr)
	{
		return false;
	}

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);					//�÷��̾�� WeaponComponent�� �߰����ش�.

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	SetRelativeTransform(RelativeTransform);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))		//��Ŭ���� ������ �� Fire�� �۵��ǵ���
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{

			Subsystem->AddMappingContext(SwingMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(SwingAction, ETriggerEvent::Triggered, this, &UMeleeWeaponComponent::PressComboCommand);
		}
	}
	return true;
}


void UMeleeWeaponComponent::PressComboCommand()
{
	if (CurrentCombo == 0)
	{
		bisCombo = true;
		Server_ComboActionBegin();
		
		return;
	}
	if (!bisCombo)		//�޺��� �������� ���¸� bhas�� true��
	{

		if (Character->HasAuthority())
		{
			bHasNextComboCommand = false;
		}
		else
			Server_RequestNextCombo(false);

	}
	else
	{
		if (Character->HasAuthority())
		{
			bHasNextComboCommand = true;
		}
		else
			Server_RequestNextCombo(true);
	}

}

void UMeleeWeaponComponent::Server_RequestNextCombo_Implementation(bool bNextCombo)
{
	bHasNextComboCommand = bNextCombo;
	// ���¸� ��� Ŭ���̾�Ʈ�� ����
}


void UMeleeWeaponComponent::Server_ComboActionBegin_Implementation()
{
	Multicast_PlayAnimation();

	SetComboCheckTimer();
}


void UMeleeWeaponComponent::Multicast_PlayAnimation_Implementation()
{
	CurrentCombo = 1;
	//Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* Animinstance = Character->GetMesh()->GetAnimInstance();
	Animinstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UMeleeWeaponComponent::ComboActionEnded);
	Animinstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
}

void UMeleeWeaponComponent::Multicast_JumpToAnimation_Implementation()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	FName CurrentSection = FName(*FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo));
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxComboCount);
	FName NextSection = FName(*FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo));

	AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
}

void UMeleeWeaponComponent::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	
	CurrentCombo = 0;
	bisCombo = false;

}
void UMeleeWeaponComponent::SetComboCheckTimer()
{
	int32 ComboIndex;
	if (CurrentCombo == 0)
	{
		return;
	}
	else
		ComboIndex = CurrentCombo - 1;

	//ComboEffectiveTime���� TImer �����̹Ƿ� �� Combo�� �ð��� ��
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (EffectiveFrameCount[ComboIndex] / FrameRate) / AttackSpeedRate;

	//GG_LOG(LogGGNetwork, Warning, TEXT("Time: %f"), GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMeleeWeaponComponent::ComboCheck, ComboEffectiveTime, false);
	}
}


void UMeleeWeaponComponent::ComboCheck()
{

	GG_LOG(LogGGNetwork, Warning, TEXT("COmboCheck Time: %f"), GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	if (bHasNextComboCommand)
	{
		Multicast_JumpToAnimation();

		GG_LOG(LogGGNetwork, Warning, TEXT("Play ANimation"));
		SetComboCheckTimer();

		if (!Character->HasAuthority())
			Server_RequestNextCombo(false);
		else
			bHasNextComboCommand = false;
	}
}


void UMeleeWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)			//
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(SwingMappingContext);
		}
	}
}

void UMeleeWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMeleeWeaponComponent, bHasNextComboCommand);
	DOREPLIFETIME(UMeleeWeaponComponent, CurrentCombo);
}

