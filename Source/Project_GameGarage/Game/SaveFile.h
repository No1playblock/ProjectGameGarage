// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveFile.generated.h"

/**
 * 
 */
class USkeletalMesh;
class UMaterialInstance;
UCLASS()
class PROJECT_GAMEGARAGE_API USaveFile : public USaveGame
{
	GENERATED_BODY()
	USaveFile();

public:
	UPROPERTY()
	int32 SkinButtonIndex;

	UPROPERTY()
	int32 EmoButtonIndex;

	UPROPERTY()
	USkeletalMesh* SaveSkin;

	UPROPERTY()
	UMaterialInstance* SaveEmo;
	
};
