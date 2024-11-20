// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"
#include "WeaponActor.h"
#include "TimerManager.h"

// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	spawnDelay = 1.0f;
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();
	weaponInstance = GetWorld()->SpawnActor<AWeaponActor>(weapon, GetActorTransform());
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AWeaponSpawner::Spawn, spawnDelay, true);

}

void AWeaponSpawner::Spawn()
{
	if (IsValid(weaponInstance->GetAttachParentActor()))
	{
		weaponInstance = GetWorld()->SpawnActor<AWeaponActor>(weapon, GetActorTransform());
	}
}

