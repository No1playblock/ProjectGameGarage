// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API AGGPlayerController : public APlayerController
{
	GENERATED_BODY()



protected:
    virtual void BeginPlay() override;

};
