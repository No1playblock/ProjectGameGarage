// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponActor.h"
#include "Weapon/PickUpComponent.h"
#include "Weapon/MeleeWeapon/MeleeWeaponComponent.h"
#include "Character/PlayableCharacter.h"
#include "Project_GameGarage.h"

// Sets default values
AMeleeWeaponActor::AMeleeWeaponActor()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meleeComponent = CreateDefaultSubobject<UMeleeWeaponComponent>(TEXT("meleeWeaponComponent"));
	RootComponent = meleeComponent;
	

	PickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(meleeComponent);
	PickUpComponent->OnPickUp.AddDynamic(this, &AMeleeWeaponActor::PickUpWeapon);

	SetReplicates(true);

	Weapontype = EWeaponType::Melee;
}

void AMeleeWeaponActor::OverlapmeleeComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetAttachParentActor() != OtherActor && OtherActor != this)	//서버에서 검증할때 히트한 위치 가져올 수 있음.
	{
		if (!bIsHit)
		{
			bIsHit = true;

			Cast<APlayableCharacter>(OtherActor)->TakeDamage(40.0f);
		}
		
	}
}


void AMeleeWeaponActor::PickUpWeapon(APlayableCharacter* PickUpCharacter)
{

	if (meleeComponent->AttachWeapon(PickUpCharacter))
	{
		SetOwner(PickUpCharacter->GetController());
		PickUpCharacter->SetEquippedWeapon(this);
	}
}


void AMeleeWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	meleeComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponActor::OverlapmeleeComponent);
}

