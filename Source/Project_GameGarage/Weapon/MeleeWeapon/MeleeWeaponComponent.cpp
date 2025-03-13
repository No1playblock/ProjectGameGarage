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
		float LocalStartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		if (!(Character->HasAuthority()))		//Ŭ���
		{
			ComboActionBegin();
			Server_ComboActionBegin(LocalStartTime);  // ������ ��� RPC ȣ��
		}
		else		//������
		{
			Multicast_ComboActionBegin();
		}
		return;
	}
	if (!ComboTimerHandle.IsValid())		//Ÿ�̸Ӱ� ���� ���¸� false	//�������� ���� �߰�
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

void UMeleeWeaponComponent::ComboActionBegin()
{
	CurrentCombo = 1;
	//Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* Animinstance = Character->GetMesh()->GetAnimInstance();
	Animinstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UMeleeWeaponComponent::ComboActionEnded);
	Animinstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	SetComboCheckTimer();
}


void UMeleeWeaponComponent::Server_ComboActionBegin_Implementation(float ClientRequestTime)
{
	float ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	float LagTime = FMath::Abs(ServerTime - ClientRequestTime);

	ComboActionBegin();
	Client_CorrectComboTimer(LagTime);

}
bool UMeleeWeaponComponent::Server_ComboActionBegin_Validate(float ComboActionStartTime)
{
	//if (LastComboActionStartTime == 0.0f)
	//{
	//	return true;
	//}

	//const float TimeSinceLastCombo = ComboActionStartTime - LastComboActionStartTime;
	//if (CurrentCombo > 0)
	//{
	//	AttackTime = (EffectiveFrameCount[CurrentCombo - 1] / FrameRate) / 1.0f;
	//}
	//else
	//{
	//	AttackTime = (EffectiveFrameCount[0] / FrameRate) / 1.0f;
	//}
	//return TimeSinceLastCombo >= AttackTime;
	return true;
}



void UMeleeWeaponComponent::Multicast_ComboActionBegin_Implementation()
{
	ComboActionBegin();
}

void UMeleeWeaponComponent::Client_CorrectComboTimer_Implementation(float LagTime)
{
	if (!Character->IsLocallyControlled())
		return;  // Ŭ���̾�Ʈ�� ó��

	if (!ComboTimerHandle.IsValid())
	{
		GG_LOG(LogGGNetwork, Log, TEXT("ComboTimerNot Valid"));
		return;
	}

	int32 ComboIndex;

	if (CurrentCombo == 0)
	{
		ComboIndex = 0;
	}
	else
		ComboIndex = CurrentCombo - 1;

	float OriginalComboEffectiveTime = (EffectiveFrameCount[ComboIndex] / FrameRate);

	// �̹� �帥 �ð�(LagTime)�� ����� �� Ÿ�̸� ��
	float AdjustedComboEffectiveTime = FMath::Max(0.f, OriginalComboEffectiveTime + LagTime);

	// ���� Ÿ�̸� �ʱ�ȭ �� ������ Ÿ�̸� �缳��
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMeleeWeaponComponent::ComboCheck, AdjustedComboEffectiveTime, false);

}

void UMeleeWeaponComponent::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;

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

	if (ComboEffectiveTime > 0.0f)
	{
		if (!Character->IsLocallyControlled())		//���Ͻ�
		{
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMeleeWeaponComponent::ComboCheck, ComboEffectiveTime, false);
		}
		else		//������ �ʰ�
		{
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMeleeWeaponComponent::ComboCheck, ComboEffectiveTime + ServerDifferenceTime, false);
		}
	}
}


void UMeleeWeaponComponent::ComboCheck()
{

	if (bHasNextComboCommand)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		FName CurrentSection = FName(*FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo));
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxComboCount);
		FName NextSection = FName(*FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo));

		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

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

