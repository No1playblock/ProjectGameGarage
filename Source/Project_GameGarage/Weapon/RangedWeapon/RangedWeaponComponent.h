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

	/** Sets default values for this component's properties */
	URangedWeaponComponent();

public:
	/*UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABulletActor> ProjectileClass;*/

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimSequence* IdleAnimation;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(APlayableCharacter* TargetCharacter);

	FORCEINLINE UAnimSequence* GetIdleAnim() const { return IdleAnimation; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float animSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTransform RelativeTransform;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;



	void RangedWeaponLook();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	void Zoom();


	UFUNCTION()
	void CaculateFirePostion();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CaculateFirePostion();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_CaculateFirePostion();

private:
	/** The Character holding this weapon*/
	APlayableCharacter* Character;

	bool bZoom;

	float CameraDefaultFOV = 90.f;
	float CameraZoomedFOV = 60.0f;

	FTimerHandle RotationTimer;

};
