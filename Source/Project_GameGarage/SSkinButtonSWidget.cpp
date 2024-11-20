// Fill out your copyright notice in the Description page of Project Settings.


#include "SSkinButtonSWidget.h"
#include "SlateOptMacros.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter.h"


void SSkinButtonSWidget::Construct(const FArguments& InArgs)
{
    Skin = InArgs._Skin;
    OwnerCharacter = InArgs._OwnerCharacter;

    // �θ� Ŭ������ Construct ȣ��
    SSCustomButtonWidget::Construct(
        SSCustomButtonWidget::FArguments()
        .DefaultColor(FLinearColor::White)
        .HoverColor(FLinearColor::Gray)
    );

    // Ŭ�� �̺�Ʈ�� ��Ų ��ư ���� �������� ����
   // Button->SetOnClicked(FSimpleDelegate::CreateSP(this, &SSkinButtonSWidget::OnSkinButtonPressed));
}
FReply SSkinButtonSWidget::OnSkinButtonPressed()
{
    if (OwnerCharacter.IsValid())
    {
        OwnerCharacter->SetSkin(Skin.Get()); // ĳ���Ϳ� ��Ų ����
    }
    return FReply::Handled();
}

void SSkinButtonSWidget::SetClicked(bool bIsClicked)
{
    // �θ� Ŭ���� ���� ȣ��
    SSCustomButtonWidget::SetClicked(bIsClicked);

    if (bIsClicked && OwnerCharacter.IsValid())
    {
        OwnerCharacter->SetSkin(Skin.Get()); // ���õ� ��Ų ����
    }
}
