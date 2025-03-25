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

bool UMeleeWeaponComponent::AttachWeapon(APlayableCharacter* TargetCharacter)		//플레이어에게 무기를 장착하는 메소드
{
	Character = TargetCharacter;
	Character->SetHasWeapon(true);				//Weapon으로 덮어씌우기 위함.

	UE_LOG(LogTemp, Warning, TEXT("Attach"));
	TArray<AActor*> ChildActors;
	Character->GetAttachedActors(ChildActors);	//붙어있는 자식 가져와서 

	for (AActor* ChildActor : ChildActors)
	{
		// 각 ChildActor에 대해 작업 수행
		ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		//다 떼고
		ChildActor->Destroy();			//삭제
		// 카메라 위치 원상복구
		TargetCharacter->MoveCameraToDefaultPosition();
	}

	if (Character == nullptr)
	{
		return false;
	}

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);					//플레이어에게 WeaponComponent를 추가해준다.

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	SetRelativeTransform(RelativeTransform);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))		//좌클릭을 눌렀을 때 Fire가 작동되도록
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
	if (!bisCombo)		//콤보가 진행중인 상태면 bhas를 true로
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
	// 상태를 모든 클라이언트에 전파
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

	//ComboEffectiveTime동안 TImer 실행이므로 각 Combo별 시간이 들어감
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

