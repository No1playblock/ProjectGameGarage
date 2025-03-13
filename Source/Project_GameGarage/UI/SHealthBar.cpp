// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SHealthBar.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHealthBar::Construct(const FArguments& InArgs)
{
	CurrentHealth = TargetHealth = InArgs._HealthPercentage.Get();
	
    RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SHealthBar::HandleActiveTimer));
}

void SHealthBar::UpdateHealth(float NewHealthPercentage)
{
    TargetHealth = FMath::Clamp(NewHealthPercentage, 0.0f, 1.0f);

    // Critical 상태 확인 (20% 이하)
    bIsCritical = TargetHealth <= 0.2f;

    RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SHealthBar::HandleActiveTimer));

}

int32 SHealthBar::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const FVector2D Size = AllottedGeometry.GetLocalSize();


    const float BorderPadding = 1.0f;  // 상하좌우로 1px 확장 (총 2px)

    FVector2D ExpandedSize = Size + FVector2D(BorderPadding * 2, BorderPadding * 2);
    FVector2D Offset = FVector2D(-BorderPadding, -BorderPadding);  // 중앙 정렬 

    //검은 테두리
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(ExpandedSize, FSlateLayoutTransform(Offset)),
        FCoreStyle::Get().GetBrush("WhiteBrush"),
        ESlateDrawEffect::None,
        FLinearColor::Black  // 검은색 배경
    );

    //회색 배경
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,  // 같은 LayerId를 사용 (먼저 그려짐)
        AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform()),
        FCoreStyle::Get().GetBrush("WhiteBrush"),
        ESlateDrawEffect::None,
        FLinearColor::Gray
    );


    // 길이
    const float HealthBarWidth = Size.X * CurrentHealth;

    // 깜빡임
    float Opacity = bIsCritical ? (0.5f + 0.5f * FMath::Sin(BlinkTimer)) : 1.0f;

    // 색
    FLinearColor HealthColor = GetHealthColor();
    HealthColor.A = Opacity;

    // 체력 바 그리기
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(FVector2D(HealthBarWidth, Size.Y), FSlateLayoutTransform()),
        FCoreStyle::Get().GetBrush("WhiteBrush"),
        ESlateDrawEffect::None,
        HealthColor
    );
    
    return LayerId + 1;
}

FLinearColor SHealthBar::GetHealthColor() const
{
    // 체력 비율에 따라 색상 변경 (초록 → 노랑 → 빨강)
    if (CurrentHealth > 0.7f)
    {
        return FLinearColor::Green;
    }
    else if (CurrentHealth > 0.3f)
    {
        return FLinearColor::Yellow;
    }
    else
    {
        return FLinearColor::Red;
    }

}
EActiveTimerReturnType SHealthBar::HandleActiveTimer(double InCurrentTime, float InDeltaTime)
{
    // 부드러운 보간 (Smooth Interpolation)
    CurrentHealth = FMath::FInterpTo(CurrentHealth, TargetHealth, InDeltaTime, InterpolationSpeed);

    // 깜빡임 타이머 (Critical 상태일 때)
    if (bIsCritical)
    {
        BlinkTimer += InDeltaTime * 10.0f;  // 깜빡임 속도
    }
    else
    {
        BlinkTimer = 0.0f;
    }

    // 화면 갱신
    Invalidate(EInvalidateWidget::Paint);

    // TargetHealth에 도달하거나 Critical 아니면 타이머 중지
    bool bIsAtTarget = FMath::IsNearlyEqual(CurrentHealth, TargetHealth, 0.001f);
    if (bIsAtTarget && !bIsCritical)
    {
        return EActiveTimerReturnType::Stop;
    }

    return EActiveTimerReturnType::Continue;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
