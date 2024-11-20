// Fill out your copyright notice in the Description page of Project Settings.


#include "SSkinButtonSWidget.h"
#include "SlateOptMacros.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter.h"


void SSkinButtonSWidget::Construct(const FArguments& InArgs)
{
    Skin = InArgs._Skin;
    OwnerCharacter = InArgs._OwnerCharacter;

    // 부모 클래스의 Construct 호출
    SSCustomButtonWidget::Construct(
        SSCustomButtonWidget::FArguments()
        .DefaultColor(FLinearColor::White)
        .HoverColor(FLinearColor::Gray)
    );

    // 클릭 이벤트를 스킨 버튼 전용 동작으로 설정
   // Button->SetOnClicked(FSimpleDelegate::CreateSP(this, &SSkinButtonSWidget::OnSkinButtonPressed));
}
FReply SSkinButtonSWidget::OnSkinButtonPressed()
{
    if (OwnerCharacter.IsValid())
    {
        OwnerCharacter->SetSkin(Skin.Get()); // 캐릭터에 스킨 적용
    }
    return FReply::Handled();
}

void SSkinButtonSWidget::SetClicked(bool bIsClicked)
{
    // 부모 클래스 동작 호출
    SSCustomButtonWidget::SetClicked(bIsClicked);

    if (bIsClicked && OwnerCharacter.IsValid())
    {
        OwnerCharacter->SetSkin(Skin.Get()); // 선택된 스킨 적용
    }
}
