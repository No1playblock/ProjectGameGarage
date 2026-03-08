// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/RangedWeapon/RangedWeaponActor.h"
#include "UI/CrosshairWidget.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Player/GGPlayerController.h"
#include "Components/ProgressBar.h"
#include "Game/SaveFile.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"                  
#include "CharacterStat/CharacterStatComponent.h"
#include "EngineUtils.h"
#include "Project_GameGarage.h"
#include "GameFramework/PlayerController.h"
#include "Game/GGGameInstance.h"
#include "Game/MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayableCharacter::APlayableCharacter() :
	Score(0)
{
	bReplicates = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MuscleCat/Meshes/SM_Muscle_Bear.SM_Muscle_Bear'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> CharacterMatRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/MuscleCat/Materials/Emo/MI_Emo_b01.MI_Emo_b01'"));
	if (CharacterMatRef.Object)
	{
		GetMesh()->SetMaterial(1, CharacterMatRef.Object);
	}
	

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	LeftHandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), "hand_l"); // 왼손 소켓에 부착
	LeftHandCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Trigger로 설정
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnHandCollision);

	RightHandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(GetMesh(), "hand_r"); // 오른손 소켓에 부착
	RightHandCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Trigger로 설정
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnHandCollision);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 


	
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // 
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	CameraBoom->bUsePawnControlRotation = true; // 


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));
	FollowCamera->bUsePawnControlRotation = false; 

	SetbCanMove(true);


}
void APlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// PlayerMesh를 리플리케이션에 추가
	DOREPLIFETIME(APlayableCharacter, PlayerMesh);
	DOREPLIFETIME(APlayableCharacter, PlayerEmo);

	DOREPLIFETIME(APlayableCharacter, bIsParrying);
	DOREPLIFETIME(APlayableCharacter, bParryFailed);
	DOREPLIFETIME(APlayableCharacter, bIsStunned);
	DOREPLIFETIME(APlayableCharacter, bHasWeapon);
}

void APlayableCharacter::OnHandCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 서버에서만 타격 및 데미지 판정을 수행하도록 권한 체크 (클라이언트는 시각적 예측만 담당)
	if (!HasAuthority()) return;

	bIshitted = true;

	if (OtherActor && OtherComp && (OtherActor != this) && (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel1) && bIsAttacking)		//공격 타겟이 Player일 때
	{
		APlayableCharacter* Target = Cast<APlayableCharacter>(OtherActor);
		if (!Target) return;

		if (bIsLeftPunch && Target->bIsLeftDodge) return;		//왼쪽 공격 체크
		if (bIsRightPunch && Target->bIsRightDodge) return;

		// 패링 판정: 타겟이 패링 중이면 공격자(this)에게 게이지 추가
		if (Target->GetbIsParrying())
		{
			bIsAttacking = false;
			ParryGauge += 1.0f;
			if (ParryGauge >= MaxParryGauge)
			{
				ParryGauge = 0.0f;
				ApplyStun();
			}
			return;
		}

		// 피니셔 판정: 타겟이 스턴 중이면 피니셔 애니메이션 + 큰 데미지
		if (Target->GetbIsStunned())
		{
			bIsAttacking = false;
			SetbCanMove(false);
			Target->EndStun();
			if (ExecutionMontage)
			{
				UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
				if (AnimInst)
				{
					AnimInst->Montage_Play(ExecutionMontage, 1.0f);
					FOnMontageEnded EndDelegate;
					EndDelegate.BindUObject(this, &APlayableCharacter::OnExecutionMontageEnded);
					AnimInst->Montage_SetEndDelegate(EndDelegate, ExecutionMontage);
				}
			}
			Target->TakeDamage(60.0f);
			return;
		}

		// 패링 실패: 윈도우 만료 후 애니메이션 중 피격 → 피해 증가
		if (Target->GetbParryFailed())
		{
			bIsAttacking = false;
			Target->TakeDamage(40.0f);
			return;
		}

		// 일반 공격
		bIsAttacking = false;
		Target->TakeDamage(20.0f);

		GG_LOG(LogTemp, Warning, TEXT("Hand collision detected with: %s"), *OtherActor->GetName());
		GG_LOG(LogTemp, Warning, TEXT("Count: %d"), ++Count);
	}

}


void APlayableCharacter::TakeDamage(float Damage)
{
	//GG_LOG(LogGGNetwork, Log, TEXT("TakeDamage"));

	if (!Stat) return;
	Stat->ApplyDamage(Damage);

}


