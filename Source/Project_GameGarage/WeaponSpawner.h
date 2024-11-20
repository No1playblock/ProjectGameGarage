// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.h"

#include "WeaponSpawner.generated.h"

UCLASS()
class PROJECT_GAMEGARAGE_API AWeaponSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponActor> weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnDelay;

private:
	AWeaponActor* weaponInstance;

	FTimerHandle spawnTimerHandle;
};
