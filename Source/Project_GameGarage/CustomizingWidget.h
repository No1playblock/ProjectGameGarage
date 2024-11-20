// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizingWidget.generated.h"

/**
 * 
 */
class UButton;
class UGridPanel;
class USkinButtonWidget;
class UEmoButtonWidget;
class UBP_SaveGame;
class USaveFile;
class UEditableTextBox;
UCLASS()
class PROJECT_GAMEGARAGE_API UCustomizingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    void InitializeButtonArrays();

    void LoadSaveGame();

    void SetSkinButton(USkinButtonWidget* NewButton);

    void SetEmotionButton(UEmoButtonWidget* NewButton);

    void FindClickEmoButton();

    void FindClickSkinButton();

    UFUNCTION()
    void OnEmoBtnClicked();

    UFUNCTION()
    void OnSkinBtnClicked();


    void SetClickType(UButton* ClickBtn);

    UFUNCTION()
    void OnSaveButtonClicked();

protected:
    // Emo 및 Skin 버튼
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* EmoBtn;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* SkinBtn;

    // Grid 패널
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UGridPanel* EmoGrid;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UGridPanel* SkinGrid;

    // Save 버튼
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* SaveBtn;


    // SkinButton
    int userIndex;


    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    USkinButtonWidget* SkinButton_0;
    
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    USkinButtonWidget* SkinButton_1;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_2;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_3;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_4;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_5;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_6;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_7;

    UPROPERTY(meta = (BindWidget))
    USkinButtonWidget* SkinButton_8;

    

    // Emotion Buttons
    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_0;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_1;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_2;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_3;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_4;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_5;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_6;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_7;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_8;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_9;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_10;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_11;

    UPROPERTY(meta = (BindWidget))
    UEmoButtonWidget* EmotionButton_12;

    // Array for Buttons
    UPROPERTY(BlueprintReadWrite, Category = "Custom Buttons")
    TArray<USkinButtonWidget*> SkinButtonArray;

    UPROPERTY(BlueprintReadWrite, Category = "Custom Buttons")
    TArray<UEmoButtonWidget*> EmoButtonArray;

    // Selected Variables
    UPROPERTY(BlueprintReadWrite, Category = "Selection")
    USkinButtonWidget* SelectedSkin;

    UPROPERTY(BlueprintReadWrite, Category = "Selection")
    UEmoButtonWidget* SelectedEmo;

    UPROPERTY(BlueprintReadWrite, Category = "Selection")
    UButton* SelectedTypeButton;

    // Save Game Object
    UPROPERTY(BlueprintReadWrite, Category = "Save Game")
    USaveFile* SaveGameObject;
};
