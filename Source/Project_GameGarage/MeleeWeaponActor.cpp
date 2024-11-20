// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponActor.h"
#include "PickUpComponent.h"
#include "MeleeWeaponComponent.h"
#include "PlayableCharacter.h"

// Sets default values
AMeleeWeaponActor::AMeleeWeaponActor()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meleeComponent = CreateDefaultSubobject<UMeleeWeaponComponent>(TEXT("meleeWeaponComponent"));
	RootComponent = meleeComponent;
	

	pickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComponent"));
	pickUpComponent->SetupAttachment(meleeComponent);
	pickUpComponent->OnPickUp.AddDynamic(this, &AMeleeWeaponActor::PickUpWeapon);

	SetReplicates(true);
	//SetReplicateMovement(true);

	weapontype = EWeaponType::Melee;
}

void AMeleeWeaponActor::OverlapmeleeComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetAttachParentActor() != OtherActor && OtherActor != this)
	{
		if (!bisHit)
		{
			bisHit = true;
			UE_LOG(LogTemp, Warning, TEXT("RealHit"));
			Cast<APlayableCharacter>(OtherActor)->TakeDamage(40.0f);
		}
		
	}
}


void AMeleeWeaponActor::PickUpWeapon(APlayableCharacter* PickUpCharacter)
{

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("pickup"));
	}
	

	if (meleeComponent->AttachWeapon(PickUpCharacter))
	{
		PickUpCharacter->EquippedWeapon = this;
		//Cast<>(PickUpCharacter->GetMesh()->GetAnimInstance())->HasWeapon(true);
		UE_LOG(LogTemp, Warning, TEXT("Attach"));
		UE_LOG(LogTemp, Warning, TEXT("%d"), weapontype);
		//FQuat NewRotation = FQuat(FRotator(-107.0f, 0.0f, 0.0f));
		FVector EulerRotation = meleeComponent->EulerRotation;
		FRotator NewRotation = FRotator::MakeFromEuler(EulerRotation);
		SetOwner(PickUpCharacter);

		SetActorRelativeRotation(NewRotation);
		
	}
}


// Called when the game starts or when spawned
void AMeleeWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	meleeComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponActor::OverlapmeleeComponent);
}

// Called every frame
void AMeleeWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
UMeleeWeaponComponent* AMeleeWeaponActor::GetMeleeComponent()
{
	return meleeComponent;
}
