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

public:
    void InitSelectedButton(int32 SkinIndex, int32 EmoIndex);

private:

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnEmoBtnClicked();

    UFUNCTION()
    void OnSkinBtnClicked();

    UFUNCTION()
    void OnSaveButtonClicked();

    void SetClickType(UButton* ClickBtn);

    // ÄÄÆ÷³ÍÆ®
    UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UButton> EmoBtn;

    UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UButton> SkinBtn;

    UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCustomizePanel> SkinPanel;
    
    UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCustomizePanel> EmoPanel;
    
    UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UButton> SaveBtn;

    //¸â¹öº¯¼ö
    UPROPERTY()
    TObjectPtr<UButton> SelectedTypeButton;

};
