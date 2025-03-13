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

    // Critical ���� Ȯ�� (20% ����)
    bIsCritical = TargetHealth <= 0.2f;

    RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SHealthBar::HandleActiveTimer));

}

int32 SHealthBar::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const FVector2D Size = AllottedGeometry.GetLocalSize();


    const float BorderPadding = 1.0f;  // �����¿�� 1px Ȯ�� (�� 2px)

    FVector2D ExpandedSize = Size + FVector2D(BorderPadding * 2, BorderPadding * 2);
    FVector2D Offset = FVector2D(-BorderPadding, -BorderPadding);  // �߾� ���� 

    //���� �׵θ�
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(ExpandedSize, FSlateLayoutTransform(Offset)),
        FCoreStyle::Get().GetBrush("WhiteBrush"),
        ESlateDrawEffect::None,
        FLinearColor::Black  // ������ ���
    );

    //ȸ�� ���
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,  // ���� LayerId�� ��� (���� �׷���)
        AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform()),
        FCoreStyle::Get().GetBrush("WhiteBrush"),
        ESlateDrawEffect::None,
        FLinearColor::Gray
    );


    // ����
    const float HealthBarWidth = Size.X * CurrentHealth;

    // ������
    float Opacity = bIsCritical ? (0.5f + 0.5f * FMath::Sin(BlinkTimer)) : 1.0f;

    // ��
    FLinearColor HealthColor = GetHealthColor();
    HealthColor.A = Opacity;

    // ü�� �� �׸���
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
    // ü�� ������ ���� ���� ���� (�ʷ� �� ��� �� ����)
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
    // �ε巯�� ���� (Smooth Interpolation)
    CurrentHealth = FMath::FInterpTo(CurrentHealth, TargetHealth, InDeltaTime, InterpolationSpeed);

    // ������ Ÿ�̸� (Critical ������ ��)
    if (bIsCritical)
    {
        BlinkTimer += InDeltaTime * 10.0f;  // ������ �ӵ�
    }
    else
    {
        BlinkTimer = 0.0f;
    }

    // ȭ�� ����
    Invalidate(EInvalidateWidget::Paint);

    // TargetHealth�� �����ϰų� Critical �ƴϸ� Ÿ�̸� ����
    bool bIsAtTarget = FMath::IsNearlyEqual(CurrentHealth, TargetHealth, 0.001f);
    if (bIsAtTarget && !bIsCritical)
    {
        return EActiveTimerReturnType::Stop;
    }

    return EActiveTimerReturnType::Continue;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
