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
	
public:
	
	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	int SkinButtonIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	int EmoButtonIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	USkeletalMesh* SaveSkin;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	UMaterialInstance* SaveEmo;
};
