// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ResetbCanMove.h"
#include "AnimNotify_ResetbCanMove.h"
#include "Character/PlayableCharacter.h"

void UAnimNotify_ResetbCanMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		APlayableCharacter* Player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			Player->SetbCanMove(true);
		}

	}
	
}
