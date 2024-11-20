// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

enum EScoreType : uint8
{
	HitPlayer = 50,
	KillPlayer = 100
};

UCLASS()
class PROJECT_GAMEGARAGE_API AScoreManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreManager();

public:	
	void UpdateScoreByScoreType(APlayableCharacter* character, EScoreType scroeType);

private:
	

};
