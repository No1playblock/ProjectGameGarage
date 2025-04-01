// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WeaponAnimInstance.h"
#include "Character/PlayableCharacter.h"
#include "Weapon/WeaponActor.h"
#include "Weapon/MeleeWeapon/MeleeWeaponActor.h"
#include "Weapon/MeleeWeapon/MeleeWeaponComponent.h"
#include "Weapon/RangedWeapon/RangedWeaponActor.h"
#include "Weapon/RangedWeapon/RangedWeaponComponent.h"

UWeaponAnimInstance::UWeaponAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimSequenceBase> IdleAnim(TEXT("/Script/Engine.AnimSequence'/Game/MuscleCat/Demo/ThirdPersonIdle.ThirdPersonIdle'"));
	if (IdleAnim.Succeeded())
	{
		BaseIdle = IdleAnim.Object;
	}
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		bIsWeapon = Player->GetHasWepaon();

		if (bIsWeapon)
		{
			AWeaponActor* equipWeapon = Player->GetEquippedWeapon();
			if (equipWeapon->GetWeaponType() == EWeaponType::Melee)
			{
				Idle = Cast<AMeleeWeaponActor>(equipWeapon)->GetMeleeComponent()->GetIdleAnim();
			}
			else if (equipWeapon->GetWeaponType() == EWeaponType::Ranged)
			{
				Idle = Cast<ARangedWeaponActor>(equipWeapon)->GetRangedComponent()->GetIdleAnim();
			}
		}
		else
		{
			Idle = BaseIdle;
		}
	}
}