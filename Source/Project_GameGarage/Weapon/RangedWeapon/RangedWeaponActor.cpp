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

	pickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComponent"));
	pickUpComponent->SetupAttachment(RangedComponent);

	pickUpComponent->OnPickUp.AddDynamic(this, &ARangedWeaponActor::PickUpWeapon);


	weapontype = EWeaponType::Ranged;
}

void ARangedWeaponActor::PickUpWeapon(APlayableCharacter* PickUpCharacter)
{
	//UE_LOG(LogTemp, Warning, TEXT("pickup"));
	if (RangedComponent->AttachWeapon(PickUpCharacter))
	{
		PickUpCharacter->SetEquippedWeapon(this);
		SetOwner(PickUpCharacter);

		//Cast<>(PickUpCharacter->GetMesh()->GetAnimInstance())->HasWeapon(true);
		//UE_LOG(LogTemp, Warning, TEXT("Attach"));
	}
}


// Called when the game starts or when spawned
void ARangedWeaponActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARangedWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

