// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ResetLeftAttack.h"
#include "Character/PlayableCharacter.h"
#include "Components/CapsuleComponent.h"
void UAnimNotify_ResetLeftAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		APlayableCharacter* Player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			Player->SetBisAttacking(false);
			Player->SetIsLeftPunch(false);
			Player->GetLeftHandCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Player->SetbCanMove(true);
		}

	}
}
