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
	void ButtonClicked(UCustomButtonWidget* NewButton);

	UCustomButtonWidget* GetSelectedButton();

	void SetSelectedButton(int32 index);
	int32 GetSelectedButtonIndex();
protected:

	

	int32 GetColumnCount(FName columnName);

	void CreateCustomizeSkinPanel(int32 ColumnNum);

	void CreateCustomizeEmoPanel(int32 ColumnNum);

	
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CustomButtonWidget;

	UPROPERTY(EditAnywhere)
	ECustomizeUIType CustomizeType;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDataTable> CustomizeDataTable;

	TArray<UCustomButtonWidget*> Buttons;

	TObjectPtr<UCustomButtonWidget> SelectedButton;

	int32 SelectedButtonIndex;

};
