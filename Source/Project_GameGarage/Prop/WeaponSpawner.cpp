// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"
#include "Weapon/WeaponActor.h"
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
	if (HasAuthority())
	{
		weaponInstance = GetWorld()->SpawnActor<AWeaponActor>(weapon, GetActorTransform());
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AWeaponSpawner::Spawn, spawnDelay, true);
	}
	

}

void AWeaponSpawner::Spawn()
{
	if (HasAuthority())
	{
		if (!weaponInstance.IsValid()) // �����Ǿ��ų� nullptr�̸�
		{
			weaponInstance = GetWorld()->SpawnActor<AWeaponActor>(weapon, GetActorTransform());
		}
		else if (weaponInstance->GetAttachParentActor()) // �پ������� ���� ����
		{
			//weaponInstance->Destroy();
			weaponInstance = GetWorld()->SpawnActor<AWeaponActor>(weapon, GetActorTransform());
		}
	}
	


	
}

