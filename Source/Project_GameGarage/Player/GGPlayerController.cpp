// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerController.h"
#include "Project_GameGarage.h"
#include "Game/GGGameInstance.h"
#include "Game/SaveFile.h"
#include "Character/PlayableCharacter.h"
#include "EngineUtils.h"


void AGGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//�������� �ʰ� ���ÿ��� ����ǵ��� GameInstance ���
	UGGGameInstance* GI = Cast<UGGGameInstance>(GetGameInstance());
	if (GI)
	{
		USaveFile* SaveData = GI->LoadPlayerData();
		if (SaveData)
		{
			if (GetPawn())
			{
				Cast<APlayableCharacter>(GetPawn())->Server_LoadAndApplySavedData(SaveData->SaveSkin, SaveData->SaveEmo);
			}
		}
	}
}






