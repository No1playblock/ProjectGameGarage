// Fill out your copyright notice in the Description page of Project Settings.


#include "EmoButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "CustomizingWidget.h"
#include "UI/CustomizePanel.h"

void UEmoButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();     
}

void UEmoButtonWidget::SetClicked(bool bIsClicked)
{

    // 클릭 상태에 따른 색상 변경
    FLinearColor Color = bIsClicked ? FLinearColor(0.0f, 0.973157f, 1.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    if (ClickedBorder)
    {
        ClickedBorder->SetBrushColor(Color);
    }

    // 캐릭터의 메쉬 설정
    if (bIsClicked)
    {
        ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        Cast<APlayableCharacter>(Character)->SetEmoMaterial(EmoMaterial);
    }
}

void UEmoButtonWidget::SetEmoBtnData(UMaterialInstance* NewMaterial, UTexture2D* NewImage)
{
    EmoMaterial = NewMaterial;
    SetPreviewTexture(NewImage);
}
