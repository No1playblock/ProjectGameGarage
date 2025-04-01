// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MeleeAttackStart.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "Weapon/MeleeWeapon/MeleeWeaponActor.h"
#include "Weapon/MeleeWeapon/MeleeWeaponComponent.h"

void UAnimNotify_MeleeAttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		APlayableCharacter* Player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			Cast<AMeleeWeaponActor>(Player->GetEquippedWeapon())->GetMeleeComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			
		}
		
	}
	
}
