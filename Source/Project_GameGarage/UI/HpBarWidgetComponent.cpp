// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarWidgetComponent.h"
#include "GGUserWidget.h"

void UHpBarWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UGGUserWidget* thisUserWidget = Cast<UGGUserWidget>(GetWidget());
	if (thisUserWidget)
	{
		thisUserWidget->SetOwningActor(GetOwner());
	}
}
