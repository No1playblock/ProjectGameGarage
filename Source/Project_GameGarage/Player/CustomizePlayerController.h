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
	

protected:
    
    virtual void BeginPlay() override;

    ACustomizePlayerController();

    virtual void Tick(float DeltaSeconds) override;
    
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> CustomizeWidgetClass;

    
private:
    

    UPROPERTY()
    TObjectPtr<UCustomizingWidget> CustomizeWidget;

    void LoadSaveFileAndInitUI();
    void SetCustomizeLevel();
};
