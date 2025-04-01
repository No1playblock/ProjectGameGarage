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
	
public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	void SetHPBar(float Hp);

private:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnyWhere, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCustomHealthBarWidget> NewHpBar;

	float MaxHp;

};
