// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedWeaponComponent.h"
#include "PlayableCharacter.h"
#include "BulletActor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "DrawDebugHelpers.h" 
#include "Engine/World.h"

// Sets default values for this component's properties
URangedWeaponComponent::URangedWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
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

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 1.0f);  // 2.0f는 선의 두께

		
	}
}
bool URangedWeaponComponent::Multicast_CaculateFirePostion_Validate()
{
	return true;
}

void URangedWeaponComponent::Fire()		//총 쏘는 기능
{

	CaculateFirePostion();

	// Try and fire a projectile
	/*
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{

			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			UE_LOG(LogTemp, Warning, TEXT("Fire"));
			// Spawn the projectile at the muzzle
			World->SpawnActor<ABulletActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	*/

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	//if (FireAnimation != nullptr)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, animSpeed);
	//	}
	//}
}

bool URangedWeaponComponent::AttachWeapon(APlayableCharacter* TargetCharacter)		//플레이어에게 무기를 장착하는 메소드
{
	Character = TargetCharacter;
	Character->SetHasWeapon(true);
	TArray<AActor*> ChildActors;
	Character->GetAttachedActors(ChildActors);
	//UE_LOG(LogTemp, Warning, TEXT("RangedAttach"));
	for (AActor* ChildActor : ChildActors)
	{
		// 각 ChildActor에 대해 작업 수행
		ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ChildActor->Destroy();
		// 카메라 위치 원상복구
		TargetCharacter->MoveCameraToDefaultPosition();
	}


	
	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr)
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

															// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);					//플레이어에게 WeaponComponent를 추가해준다.

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))		//좌클릭을 눌렀을 때 Fire가 작동되도록
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Swing
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &URangedWeaponComponent::Fire);
		}
	}

	TargetCharacter->MoveCameraToRangedWeaponPosition();
	
	

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

