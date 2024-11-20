// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

void UCrosshairWidget::ChangeZoomState(bool IsZoom)
{
	bIsZoom = IsZoom;
	DrawHUD();
}
