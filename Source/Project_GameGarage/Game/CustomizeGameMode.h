// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomizeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API ACustomizeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	ACustomizeGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
