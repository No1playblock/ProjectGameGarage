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
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPickUpComponent* pickUpComponent;

	UFUNCTION(BlueprintCallable)
	virtual void PickUpWeapon(APlayableCharacter* PickUpCharacter);

public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType weapontype;

};
