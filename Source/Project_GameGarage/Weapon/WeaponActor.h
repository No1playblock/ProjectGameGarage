// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"


class UPickUpComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged")
};
																	


UCLASS()
class PROJECT_GAMEGARAGE_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponActor();

	FORCEINLINE EWeaponType GetWeaponType() { return Weapontype; }

protected:
	

	UFUNCTION(BlueprintCallable)
	virtual void PickUpWeapon(APlayableCharacter* PickUpCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPickUpComponent* PickUpComponent;

	UPROPERTY(EditAnywhere)
	EWeaponType Weapontype;

};
