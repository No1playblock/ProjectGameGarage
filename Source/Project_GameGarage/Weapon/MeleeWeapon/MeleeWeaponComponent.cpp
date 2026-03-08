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

	SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
}

bool UMeleeWeaponComponent::AttachWeapon(APlayableCharacter* TargetCharacter)		//플레이어가 무기를 장착하는 메소드
{
	if (!TargetCharacter) return false;

	Character = TargetCharacter;
	Character->SetHasWeapon(true);				//Weapon상태를 변경해 줌.

	TArray<AActor*> ChildActors;
	Character->GetAttachedActors(ChildActors);	//부착되어있는 자식 액터에서

	for (AActor* ChildActor : ChildActors)
	{
		// 각 ChildActor에 대한 작업 수행
		ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		//떼어 냄
		ChildActor->Destroy();			//파괴
		// 카메라 위치 원상복구
		TargetCharacter->MoveCameraToDefaultPosition();
	}

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);					//플레이어에 WeaponComponent를 추가해준다.

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	SetRelativeTransform(RelativeTransform);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))		//플레이어 컨트롤러가 있을 때 Fire가 작동되도록
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
	if (!Character) return;

	// 캐릭터가 스턴 상태일 경우 공격을 시도하지 않음
	if (Character->GetbIsStunned()) return;

	if (CurrentCombo == 0)
	{
		bIsCombo = true;
		Server_ComboActionBegin();

		return;
	}
	if (!bIsCombo)		//콤보가 진행중이지 않은 상태라면 bHasNextComboCommand를 false로
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
	// 상태를 모든 클라이언트에 전달
}


void UMeleeWeaponComponent::Server_ComboActionBegin_Implementation()
{
	CurrentCombo = 1;
	OnRep_CurrentCombo();
	SetComboCheckTimer();
}


void UMeleeWeaponComponent::OnRep_CurrentCombo()
{
	if (!Character || !Character->GetMesh() || CurrentCombo == 0) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	if (CurrentCombo == 1)
	{
		// 첫 번째 콤보 시작
		const float AttackSpeedRate = 1.0f;
		AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UMeleeWeaponComponent::ComboActionEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
	}
	else
	{
		// 다음 콤보 섹션으로 점프
		FName NextSection = FName(*FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo));
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
	}
}

void UMeleeWeaponComponent::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{

	CurrentCombo = 0;
	bIsCombo = false;

}

void UMeleeWeaponComponent::StartSimpleAutoSwing(float Interval)
{

	AActor* MyOwner = GetOwner();
	if (!MyOwner || !MyOwner->HasAuthority()) return;

	GetWorld()->GetTimerManager().SetTimer(
		AutoSwingTimerHandle,
		this,
		&UMeleeWeaponComponent::ExecuteSimpleAutoSwing,
		Interval,
		true
	);
}

void UMeleeWeaponComponent::ExecuteSimpleAutoSwing()
{
	Server_SimpleAutoSwing();
}

void UMeleeWeaponComponent::Server_SimpleAutoSwing_Implementation()
{
	if (AMeleeWeaponActor* WeaponActor = Cast<AMeleeWeaponActor>(GetOwner()))
	{
		WeaponActor->SetIsHit(false);
	}

	Multicast_SimpleAutoSwing();
}

void UMeleeWeaponComponent::Multicast_SimpleAutoSwing_Implementation()
{
	//GG_LOG(LogGGNetwork, Warning, TEXT("Simple Auto Swing Executed at Time: %f"), GetWorld()->GetGameState()->GetServerWorldTimeSeconds());

	// Character와 Mesh가 유효한지 확인 (네트워크 복제 확인)
	if (!Character || !Character->GetMesh()) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	if (ComboActionMontage)
	{
		AnimInstance->Montage_Play(ComboActionMontage, 1.0f);
		// 섹션 점프 없이 몽타주의 처음부터 재생
	}
	// 콤보 시스템 상태 초기화 (방해 금지)
	CurrentCombo = 0;
	bIsCombo = false;
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

	//ComboEffectiveTime에서 Timer 설정이므로 각 Combo의 시간에 맞
	if (ComboIndex >= EffectiveFrameCount.Num()) return;

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
	if (!Character) return;

	if (bHasNextComboCommand)
	{
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, (int32)MaxComboCount);
		OnRep_CurrentCombo();

		SetComboCheckTimer();

		if (!Character->HasAuthority())
			Server_RequestNextCombo(false);
		else
			bHasNextComboCommand = false;
	}
}


void UMeleeWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
	DOREPLIFETIME(UMeleeWeaponComponent, Character);
}

