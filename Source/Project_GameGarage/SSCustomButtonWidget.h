// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SButton;
class SBorder;
class SImage;
class PROJECT_GAMEGARAGE_API SSCustomButtonWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSCustomButtonWidget)
	{}
		SLATE_ARGUMENT(FLinearColor, DefaultColor)
		SLATE_ARGUMENT(FLinearColor, HoverColor)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void SetClicked(bool bIsClicked);

protected:
	FReply OnButtonPressed();
	void OnHovered();
	void OnUnhovered();

protected:
	TSharedPtr<SButton> Button;
	TSharedPtr<SBorder> ClickedBorder;
	TSharedPtr<SImage> PreviewImage;

	FLinearColor DefaultColor;
	FLinearColor HoverColor;

private:
	FSlateBrush MySlateBrush;
};
