// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class PROJECT_GAMEGARAGE_API SHealthBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHealthBar)
	{}
	SLATE_ATTRIBUTE(float, HealthPercentage)  // ü�� ���� (0.0 ~ 1.0)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

    void UpdateHealth(float NewHealthPercentage);

private:
    // ü�� ����
    float CurrentHealth;
    float TargetHealth;

    // Ÿ�Ӷ����� ����� �ε巴�� ����
    float InterpolationSpeed = 5.0f;

    // ������ ����
    bool bIsCritical = false;
    float BlinkTimer = 0.0f;

    // ���� ���
    FLinearColor GetHealthColor() const;

    //�ε巴�� �پ���
    EActiveTimerReturnType HandleActiveTimer(double InCurrentTime, float InDeltaTime);

    // ���� ������Ʈ
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};
