// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ResetLeftAttack.h"
#include "Character/PlayableCharacter.h"
#include "Components/CapsuleComponent.h"
void UAnimNotify_ResetLeftAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//UE_LOG(LogTemp, Warning, TEXT("AttackStartNotify"));
	if (MeshComp)
	{
		APlayableCharacter* player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (player)
		{
			player->SetBisAttacking(false);
			player->SetIsLeftPunch(false);
			player->GetLeftHandCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			player->SetbCanMove(true);
		}

	}
}
