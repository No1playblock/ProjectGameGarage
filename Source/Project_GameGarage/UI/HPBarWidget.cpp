// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"
#include "UI/CustomHealthBarWidget.h"

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}
void UHPBarWidget::SetHPBar(float Hp)
{
	NewHpBar->UpdateHealth(Hp / 100.0f);
}
