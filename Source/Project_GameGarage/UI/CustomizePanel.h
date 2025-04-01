// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizePanel.generated.h"

/**
 * 
 */
class UCustomButtonWidget;

UENUM(BlueprintType)
enum class ECustomizeUIType : uint8
{
	Skin       UMETA(DisplayName = "Skin"),         
	Emo        UMETA(DisplayName = "Emotion"),         
};


UCLASS()
class PROJECT_GAMEGARAGE_API UCustomizePanel : public UUserWidget
{
	GENERATED_BODY()
	

public:

	FORCEINLINE UCustomButtonWidget* GetSelectedButton() { return SelectedButton; }

	FORCEINLINE int32 GetSelectedButtonIndex() { return SelectedButtonIndex; }

	void SetSelectedButton(int32 index);

	void ButtonClicked(UCustomButtonWidget* NewButton);


private:

	void CreateCustomizeSkinPanel(int32 ColumnNum);

	void CreateCustomizeEmoPanel(int32 ColumnNum);

	virtual void NativeConstruct() override;
	
	int32 GetColumnCount(FName columnName);

	//ÄÄÆ÷³ÍÆ®
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> SizeBox;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UScrollBox> ScrollBox;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGridPanel> GridPanel;

	//¸â¹öº¯¼ö
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CustomButtonWidget;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ECustomizeUIType CustomizeType;

	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDataTable> CustomizeDataTable;



	TArray<UCustomButtonWidget*> Buttons;

	TObjectPtr<UCustomButtonWidget> SelectedButton;

	int32 SelectedButtonIndex;

};
