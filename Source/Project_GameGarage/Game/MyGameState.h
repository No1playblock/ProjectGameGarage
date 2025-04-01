// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:


    UFUNCTION()
    void SetTimer(int time);

    UPROPERTY(ReplicatedUsing = OnRep_TimerChanged)
    int32 Timer;

private:

    virtual void BeginPlay() override;
    
    AMyGameState();

    UFUNCTION()
    void OnRep_TimerChanged();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

};
