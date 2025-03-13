// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ChangeZoomState(bool IsZoom);

	UFUNCTION()
	void SetTimerText(FText text);

	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	bool bIsZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UTextBlock* timerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UImage> PistolCrosshair;
	
};
