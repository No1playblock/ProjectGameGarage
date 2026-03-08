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
	bIsHit = false;
}

void AMeleeWeaponActor::OverlapmeleeComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 서버에서만 타격 및 데미지 판정을 수행하도록 권한 체크 추가 (지연 보상 및 보안)
	if (!HasAuthority()) return;

	// 무기를 장착한 부모, 자신, 또는 다른 무기와의 충돌 무시
	if (OtherActor && GetAttachParentActor() != OtherActor && OtherActor != this && !OtherActor->IsA(AWeaponActor::StaticClass()))
	{
		if (!bIsHit)
		{
			bIsHit = true;

			UE_LOG(LogTemp, Warning, TEXT("OverlapmeleeComponent: %s"), *OtherActor->GetName());
			APlayableCharacter* Target = Cast<APlayableCharacter>(OtherActor);
			if (!Target) return;

			APlayableCharacter* Attacker = Cast<APlayableCharacter>(GetAttachParentActor());

			// 패링 판정: 타겟이 패링 중이면 공격자에게 스턴 적용
			if (Target->GetbIsParrying())
			{
				if (Attacker)
				{
					Attacker->ApplyStun();  // 직접 스턴 적용 (게이지 없이 무기 패링은 즉시)
				}
				return;
			}

			// 피니셔 판정: 타겟이 스턴 중이면 큰 데미지 + 스턴 해제
			if (Target->GetbIsStunned())
			{
				Target->EndStun();
				Target->TakeDamage(60.0f);
				return;
			}

			// 일반 공격
			Target->TakeDamage(40.0f);
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

void AMeleeWeaponActor::ForceEquip(APlayableCharacter* TargetCharacter)
{
	//픽업 콜리전 비활성화 (장착 후 PickUpComponent가 캐릭터와 겹쳐 재트리거되는 것 방지)
	PickUpComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (meleeComponent->AttachWeapon(TargetCharacter))
	{
		TargetCharacter->SetEquippedWeapon(this);
	}
}


void AMeleeWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	meleeComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponActor::OverlapmeleeComponent);
}

