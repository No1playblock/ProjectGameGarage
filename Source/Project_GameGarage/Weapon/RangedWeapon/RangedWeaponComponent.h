// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "RangedWeaponComponent.generated.h"

/**
 *
 */
class APlayableCharacter;
class UInputAction;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GAMEGARAGE_API URangedWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	UFUNCTION()
	bool AttachWeapon(APlayableCharacter* TargetCharacter);

	FORCEINLINE UAnimSequence* GetIdleAnim() const { return IdleAnimation; }

private:

	URangedWeaponComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Fire();

	void Zoom();

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void RangedWeaponLook();

	UFUNCTION()
	void CaculateFirePostion();

	UFUNCTION(Server, Reliable)
	void Server_CaculateFirePostion();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CaculateFirePostion();


	UPROPERTY(EditAnywhere)
	float animSpeed;

	UPROPERTY(EditAnywhere)
	FTransform RelativeTransform;

	TObjectPtr<APlayableCharacter> Character;

	bool bZoom;

	float CameraDefaultFOV = 90.f;
	float CameraZoomedFOV = 60.0f;

	FTimerHandle RotationTimer;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	TObjectPtr<USoundBase> FireSound;

	//Animation
	UPROPERTY(EditAnywhere, Category = Gameplay)
	TObjectPtr<UAnimMontage> FireAnimation;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimSequence> IdleAnimation;

	//Input
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> FireMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

};
