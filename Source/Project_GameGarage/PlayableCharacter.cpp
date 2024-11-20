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
#include "RangedWeaponActor.h"
#include "CrosshairWidget.h"
#include "GameFramework/PlayerState.h"
#include <Net/UnrealNetwork.h>
#include "RoundManager.h"
#include "Components/WidgetComponent.h"
#include "HPBarWidget.h"
#include "Components/ProgressBar.h"
#include <Kismet/GameplayStatics.h>
#include "ScoreManager.h"
#include "SaveFile.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"                  
#include "Net/UnrealNetwork.h"

// Sets default values
APlayableCharacter::APlayableCharacter() :
	Score(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	LeftHandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), "hand_l"); // ���� ���Ͽ� ����
	LeftHandCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Trigger�� ����
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnHandCollision);

	RightHandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(GetMesh(), "hand_r"); // ���� ���Ͽ� ����
	RightHandCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Trigger�� ����
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnHandCollision);

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnOverlapBegin);

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

	DefaultCameraPosition = FVector(0.f, 0.f, 130.f);
	RangedWeaponCameraPosition = FVector(0.f, 60.f, 80.f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeLocation(DefaultCameraPosition);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPWidget->SetupAttachment(GetMesh());

	HP = 100.0f;
	SetbCanMove(true);
	
	//HPBar->CreateDefaultSubobject<UWidgetComponent>("HPBar");
	

	CameraDefaultFOV = 0.f;
	CameraZoomedFOV = 60.f;
}
void APlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// PlayerMesh�� ���ø����̼ǿ� �߰�
	DOREPLIFETIME(APlayableCharacter, PlayerMesh);
	DOREPLIFETIME(APlayableCharacter, PlayerEmo);
}
void APlayableCharacter::SetPlayerMesh(USkeletalMesh* NewMesh)
{
	if (HasAuthority())
	{
		PlayerMesh = NewMesh;
		OnRep_PlayerMesh();
		MulticastSetPlayerMesh(NewMesh);
	}
	else
	{
		PlayerMesh = NewMesh; 
		OnRep_PlayerMesh();   
		ServerSetPlayerMesh(NewMesh); 

	}
}

void APlayableCharacter::ServerSetPlayerMesh_Implementation(USkeletalMesh* NewMesh)
{
	if (NewMesh)
	{
		PlayerMesh = NewMesh; 
		OnRep_PlayerMesh();   
	}
}

bool APlayableCharacter::ServerSetPlayerMesh_Validate(USkeletalMesh* NewMesh)
{
	return true;
}

void APlayableCharacter::MulticastSetPlayerMesh_Implementation(USkeletalMesh* NewMesh)
{
	if (NewMesh)
	{
		PlayerMesh = NewMesh;  
		OnRep_PlayerMesh();   
	}
}

bool APlayableCharacter::MulticastSetPlayerMesh_Validate(USkeletalMesh* NewMesh)
{
	return true;
}
void APlayableCharacter::OnRep_PlayerMesh()
{
	if (PlayerMesh)
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh);
	}
}

void APlayableCharacter::SetPlayerEmo(UMaterialInstance* NewEmo)
{
	if (HasAuthority())
	{
		PlayerEmo = NewEmo;
		OnRep_PlayerEmo();
		MulticastSetPlayerEmo(NewEmo);
	}
	else
	{

		PlayerEmo = NewEmo;
		OnRep_PlayerEmo();
		ServerSetPlayerEmo(NewEmo); 

	}
}

void APlayableCharacter::ServerSetPlayerEmo_Implementation(UMaterialInstance* NewEmo)
{
	if (NewEmo)
	{
		PlayerEmo = NewEmo;
		OnRep_PlayerEmo();
	}
}
bool APlayableCharacter::ServerSetPlayerEmo_Validate(UMaterialInstance* NewEmo)
{
	return true;
}
void APlayableCharacter::MulticastSetPlayerEmo_Implementation(UMaterialInstance* NewEmo)
{
	if (NewEmo)
	{
		PlayerEmo = NewEmo;
		OnRep_PlayerEmo();
	}
}
bool APlayableCharacter::MulticastSetPlayerEmo_Validate(UMaterialInstance* NewEmo)
{
	return true;
}
void APlayableCharacter::OnRep_PlayerEmo()
{
	if (PlayerMesh)
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh);
	}
}


