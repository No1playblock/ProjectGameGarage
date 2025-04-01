// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ResetRightAttack.h"
#include "Character/PlayableCharacter.h"
#include "Components/CapsuleComponent.h"

void UAnimNotify_ResetRightAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		APlayableCharacter* Player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			Player->SetBisAttacking(false);
			Player->SetIsRightPunch(false);
			Player->GetRightHandCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Player->SetbCanMove(true);
		}

	}
}