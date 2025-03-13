// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedWeaponComponent.h"
#include "Character/PlayableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "DrawDebugHelpers.h" 
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/CrosshairWidget.h"

// Sets default values for this component's properties
URangedWeaponComponent::URangedWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	PrimaryComponentTick.bCanEverTick = true;
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);


}

void URangedWeaponComponent::CaculateFirePostion()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));

	Server_CaculateFirePostion();
}

void URangedWeaponComponent::Server_CaculateFirePostion_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("serverFire"));
	Multicast_CaculateFirePostion();
}
bool URangedWeaponComponent::Server_CaculateFirePostion_Validate()
{
	return true;
}

void URangedWeaponComponent::Multicast_CaculateFirePostion_Implementation()
{
	
	UE_LOG(LogTemp, Warning, TEXT("MultiFire"));
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, animSpeed);
		}
	}
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController) {

		int32 ScreenSizeX, ScreenSizeY;
		PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);

		FVector2d ScreenCenter(ScreenSizeX * 0.5f, ScreenSizeY * 0.5f);

		FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
		FVector End = Start + (PlayerController->PlayerCameraManager->GetCameraRotation().Vector() * 5000.f);

		TArray<FHitResult> HitResults;
		FHitResult HitResult;

		// PlayerController->GetHitResultAtScreenPosition(ScreenCenter, ECC_Visibility, false, HitResult)
		// GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility)

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("Class: %s"), *HitResult.GetActor()->GetClass()->GetSuperClass()->GetFName().ToString());

			if (HitResult.GetActor()->GetClass()->GetSuperClass() && HitResult.GetActor()->GetClass()->GetSuperClass() == APlayableCharacter::StaticClass())
			{
				Cast<APlayableCharacter>(HitResult.GetActor())->TakeDamage(50.0f);
			}
		}

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 1.0f);  // 2.0f�� ���� �β�

		
	}
}
bool URangedWeaponComponent::Multicast_CaculateFirePostion_Validate()
{
	return true;
}

void URangedWeaponComponent::Fire()		//�� ��� ���
{

	CaculateFirePostion();

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

}
void URangedWeaponComponent::Zoom()
{
	
	if (bZoom) {
		bZoom = false;
		// SetbCanMove(true);
		Character->GetFollowCamera()->SetFieldOfView(CameraDefaultFOV);
		Character->GetCameraBoom()->TargetArmLength = 250.0f;
		UE_LOG(LogTemp, Warning, TEXT("Zoomout"));
	}
	else {
		bZoom = true;
		// SetbCanMove(false);
		Character->GetFollowCamera()->SetFieldOfView(CameraZoomedFOV);
		Character->GetCameraBoom()->TargetArmLength = 150.0f;
		UE_LOG(LogTemp, Warning, TEXT("Zoomin"));

	}

	if (Character->GetCrosshairWidgetInstance())
	{

		//CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrossHairWidget);

		Cast<UCrosshairWidget>(Character->GetCrosshairWidgetInstance())->ChangeZoomState(bZoom);
		//CrosshairWidgetInstance->ChangeZoomein
	}
}

bool URangedWeaponComponent::AttachWeapon(APlayableCharacter* TargetCharacter)		//�÷��̾�� ���⸦ �����ϴ� �޼ҵ�
{
	Character = TargetCharacter;
	Character->SetHasWeapon(true);
	TArray<AActor*> ChildActors;
	Character->GetAttachedActors(ChildActors);
	//UE_LOG(LogTemp, Warning, TEXT("RangedAttach"));
	for (AActor* ChildActor : ChildActors)
	{
		// �� ChildActor�� ���� �۾� ����
		ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ChildActor->Destroy();
		// ī�޶� ��ġ ���󺹱�
		TargetCharacter->MoveCameraToDefaultPosition();
	}


	
	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr)
	{
		return false;
	}

	Character->AddInstanceComponent(this);					//�÷��̾�� WeaponComponent�� �߰����ش�.

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	SetRelativeTransform(RelativeTransform);
															// add the weapon as an instance component to the character
	
	
	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))		//��Ŭ���� ������ �� Fire�� �۵��ǵ���
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Swing
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &URangedWeaponComponent::RangedWeaponLook);

			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &URangedWeaponComponent::Fire);

			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &URangedWeaponComponent::Zoom);
			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &URangedWeaponComponent::Zoom);

		}
	}

	TargetCharacter->MoveCameraToRangedWeaponPosition();
	//GetWorld()->GetTimerManager().SetTimer(RotationTimer, this, &URangedWeaponComponent::RangedWeaponLook, 0.01f, true);
	//ƽ���� �ٲ�߰ڴ�.
	
	return true;
}

void URangedWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)			
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void URangedWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	RangedWeaponLook();

}

void URangedWeaponComponent::RangedWeaponLook()
{
	//UE_LOG(LogTemp, Warning, TEXT("RangedWeaponLoock"));
	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (!PlayerController) return;

		// ī�޶��� Yaw�� �����ͼ� ĳ���� ȸ��
		FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		FRotator CurrentRotation = Character->GetActorRotation();

		FRotator TargetRotation(0.0f, CameraRotation.Yaw, 0.0f);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);  // �ε巴�� ȸ��

		Character->SetActorRotation(NewRotation);
	}
	
}