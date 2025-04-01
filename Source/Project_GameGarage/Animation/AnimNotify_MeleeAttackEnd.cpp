// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MeleeAttackEnd.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "Weapon/MeleeWeapon/MeleeWeaponActor.h"
#include "Weapon/MeleeWeapon/MeleeWeaponComponent.h"

void UAnimNotify_MeleeAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//UE_LOG(LogTemp, Warning, TEXT("AttackEndNotify"));
	if (MeshComp)
	{
		APlayableCharacter* Player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			AMeleeWeaponActor* MeleeWeapon = Cast<AMeleeWeaponActor>(Player->GetEquippedWeapon());

			MeleeWeapon->GetMeleeComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MeleeWeapon->bIsHit = false;
			
			//UE_LOG(LogTemp, Warning, TEXT("AttackEnd"));
		}
		
	}
}