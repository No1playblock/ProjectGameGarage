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
	

public:

	virtual void SetClicked(bool bIsClicked) override;

	void SetEmoBtnData(UMaterialInstance* NewMaterial, UTexture2D* NewImage);

	FORCEINLINE UMaterialInstance* GetEmo() { return EmoMaterial; };

private:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnyWhere, Category = "Appearance")
	UMaterialInstance* EmoMaterial;

};
