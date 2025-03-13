// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CustomizeGameMode.h"
#include "SaveFile.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "Player/CustomizePlayerController.h"
#include "Project_GameGarage.h"

ACustomizeGameMode::ACustomizeGameMode()
{
    PlayerControllerClass = ACustomizePlayerController::StaticClass();
}


void ACustomizeGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    GG_LOG(LogTemp, Warning, TEXT("ControllerName: %s"), *NewPlayer->GetFName().ToString());
    UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("HasAuthority"));
        if (!UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0))		//������ ����
        {
            UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass());
            return;
        }

        USaveFile* SaveData = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));
        if (SaveData)
        {
            APlayableCharacter* PlayerCharacter = Cast<APlayableCharacter>(NewPlayer->GetPawn());
            if (PlayerCharacter)
            {
                UE_LOG(LogTemp, Warning, TEXT("Server_LoadAndApplySavedData"));
                PlayerCharacter->Server_LoadAndApplySavedData(SaveData->SaveSkin, SaveData->SaveEmo);

            }
        }
    }

}