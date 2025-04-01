// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_LeftAttackStart.h"
#include "Character/PlayableCharacter.h"
#include "Components/CapsuleComponent.h"
void UAnimNotify_LeftAttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//UE_LOG(LogTemp, Warning, TEXT("AttackStartNotify"));
	if (MeshComp)
	{
		APlayableCharacter* Player = Cast<APlayableCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			Player->GetLeftHandCollision()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

	}
}