// Called when the game starts or when spawned
void APlayableCharacter::BeginPlay()
{
	GG_LOG(LogTemp, Log, TEXT("Start"));

	Super::BeginPlay();

	CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrossHairWidget);
	if (CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->AddToViewport();
	}


	if (FollowCamera) {
		//CameraDefaultFOV = GetFollowCamera()->FieldOfView;
	}

	GG_LOG(LogTemp, Log, TEXT("End"));

}
void APlayableCharacter::OnRep_SetSkinAndEmotion()
{

	GG_LOG(LogTemp, Log, TEXT("Apply"));

	//클라이언트에서 실행
	GetMesh()->SetSkeletalMeshAsset(PlayerMesh);

	GetMesh()->SetMaterial(1, PlayerEmo);

}


void APlayableCharacter::Server_LoadAndApplySavedData_Implementation(USkeletalMesh* NewSkin, UMaterialInstance* NewEmotion)
{
	//서버에서 실행 → 변수 변경, 클라이언트 OnRep 호출됨
	PlayerMesh = NewSkin;
	PlayerEmo = NewEmotion;

	//서버에서 적용
	OnRep_SetSkinAndEmotion();
}



// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HPWidget) return;

	
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!CameraManager) return;

	
	FVector StartLocation = HPWidget->GetComponentLocation();
	FVector TargetLocation = CameraManager->GetRootComponent()->GetComponentLocation();

	
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	// 회전 적용
	HPWidget->SetWorldRotation(LookAtRotation);
}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayableCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayableCharacter::Look);

		//Attack

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &APlayableCharacter::LeftPunch);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &APlayableCharacter::RightPunch);



		EnhancedInputComponent->BindAction(LeftDodgeAction, ETriggerEvent::Started, this, &APlayableCharacter::LeftDodge);
		EnhancedInputComponent->BindAction(LeftDodgeAction, ETriggerEvent::Completed, this, &APlayableCharacter::LeftDodge);

		EnhancedInputComponent->BindAction(RightDodgeAction, ETriggerEvent::Started, this, &APlayableCharacter::RightDodge);
		EnhancedInputComponent->BindAction(RightDodgeAction, ETriggerEvent::Completed, this, &APlayableCharacter::RightDodge);

		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &APlayableCharacter::Parry);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void APlayableCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && bCanMove)				//!isLeftPunch && !isRightPunch && !isLeftDodge && !isRightDodge
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayableCharacter::Look(const FInputActionValue& Value)
{
	// Can't Look around when in zoom state

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayableCharacter::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
}

void APlayableCharacter::LeftPunch()
{
	if (bIsStunned) return;
	Server_LeftPunch();
}
void APlayableCharacter::Server_LeftPunch_Implementation()
{
	if (bIsStunned) return;
	Multicast_LeftPunch();
}

void APlayableCharacter::Multicast_LeftPunch_Implementation()
{
	if (bIsStunned) return;

	SetbCanMove(false);

	if (!GetHasWepaon())
	{
		if (!bIsLeftPunch)
		{
			bIsAttacking = true;
			PlayAnimMontage(LeftPunchMontange, PunchAnimSpeed); // 무기 없을 때 애니메이션 재생
			bIsLeftPunch = true;

			UE_LOG(LogTemp, Warning, TEXT("LeftPunch"));
		}
	}


}


void APlayableCharacter::RightPunch()
{
	if (bIsStunned) return;
	Server_RightPunch();
}
void APlayableCharacter::Server_RightPunch_Implementation()
{
	if (bIsStunned) return;
	Multicast_RightPunch();
}

void APlayableCharacter::Multicast_RightPunch_Implementation()
{
	if (bIsStunned) return;

	SetbCanMove(false);

	if (!bIsRightPunch) {
		bIsAttacking = true;
		PlayAnimMontage(RightPunchMontange, PunchAnimSpeed);
		bIsRightPunch = true;
	}
}

