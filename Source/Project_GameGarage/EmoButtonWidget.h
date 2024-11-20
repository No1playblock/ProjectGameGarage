// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomButtonWidget.h"
#include "EmoButtonWidget.generated.h"

/**
 * 
 */
class UMaterialInstance;
class UCustomizingWidget;
UCLASS()
class PROJECT_GAMEGARAGE_API UEmoButtonWidget : public UCustomButtonWidget
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Appearance")
	UMaterialInstance* EmoMaterial;

	

public:
	virtual void NativeConstruct() override;

	virtual void OnButtonClicked() override;

	virtual void SetClicked(bool bIsClicked) override;

	UMaterialInstance* GetEmo();
};
