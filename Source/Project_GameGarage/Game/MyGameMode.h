// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Prop/PressurePlate.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ERoundType : uint8
{
	WatingTime UMETA(DisplayName = "WatingTime"),
	PlayTime UMETA(DisplayName = "PlayTime")
};

UCLASS()
class PROJECT_GAMEGARAGE_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void TimerStart();

	int Timer;

	class AMyGameState* MyGS;


private:

	virtual void PostLogin(APlayerController* NewPlayer) override;


	//SwitchManager
	

	UFUNCTION()
	void OnButtonPressed(APressurePlate* Button, bool bIsPressed);

	void CheckAllButtonsPressed();

	UPROPERTY()
	TArray<APressurePlate*> Buttons;

	UPROPERTY()
	TMap<APressurePlate*, bool> ButtonStates;
	//RoundManger
	

	void TimerProgress();
	void TimerStop();

	void GameFinish();

	void GotoBattleField();

	ERoundType RoundType;
	
	FTimerHandle Timerhandle;

	int RoundCnt = 0;

	bool bGameStart;

	TArray<class APlayableCharacter*> PlayerArr;

	FVector SpawnPoint1 = FVector(7200.0f, 3600.0f, 592.0f);
	
	FVector SpawnPoint2 = FVector(8800.0f, 3600.0f, 592.0f);

	FVector Map1SpawnPoint1 = FVector(4504.3822f, -307.575225f, 124.945456f);

	FVector Map1SpawnPoint2 = FVector(3701.275128f, -89.156732f, 124.945456f);

	FVector Map2SpawnPoint1 = FVector(-4731.515289f, -2226.105225, 244.607269);

	FVector Map2SpawnPoint2 = FVector(-5653.996826f, 5950.0f, 140.0f);

	int32 PlayerNum=1;
};