void APlayableCharacter::MoveCameraToRangedWeaponPosition()
{
	CameraBoom->SetRelativeLocation(RangedWeaponCameraPosition);
	FollowCamera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

void APlayableCharacter::MoveCameraToDefaultPosition()
{
	CameraBoom->SetRelativeLocation(DefaultCameraPosition);
	FollowCamera->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
}

void APlayableCharacter::UpdateScore(float score)
{
	Score += score;
}

void APlayableCharacter::LeftDodge()
{
	Server_LeftDodge();
	UE_LOG(LogTemp, Warning, TEXT("isLeftDodge: %s"), bIsLeftDodge ? TEXT("true") : TEXT("false"));
}
void APlayableCharacter::Server_LeftDodge_Implementation()
{
	Multicast_LeftDodge();
}



void APlayableCharacter::Multicast_LeftDodge_Implementation()
{
	if (bIsLeftDodge)
	{
		bIsLeftDodge = false;
		SetbCanMove(true);
	}

	else		//dodge 시작
	{
		bIsLeftDodge = true;

		SetbCanMove(false);
	}
}



void APlayableCharacter::RightDodge()
{
	Server_RightDodge();
	UE_LOG(LogTemp, Warning, TEXT("isRigtDodge: %s"), bIsRightDodge ? TEXT("true") : TEXT("false"));
}
void APlayableCharacter::Server_RightDodge_Implementation()
{
	Multicast_RightDodge();
}



void APlayableCharacter::Multicast_RightDodge_Implementation()
{
	if (bIsRightDodge)
	{
		bIsRightDodge = false;
		SetbCanMove(true);
	}

	else
	{
		bIsRightDodge = true;
		SetbCanMove(false);
	}
}


void APlayableCharacter::Parry()
{
	if (bIsStunned || bIsParrying) return;

	// 클라이언트 예측하기(로컬 플레이어인 경우 서버 응답을 기다리지 않고 즉시 패링 시작)
	if (IsLocallyControlled())
	{
		Local_ParryStart();
	}
	// 서버에 패링 사실 알림
	Server_Parry(GetWorld()->GetTimeSeconds());
}

void APlayableCharacter::Local_ParryStart()
{
	if (bIsStunned) return;

	bIsParrying = true;
	SetbCanMove(false);

	// 패링 애니메이션 재생
	if (ParryMontage)	
	{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			if (!AnimInst->Montage_IsPlaying(ParryMontage))
			{
				AnimInst->Montage_Play(ParryMontage, 1.0f);
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &APlayableCharacter::OnParryMontageEnded);
				AnimInst->Montage_SetEndDelegate(EndDelegate, ParryMontage);
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		ParryWindowTimerHandle,
		this,
		&APlayableCharacter::EndParryWindow,
		ParryWindowDuration,
		false
	);
}

void APlayableCharacter::Server_Parry_Implementation(float ClientTimestamp)
{
	// 서버에서도 애니메이션 로직을 실행하여 서버 화면 및 다른 클라이언트 동기화 보장
	Local_ParryStart();
}

void APlayableCharacter::OnRep_bIsParrying()
{
	if (bIsParrying)
	{
		// 로컬 플레이어(클라이언트)는 이미 예측해서 실행했으므로 Proxy들만 실행
		if (!IsLocallyControlled())
		{
			Local_ParryStart();
		}
	}
}

void APlayableCharacter::OnRep_bParryFailed()
{
	// 패링 실패에 대한 시각적/사운드 피드백 처리
}

void APlayableCharacter::EndParryWindow()
{
	bIsParrying = false;
	bParryFailed = true;	//패링 윈도우 만료, 피해 취약 상태 진입
}

void APlayableCharacter::OnParryMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 새 패링 시작으로 몽타주가 중단된 경우 상태 유지
	if (bInterrupted && bIsParrying && !bIsStunned)
	{
		return;
	}

	bIsParrying = false;
	bParryFailed = false;
	if (!bIsStunned)
	{
		SetbCanMove(true);
	}
	GetWorldTimerManager().ClearTimer(ParryWindowTimerHandle);
}


void APlayableCharacter::ApplyStun()
{
	if (!HasAuthority()) return;

	Multicast_ApplyStun();

	GetWorldTimerManager().SetTimer(
		StunTimerHandle,
		this,
		&APlayableCharacter::EndStun,
		StunDuration,
		false
	);

}

void APlayableCharacter::Multicast_ApplyStun_Implementation()
{
	bIsStunned = true;
	SetbCanMove(false);
	bIsAttacking = false;
	bIshitted = false;

	if (StunMontage)
	{
		PlayAnimMontage(StunMontage, 1.0f);
	}
}

void APlayableCharacter::EndStun()
{
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	GetWorldTimerManager().ClearTimer(StunLogTimerHandle);
	Multicast_EndStun();
}

void APlayableCharacter::Multicast_EndStun_Implementation()
{
	bIsStunned = false;
	bIsParrying = false;
	bParryFailed = false;
	bIsLeftPunch = false;
	bIsRightPunch = false;
	bIsAttacking = false;
	ParryGauge = 0.0f;
	GetWorldTimerManager().ClearTimer(ParryWindowTimerHandle);
	SetbCanMove(true);
}

void APlayableCharacter::OnExecutionMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	SetbCanMove(true);
}

void APlayableCharacter::SetHasWeapon(bool bNewHasWeapon)
{
	bHasWeapon = bNewHasWeapon;
	
	//무기 InputMappingContext 업데이트
	if (bHasWeapon)		//무기를 들었을 때 
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Weapon"));
				Subsystem->AddMappingContext(WeaponMappingContext, 1);
			}
		}
	}
	else					//무기 없을 때
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(WeaponMappingContext);
			}
		}
	}
}