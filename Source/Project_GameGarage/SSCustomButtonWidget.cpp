// Fill out your copyright notice in the Description page of Project Settings.


#include "SSCustomButtonWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCustomButtonWidget::Construct(const FArguments& InArgs)
{
    DefaultColor = InArgs._DefaultColor;
    HoverColor = InArgs._HoverColor;

    ChildSlot
        [
            SNew(SBorder)
                .BorderBackgroundColor(DefaultColor) // 기본 색상
                [
                    SNew(SButton)
                        .OnClicked(this, &SSCustomButtonWidget::OnButtonPressed)
                        .OnHovered(FSimpleDelegate::CreateSP(this, &SSCustomButtonWidget::OnHovered))
                        .OnUnhovered(FSimpleDelegate::CreateSP(this, &SSCustomButtonWidget::OnUnhovered))
                        [
                            SNew(SImage)
                                .Image(&MySlateBrush) // 빈값     
                        ]
                ]
        ];

    ClickedBorder = StaticCastSharedRef<SBorder>(ChildSlot.GetWidget());
    Button = StaticCastSharedRef<SButton>(ClickedBorder->GetContent());
    PreviewImage = StaticCastSharedRef<SImage>(Button->GetContent());
}
FReply SSCustomButtonWidget::OnButtonPressed()
{
    // 기본 버튼 클릭 동작 (필요 시 오버라이드 가능)
    return FReply::Handled();
}

void SSCustomButtonWidget::OnHovered()
{
    if (ClickedBorder.IsValid())
    {
        ClickedBorder->SetBorderBackgroundColor(HoverColor); // 호버 시 색상 변경
    }
}

void SSCustomButtonWidget::OnUnhovered()
{
    if (ClickedBorder.IsValid())
    {
        ClickedBorder->SetBorderBackgroundColor(DefaultColor); // 비호버 시 복구
    }
}

void SSCustomButtonWidget::SetClicked(bool bIsClicked)
{
    if (ClickedBorder.IsValid())
    {
        FLinearColor NewColor = bIsClicked ? FLinearColor::Green : DefaultColor;
        ClickedBorder->SetBorderBackgroundColor(NewColor); // 클릭 상태 반영
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
