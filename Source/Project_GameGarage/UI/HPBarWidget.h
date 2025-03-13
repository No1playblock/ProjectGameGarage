// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GGUserWidget.h"
#include "HPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GAMEGARAGE_API UHPBarWidget : public UGGUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;


public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	UFUNCTION(BlueprintCallable)
	void SetHPBar(float hp);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UCustomHealthBarWidget> NewHPBar;
	float MaxHp;

	
};
