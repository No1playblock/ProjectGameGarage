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
	SLATE_ATTRIBUTE(float, HealthPercentage)  // 체력 비율 (0.0 ~ 1.0)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

    void UpdateHealth(float NewHealthPercentage);

private:
    // 체력 비율
    float CurrentHealth;
    float TargetHealth;

    // 타임라인을 사용해 부드럽게 보간
    float InterpolationSpeed = 5.0f;

    // 깜빡임 상태
    bool bIsCritical = false;
    float BlinkTimer = 0.0f;

    // 색상 계산
    FLinearColor GetHealthColor() const;

    //부드럽게 줄어들기
    EActiveTimerReturnType HandleActiveTimer(double InCurrentTime, float InDeltaTime);

    // 위젯 업데이트
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};
