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
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;



	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	LeftHandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), "hand_l"); // 본의 소켓에 연결
	LeftHandCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Trigger로 설정
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnHandCollision);

	RightHandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(GetMesh(), "hand_r"); // 본의 소켓에 연결
	RightHandCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Trigger로 설정
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnHandCollision);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	//GetCharacterMovement()->JumpZVelocity = 700.f;
	//GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	/*DefaultCameraPosition = FVector(0.f, 0.f, 130.f);
	RangedWeaponCameraPosition = FVector(0.f, 60.f, 80.f);*/

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm



	//HP = 100.0f;
	SetbCanMove(true);

	//HPBar->CreateDefaultSubobject<UWidgetComponent>("HPBar");


	/*CameraDefaultFOV = 0.f;
	CameraZoomedFOV = 60.f;*/
}
void APlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// PlayerMesh를 리플리케이션에 추가
	//DOREPLIFETIME(APlayableCharacter, HP);
	DOREPLIFETIME(APlayableCharacter, PlayerMesh);
	DOREPLIFETIME(APlayableCharacter, PlayerEmo);
}

void APlayableCharacter::OnHandCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	bishitted = true;
	//ECollisionChannel playerCollisionChannel = ECC_EngineTraceChannel1;
	if (OtherActor && (OtherActor != this) && (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel1) && bisAttacking)		//맞은거 타입이 Player면
	{

		if (isLeftPunch)		//안 맞게 하기
		{
			if (Cast<APlayableCharacter>(OtherActor)->isLeftDodge)
				return;
		}
		if (isRightPunch)
		{
			if (Cast<APlayableCharacter>(OtherActor)->isRightDodge)
				return;
		}

		// 충돌 처리 로직
		bisAttacking = false;//
		ensure(Cast<APlayableCharacter>(OtherActor));
		if (Cast<APlayableCharacter>(OtherActor))
		{
			Cast<APlayableCharacter>(OtherActor)->TakeDamage(20.0f);
		}


		GG_LOG(LogTemp, Warning, TEXT("Hand collision detected with: %s"), *OtherActor->GetName());
		GG_LOG(LogTemp, Warning, TEXT("Count: %d"), ++count);
	}

}


void APlayableCharacter::TakeDamage(float damage)
{
	//GG_LOG(LogGGNetwork, Log, TEXT("TakeDamage"));

	Stat->ApplyDamage(damage);

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

	//클라이언트에서 적용
	GetMesh()->SetSkeletalMeshAsset(PlayerMesh);

	GetMesh()->SetMaterial(1, PlayerEmo);

}


void APlayableCharacter::Server_LoadAndApplySavedData_Implementation(USkeletalMesh* NewSkin, UMaterialInstance* NewEmotion)
{
	//서버에서 변경		변경했으니 값 전달하고 클라이언트에서 OnRep 실행될꺼임(클라이언트에서 적용)
	PlayerMesh = NewSkin;
	PlayerEmo = NewEmotion;

	//서버에서 적용
	OnRep_SetSkinAndEmotion();
}



// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	Server_LeftPunch();
}
void APlayableCharacter::Server_LeftPunch_Implementation()
{
	Multicast_LeftPunch();
}
bool APlayableCharacter::Server_LeftPunch_Validate()
{
	return true;
}
void APlayableCharacter::Multicast_LeftPunch_Implementation()
{

	SetbCanMove(false);

	if (!GetHasWepaon())
	{
		if (!isLeftPunch)
		{
			bisAttacking = true;
			PlayAnimMontage(LeftPunchMontange, punchAnimSpeed); // 서버에서 애니메이션 재생
			isLeftPunch = true;

			UE_LOG(LogTemp, Warning, TEXT("LeftPunch"));
		}
	}


}
bool APlayableCharacter::Multicast_LeftPunch_Validate()
{
	return true;
}

void APlayableCharacter::SetbCanMove(bool isbool)
{
	bCanMove = isbool;
}

bool APlayableCharacter::GetbCanMove()
{
	return bCanMove;
}

void APlayableCharacter::RightPunch()
{
	Server_RightPunch();
}
void APlayableCharacter::Server_RightPunch_Implementation()
{
	Multicast_RightPunch();
}
bool APlayableCharacter::Server_RightPunch_Validate()
{
	return true;
}
void APlayableCharacter::Multicast_RightPunch_Implementation()
{
	SetbCanMove(false);

	if (!isRightPunch) {
		bisAttacking = true;
		PlayAnimMontage(RightPunchMontange, punchAnimSpeed);
		isRightPunch = true;
	}
}
bool APlayableCharacter::Multicast_RightPunch_Validate()
{
	return true;
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

void APlayableCharacter::SetEmoMaterial(UMaterialInstance* EmoMaterial)
{
	GetMesh()->SetMaterial(1, EmoMaterial);
}

void APlayableCharacter::SetSkin(USkeletalMesh* Skin)
{
	GetMesh()->SetSkeletalMeshAsset(Skin);

}

void APlayableCharacter::UpdateScore(float score)
{
	Score += score;
}

void APlayableCharacter::LeftDodge()
{
	Server_LeftDodge();
	UE_LOG(LogTemp, Warning, TEXT("isLeftDodge: %s"), isLeftDodge ? TEXT("true") : TEXT("false"));
}
void APlayableCharacter::Server_LeftDodge_Implementation()
{
	Multicast_LeftDodge();
}
bool APlayableCharacter::Server_LeftDodge_Validate()
{
	return true;
}


void APlayableCharacter::Multicast_LeftDodge_Implementation()
{
	if (isLeftDodge)
	{
		isLeftDodge = false;
		SetbCanMove(true);
	}

	else		//dodge 상태
	{
		isLeftDodge = true;

		SetbCanMove(false);
	}
}
bool APlayableCharacter::Multicast_LeftDodge_Validate()
{
	return true;
}


void APlayableCharacter::RightDodge()
{
	Server_RightDodge();
	UE_LOG(LogTemp, Warning, TEXT("isRigtDodge: %s"), isRightDodge ? TEXT("true") : TEXT("false"));
}
void APlayableCharacter::Server_RightDodge_Implementation()
{
	Multicast_RightDodge();
}
bool APlayableCharacter::Server_RightDodge_Validate()
{
	return true;
}


void APlayableCharacter::Multicast_RightDodge_Implementation()
{
	if (isRightDodge)
	{
		isRightDodge = false;
		SetbCanMove(true);
	}

	else
	{
		isRightDodge = true;
		SetbCanMove(false);
	}
}
bool APlayableCharacter::Multicast_RightDodge_Validate()
{
	return true;
}

void APlayableCharacter::SetHasWeapon(bool ISHASWEAPON)
{
	bHasWeapon = ISHASWEAPON;
	
	//우클릭 InputMappingContext 변경
	if (bHasWeapon)		//무기를 가지고 있으면 
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
	else					//무기 없으면
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