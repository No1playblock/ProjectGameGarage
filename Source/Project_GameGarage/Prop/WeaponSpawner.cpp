// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"
#include "Weapon/WeaponActor.h"
#include "TimerManager.h"

// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnDelay = 1.0f;
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		WeaponInstance = GetWorld()->SpawnActor<AWeaponActor>(Weapon, GetActorTransform());
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWeaponSpawner::Spawn, SpawnDelay, true);
	}
	

}

void AWeaponSpawner::Spawn()
{
	if (HasAuthority())
	{
		if (!WeaponInstance.IsValid()) // 삭제되었거나 nullptr이면
		{
			WeaponInstance = GetWorld()->SpawnActor<AWeaponActor>(Weapon, GetActorTransform());
		}
		else if (WeaponInstance->GetAttachParentActor()) // 붙어있으면 새로 스폰
		{
			WeaponInstance = GetWorld()->SpawnActor<AWeaponActor>(Weapon, GetActorTransform());
		}
	}
}

