// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
void UCrosshairWidget::ChangeZoomState(bool IsZoom)
{
	bIsZoom = IsZoom;
	
	if (IsZoom)
	{
		PistolCrosshair->SetRenderOpacity(1.0f);
	}
	else
	{
		PistolCrosshair->SetRenderOpacity(0.0f);
	}

}

void UCrosshairWidget::SetTimerText(FText text)
{
	timerText->SetText(text);
}
