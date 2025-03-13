// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HpBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API UHpBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
