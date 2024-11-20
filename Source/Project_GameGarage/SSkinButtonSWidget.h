// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSCustomButtonWidget.h"

/**
 * 
 */
class USkeletalMesh;

class PROJECT_GAMEGARAGE_API SSkinButtonSWidget : public SSCustomButtonWidget
{
public:
	SLATE_BEGIN_ARGS(SSkinButtonSWidget)
	{}
		SLATE_ARGUMENT(TSharedPtr<USkeletalMesh>, Skin)
		SLATE_ARGUMENT(TWeakObjectPtr<class APlayableCharacter>, OwnerCharacter)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void SetClicked(bool bIsClicked) override;

private:
	TSharedPtr<USkeletalMesh> Skin;
	TWeakObjectPtr<class APlayableCharacter> OwnerCharacter;

	FReply OnSkinButtonPressed();
};
