// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomButtonWidget.h"
#include "SkinButtonWidget.generated.h"

/**
 * 
 */
class UCustomizingWidget;
class USkeletalMesh;
UCLASS()
class PROJECT_GAMEGARAGE_API USkinButtonWidget : public UCustomButtonWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* Skin;

	virtual void NativeConstruct() override;
public:
	
	virtual void OnButtonClicked() override;

	virtual void SetClicked(bool bIsClicked) override;

	USkeletalMesh* GetSkin();
};
