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
	// Sets default values for this actor's properties
	ARangedWeaponActor();
	
	virtual void PickUpWeapon(APlayableCharacter* PickUpCharacter) override;

	FORCEINLINE URangedWeaponComponent* GetRangedComponent() const { return RangedComponent; }
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URangedWeaponComponent> RangedComponent;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
