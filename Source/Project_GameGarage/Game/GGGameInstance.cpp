// Fill out your copyright notice in the Description page of Project Settings.

#include "GGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SaveFile.h"
#include "Project_GameGarage.h"


void UGGGameInstance::SavePlayerData(int32 InSkinButtonIndex, int32 InEmoButtonIndex, USkeletalMesh* InSaveSkin, UMaterialInstance* InSaveEmo)
{
    USaveFile* SaveGameInstance = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));

    if (SaveGameInstance)
    {
        SaveGameInstance->SkinButtonIndex = InSkinButtonIndex;
        SaveGameInstance->EmoButtonIndex = InEmoButtonIndex;
        SaveGameInstance->SaveEmo = InSaveEmo;
        SaveGameInstance->SaveSkin = InSaveSkin;


        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
        {
            UE_LOG(LogTemp, Warning, TEXT("Game saved successfully"));
        }
        else
            UE_LOG(LogTemp, Error, TEXT("Failed to save game"));
    }
}

USaveFile* UGGGameInstance::LoadPlayerData()
{
    USaveFile* SaveGameInstance;

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))           
    {
        SaveGameInstance = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }
    else            //없으면 생성
    {
        SaveGameInstance = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
        
        if (!SaveGameInstance) // 
        {
            UE_LOG(LogTemp, Error, TEXT("CreateSaveGameObject() Fail!"));
            return nullptr;
        }

        bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);

        if (!bSuccess)
        {
            UE_LOG(LogTemp, Error, TEXT("SaveGameToSlot() Fail!"));
            return nullptr;
        }
    }

    if (SaveGameInstance)
    {
        return SaveGameInstance;
    }
    else
    {   
        UE_LOG(LogTemp, Warning, TEXT("Fail, NullPtr"));
        return nullptr;
    }
}
