// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGGameInstance.generated.h"

class USaveFile;
/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API UGGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	void SavePlayerData(int32 InSkinButtonIndex, int32 InEmoButtonIndex, USkeletalMesh* InSaveSkin, UMaterialInstance* InSaveEmo);

	USaveFile* LoadPlayerData();

private:

	FString SaveSlotName = TEXT("PlayerSaveSlot");

};

