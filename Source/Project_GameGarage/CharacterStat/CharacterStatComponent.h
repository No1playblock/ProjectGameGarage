// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_GAMEGARAGE_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()


public:

	//Getter
	FORCEINLINE float GetMaxHp() { return MaxHp; }

	FORCEINLINE float GetCurrentHp() { return CurrentHp; }


	float ApplyDamage(float Damage);

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

protected:

	UCharacterStatComponent();

	virtual void BeginPlay() override;
		

	void SetHp(float NewHp);


	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
		

};

