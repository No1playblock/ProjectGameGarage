// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizingWidget.generated.h"

/**
 * 
 */
class UButton;
class USaveFile;
class UDataTable;
class UCustomizePanel;
UCLASS()
class PROJECT_GAMEGARAGE_API UCustomizingWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

    void InitializeButtonArrays();

    void LoadSaveGame();


    UFUNCTION()
    void OnEmoBtnClicked();

    UFUNCTION()
    void OnSkinBtnClicked();

    void SetClickType(UButton* ClickBtn);

    UFUNCTION()
    void OnSaveButtonClicked();

    

    // Emo 및 Skin 패널 버튼
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UButton> EmoBtn;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UButton> SkinBtn;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UCustomizePanel> SkinPanel;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UCustomizePanel> EmoPanel;
    
    // Save 버튼
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UButton> SaveBtn;

    // SkinButton
    int userIndex;


    UPROPERTY(BlueprintReadWrite, Category = "Selection")
    TObjectPtr<UButton> SelectedTypeButton;

    // Save Game Object
    /*UPROPERTY(BlueprintReadWrite, Category = "Save Game")
    TObjectPtr<USaveFile> SaveGameObject;*/

public:
    void InitSelectedButton(int32 SkinIndex, int32 EmoIndex);
};
