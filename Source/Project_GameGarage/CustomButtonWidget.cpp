// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter.h"
#include "CustomizingWidget.h"

//#include "WBP_CustomizeCharacterWidget.h"

void UCustomButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 이벤트 바인딩
    if (Button)
    {
        Button->OnPressed.AddDynamic(this, &UCustomButtonWidget::OnButtonPressed);
        Button->OnReleased.AddDynamic(this, &UCustomButtonWidget::OnButtonReleased);
        Button->OnHovered.AddDynamic(this, &UCustomButtonWidget::OnButtonHovered);
        Button->OnUnhovered.AddDynamic(this, &UCustomButtonWidget::OnButtonUnhovered);
    }
}

void UCustomButtonWidget::OnButtonPressed()
{
    SetCharacterImageColorAndOpacity(FLinearColor(0.144792f, 0.144792f, 0.144792f, 1.0f)); // 예시 값으로 설정
}

void UCustomButtonWidget::OnButtonReleased()
{
    SetCharacterImageColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // 예시 값으로 설정
}

void UCustomButtonWidget::OnButtonHovered()
{
    SetCharacterImageColorAndOpacity(FLinearColor(0.5, 0.5, 0.5, 1.0f)); // 예시 값으로 설정
}

void UCustomButtonWidget::OnButtonUnhovered()
{
    SetCharacterImageColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // 예시 값으로 설정
}

void UCustomButtonWidget::OnButtonClicked()
{
}



void UCustomButtonWidget::SetClicked(bool bIsClicked)
{

}

void UCustomButtonWidget::SetOwner(UCustomizingWidget* NewOwner)
{
    Owner = NewOwner;
}


void UCustomButtonWidget::SetCharacterImageColorAndOpacity(FLinearColor NewColor)
{
    if (PreviewImage)
    {
        PreviewImage->SetColorAndOpacity(NewColor);
    }
}