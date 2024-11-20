// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"
#include "Components/ProgressBar.h"
void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UHPBarWidget::SetHPBar(float hp)
{
	float HP = hp / 100.0f;
	UE_LOG(LogTemp, Warning, TEXT("hp: %f"), HP);
	HPBar->SetPercent(HP);

}
