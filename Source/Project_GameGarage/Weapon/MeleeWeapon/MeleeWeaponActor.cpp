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
	


	pickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComponent"));
	pickUpComponent->SetupAttachment(meleeComponent);
	pickUpComponent->OnPickUp.AddDynamic(this, &AMeleeWeaponActor::PickUpWeapon);

	SetReplicates(true);
	//SetReplicateMovement(true);

	weapontype = EWeaponType::Melee;
}

void AMeleeWeaponActor::OverlapmeleeComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetAttachParentActor() != OtherActor && OtherActor != this)	//�������� �����Ҷ� ��Ʈ�� ��ġ ������ �� ����.
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
		//SetOwner(PickUpCharacter);
		SetOwner(PickUpCharacter->GetController());
		PickUpCharacter->SetEquippedWeapon(this);
		//Cast<>(PickUpCharacter->GetMesh()->GetAnimInstance())->HasWeapon(true);
		UE_LOG(LogTemp, Warning, TEXT("PickUp"));
		UE_LOG(LogTemp, Warning, TEXT("%d"), weapontype);

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
