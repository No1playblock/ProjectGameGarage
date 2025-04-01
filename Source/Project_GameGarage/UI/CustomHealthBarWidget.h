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

public:
	// 체력 업데이트 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float HealthPercentage);
	
private:

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual TSharedRef<SWidget> RebuildWidget() override;


	float CurrentHealth = 1.0f;
	
	TSharedPtr<class SHealthBar> MyCustomHealthBar;
};
