// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/WeaponActor.h"
#include "RangedWeaponActor.generated.h"


class UPickUpComponent;
class URangedWeaponComponent;

UCLASS()
class PROJECT_GAMEGARAGE_API ARangedWeaponActor : public AWeaponActor
{
	GENERATED_BODY()
	
public:	
	FORCEINLINE URangedWeaponComponent* GetRangedComponent() const { return RangedComponent; }
	
private:
	
	ARangedWeaponActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URangedWeaponComponent> RangedComponent;

	virtual void PickUpWeapon(APlayableCharacter* PickUpCharacter) override;

};
