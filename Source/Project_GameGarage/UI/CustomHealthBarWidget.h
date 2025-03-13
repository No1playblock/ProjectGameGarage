// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SHealthBar.h"
#include "CustomHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API UCustomHealthBarWidget : public UWidget
{
	GENERATED_BODY()

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
protected:

	float CurrentHealth = 1.0f;
	

	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<class SHealthBar> MyCustomHealthBar;

public:
	// 체력 업데이트 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float HealthPercentage);
	
};
