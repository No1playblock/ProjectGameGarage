// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GGPlayerController.h"
#include "CustomizePlayerController.generated.h"

/**
 * 
 */
class UCustomizingWidget;
class UUserWidget;
UCLASS()
class PROJECT_GAMEGARAGE_API ACustomizePlayerController : public AGGPlayerController
{
	GENERATED_BODY()

    
private:
    
    ACustomizePlayerController();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    void LoadSaveFileAndInitUI();
    
    void SetCustomizeLevel();

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> CustomizeWidgetClass;

    UPROPERTY()
    TObjectPtr<UCustomizingWidget> CustomizeWidget;

    
};
