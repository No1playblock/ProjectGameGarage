// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponComponent.h"
#include "PlayableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMeleeWeaponComponent::UMeleeWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	
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

	
	
	// Check that the character is valid, and has no weapon component yet
	// || Character->GetInstanceComponents().FindItemByClass<UMeleeWeaponComponent>()
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
			Subsystem->AddMappingContext(SwingMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Swing
			
			EnhancedInputComponent->BindAction(SwingAction, ETriggerEvent::Triggered, this, &UMeleeWeaponComponent::Swing);
		}
	}
	return true;	
}

void UMeleeWeaponComponent::Swing()
{
	if (SwingAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(SwingAnimation, animSpeed);
		}
	}
	Server_Swing();
}

void UMeleeWeaponComponent::Server_Swing_Implementation()
{
	Multicast_Swing();
}
void UMeleeWeaponComponent::Multicast_Swing_Implementation()
{
	if (SwingSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwingSound, Character->GetActorLocation());
	}
	if (SwingAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(SwingAnimation, animSpeed);
		}
	}
}


bool UMeleeWeaponComponent::Server_Swing_Validate()
{
	return true;
}

bool UMeleeWeaponComponent::Multicast_Swing_Validate()
{
	return true;
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