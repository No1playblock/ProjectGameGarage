// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/WeaponActor.h"

#include "WeaponSpawner.generated.h"

UCLASS()
class PROJECT_GAMEGARAGE_API AWeaponSpawner : public AActor
{
	GENERATED_BODY()
	

private:

	AWeaponSpawner();

	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponActor> Weapon;

	UPROPERTY(EditAnywhere)
	float SpawnDelay;
	
	UPROPERTY()
	TWeakObjectPtr<AWeaponActor> WeaponInstance;

	FTimerHandle SpawnTimerHandle;
};
