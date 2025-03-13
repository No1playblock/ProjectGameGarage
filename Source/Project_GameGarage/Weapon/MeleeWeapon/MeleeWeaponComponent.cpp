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
		float LocalStartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		if (!(Character->HasAuthority()))		//클라면
		{
			ComboActionBegin();
			Server_ComboActionBegin(LocalStartTime);  // 서버로 즉시 RPC 호출
		}
		else		//서버면
		{
			Multicast_ComboActionBegin();
		}
		return;
	}
	if (!ComboTimerHandle.IsValid())		//타이머가 죽은 상태면 false	//안정성을 위해 추가
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
		return;  // 클라이언트만 처리

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

	// 이미 흐른 시간(LagTime)을 고려한 새 타이머 값
	float AdjustedComboEffectiveTime = FMath::Max(0.f, OriginalComboEffectiveTime + LagTime);

	// 기존 타이머 초기화 후 보정된 타이머 재설정
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

	//ComboEffectiveTime동안 TImer 실행이므로 각 Combo별 시간이 들어감
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (EffectiveFrameCount[ComboIndex] / FrameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		if (!Character->IsLocallyControlled())		//프록시
		{
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMeleeWeaponComponent::ComboCheck, ComboEffectiveTime, false);
		}
		else		//서버면 늦게
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

