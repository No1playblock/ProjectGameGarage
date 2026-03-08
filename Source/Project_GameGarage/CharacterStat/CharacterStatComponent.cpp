// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"
#include "Character/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	MaxHp = 100.0f;
	CurrentHp = MaxHp;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()		//۵
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("StatBeginPlay"));
	SetHp(MaxHp);			//۵
}

void UCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterStatComponent, CurrentHp);
}

float UCharacterStatComponent::ApplyDamage(float Damage)
{
	const float PrevHp = CurrentHp;			//
	const float ActualDamage = FMath::Clamp<float>(Damage, 0.0f, Damage);			//
//PrveHp-ActualDamage .
	SetHp(PrevHp - ActualDamage);

	return ActualDamage;
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	//UE_LOG(LogTemp, Warning, TEXT("SetHP: %f"), NewHp);		//.
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}

void UCharacterStatComponent::OnRep_CurrentHp()
{
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}