void APlayableCharacter::OnHandCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	bishitted = true;
	//ECollisionChannel playerCollisionChannel = ECC_EngineTraceChannel1;
	if (OtherActor && (OtherActor != this) && (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel1) && bisAttacking)
	{
		// �浹 ó�� ����
		bisAttacking = false;
		Cast<APlayableCharacter>(OtherActor)->TakeDamage(20.0f);

		UE_LOG(LogTemp, Warning, TEXT("Hand collision detected with: %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Count: %d"), ++count);
	}

}
void APlayableCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())  // ���������� ����ǵ��� ����
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s: OnHit"), *GetFName().ToString());
	}

	//if (OtherComp && OtherComp->IsSimulatingPhysics())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IsSimulatingPhysics"));
	//	FName HitBoneName = SweepResult.BoneName;  // �浹�� �� �̸� ��������
	//	if (!HitBoneName.IsNone())
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("IsNone"));
	//		// �ش� ���� ���� ���� �ùķ��̼� Ȱ��ȭ
	//		GetMesh()->SetAllBodiesBelowSimulatePhysics(HitBoneName, true, false);
	//		// �浹�� ������ ���޽� �߰� (��: �� ������� ������ ����)
	//		FVector Impulse = SweepResult.ImpactNormal * 100.0f;  // ��: ���޽� ���� ����
	//		GetMesh()->AddImpulseToAllBodiesBelow(Impulse, HitBoneName);
	//		UE_LOG(LogTemp, Warning, TEXT("Simulating physics for bone: %s"), *HitBoneName.ToString());
	//	}
	//}
}

void APlayableCharacter::TakeDamage(float damage)
{
	if (HP - damage >= 0) {
		HP -= damage;
	}
	else {
		HP = 0;
	}
	Cast<UHPBarWidget>(HPWidget->GetWidget())->SetHPBar(HP);
}


// Called when the game starts or when spawned
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	LoadAndApplySavedData();
	CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrossHairWidget);
	if (CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->AddToViewport();
		AActor* roundmanager = UGameplayStatics::GetActorOfClass(GetWorld(), ARoundManager::StaticClass());
		if (roundmanager)
		{
			Cast<ARoundManager>(roundmanager)->HUDWidget = CrosshairWidgetInstance;
		}
		
	}
	if (HPWidget->GetWidget() && HP)
	{
		Cast<UHPBarWidget>(HPWidget->GetWidget())->SetHPBar(HP);
	}

	if (FollowCamera) {
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
	}
}

void APlayableCharacter::LoadAndApplySavedData()
{

	if (!UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0))
	{
		UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass());
		return;
	}
	USaveFile* SaveGameObject = Cast<USaveFile>
		(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

	if (SaveGameObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Game Loaded Successfully"));

		// �ҷ��� ���̺� �����Ϳ��� Mesh�� EmoMaterial ����
		if (SaveGameObject->SaveSkin)
		{
			SetPlayerMesh(SaveGameObject->SaveSkin);
			//GetMesh()->SetSkeletalMesh(SaveGameObject->SaveSkin);
		}

		if (SaveGameObject->SaveEmo)
		{
			SetPlayerEmo(SaveGameObject->SaveEmo);
			//GetMesh()->SetMaterial(1, SaveGameObject->SaveEmo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Save Game Found"));
	}
}

// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isZoom) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (!PlayerController) {
			return;
		}

		FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		FRotator CurrentRotation = GetActorRotation();

		FRotator TargetRotation(0.0f, CameraRotation.Yaw, 0.0f);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 0.f);

		SetActorRotation(NewRotation);
	}
}

bool APlayableCharacter::GetHasWepaon()
{
	return bHasWeapon;
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
		//EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &APlayableCharacter::LeftClick);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &APlayableCharacter::RightPunch);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &APlayableCharacter::LeftPunch);
		//EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &APlayableCharacter::LeftClick);


		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &APlayableCharacter::Zoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &APlayableCharacter::Zoom);

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
			PlayAnimMontage(LeftPunchMontange, punchAnimSpeed); // �������� �ִϸ��̼� ���
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

//void APlayableCharacter::SetScore(EScoreType scoreToAdd)
//{
//	Score += static_cast<uint8>(scoreToAdd);
//}

void APlayableCharacter::Zoom()
{
	if (!EquippedWeapon->IsA(ARangedWeaponActor::StaticClass())) {
		return;
	}

	if (isZoom) {
		isZoom = false;
		// SetbCanMove(true);
		FollowCamera->SetFieldOfView(CameraDefaultFOV);
	}
	else {
		isZoom = true;
		// SetbCanMove(false);
		FollowCamera->SetFieldOfView(CameraZoomedFOV);
	}

	if (CrosshairWidgetInstance)
	{

		//CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrossHairWidget);

		Cast<UCrosshairWidget>(CrosshairWidgetInstance)->ChangeZoomState(isZoom);
		//CrosshairWidgetInstance->ChangeZoomein
	}


	/*UCrosshairWidget* instance = Cast<UCrosshairWidget>(CrosshairWidgetInstance);
	instance->ChangeZoomState(isZoom);*/

	// UE_LOG(LogTemp, Warning, TEXT("isZoom: %s"), isZoom ? TEXT("true") : TEXT("false"));
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

	else
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

	//��Ŭ�� InputMappingContext ����
	if (bHasWeapon)		//���⸦ ������ ������ 
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
	else					//���� ������
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