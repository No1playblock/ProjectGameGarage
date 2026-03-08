// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PlayableCharacter.h"
#include "Weapon/WeaponActor.h"
#include "MeleeWeaponActor.generated.h"



class UPickUpComponent;
class UMeleeWeaponComponent;

UCLASS()
class PROJECT_GAMEGARAGE_API AMeleeWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMeleeWeaponComponent* GetMeleeComponent() { return meleeComponent; }

	//충돌 한번만 확인하기 위해서
	bool bIsHit;

	FORCEINLINE void SetIsHit(bool bNewIsHit) { bIsHit = bNewIsHit; }

	// 더미/AI용 강제 장착 (PickUpComponent 없이 직접 장착)
	UFUNCTION(BlueprintCallable)
	void ForceEquip(APlayableCharacter* TargetCharacter);

private:
	
	AMeleeWeaponActor();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapmeleeComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMeleeWeaponComponent> meleeComponent;

	virtual void PickUpWeapon(APlayableCharacter* PickUpCharacter) override;
};
