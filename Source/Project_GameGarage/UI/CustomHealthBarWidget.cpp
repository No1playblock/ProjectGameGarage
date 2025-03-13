// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomHealthBarWidget.h"
#include "UI/SHealthBar.h"

void UCustomHealthBarWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	//자식이 있으면 자식도 다 해제해주자
	MyCustomHealthBar.Reset();
}

TSharedRef<SWidget> UCustomHealthBarWidget::RebuildWidget()
{
	SAssignNew(MyCustomHealthBar, SHealthBar)
		.HealthPercentage(CurrentHealth);  // 초기 체력 값 설정

	return MyCustomHealthBar.ToSharedRef();
}

void UCustomHealthBarWidget::UpdateHealth(float HealthPercentage)
{
	CurrentHealth = FMath::Clamp(HealthPercentage, 0.0f, 1.0f);

	if (MyCustomHealthBar.IsValid())
	{
		MyCustomHealthBar->UpdateHealth(CurrentHealth);
	}
}
