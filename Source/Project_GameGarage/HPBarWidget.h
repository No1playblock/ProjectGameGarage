// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HPBar;

	UFUNCTION(BlueprintCallable)
	void SetHPBar(float hp);
	
};
