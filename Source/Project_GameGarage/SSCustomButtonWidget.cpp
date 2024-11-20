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
                .BorderBackgroundColor(DefaultColor) // �⺻ ����
                [
                    SNew(SButton)
                        .OnClicked(this, &SSCustomButtonWidget::OnButtonPressed)
                        .OnHovered(FSimpleDelegate::CreateSP(this, &SSCustomButtonWidget::OnHovered))
                        .OnUnhovered(FSimpleDelegate::CreateSP(this, &SSCustomButtonWidget::OnUnhovered))
                        [
                            SNew(SImage)
                                .Image(&MySlateBrush) // ��     
                        ]
                ]
        ];

    ClickedBorder = StaticCastSharedRef<SBorder>(ChildSlot.GetWidget());
    Button = StaticCastSharedRef<SButton>(ClickedBorder->GetContent());
    PreviewImage = StaticCastSharedRef<SImage>(Button->GetContent());
}
FReply SSCustomButtonWidget::OnButtonPressed()
{
    // �⺻ ��ư Ŭ�� ���� (�ʿ� �� �������̵� ����)
    return FReply::Handled();
}

void SSCustomButtonWidget::OnHovered()
{
    if (ClickedBorder.IsValid())
    {
        ClickedBorder->SetBorderBackgroundColor(HoverColor); // ȣ�� �� ���� ����
    }
}

void SSCustomButtonWidget::OnUnhovered()
{
    if (ClickedBorder.IsValid())
    {
        ClickedBorder->SetBorderBackgroundColor(DefaultColor); // ��ȣ�� �� ����
    }
}

void SSCustomButtonWidget::SetClicked(bool bIsClicked)
{
    if (ClickedBorder.IsValid())
    {
        FLinearColor NewColor = bIsClicked ? FLinearColor::Green : DefaultColor;
        ClickedBorder->SetBorderBackgroundColor(NewColor); // Ŭ�� ���� �ݿ�
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
