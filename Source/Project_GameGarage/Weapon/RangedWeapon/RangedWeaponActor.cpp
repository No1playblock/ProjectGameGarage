// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeaponActor.h"
#include "Weapon/PickUpComponent.h"
#include "RangedWeaponComponent.h"
#include "Character/PlayableCharacter.h"

// Sets default values
ARangedWeaponActor::ARangedWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangedComponent = CreateDefaultSubobject<URangedWeaponComponent>(TEXT("RangedWeaponComponent"));
	RootComponent = RangedComponent;

	PickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(RangedComponent);

	PickUpComponent->OnPickUp.AddDynamic(this, &ARangedWeaponActor::PickUpWeapon);


	Weapontype = EWeaponType::Ranged;
}

void ARangedWeaponActor::PickUpWeapon(APlayableCharacter* PickUpCharacter)
{

	if (RangedComponent->AttachWeapon(PickUpCharacter))
	{
		PickUpCharacter->SetEquippedWeapon(this);
		SetOwner(PickUpCharacter);

	}
}

