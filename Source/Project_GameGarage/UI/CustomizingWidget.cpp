// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizingWidget.h"
#include "EmoButtonWidget.h"
#include "SkinButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "Components/GridPanel.h"
#include "Game/SaveFile.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SizeBox.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SWeakWidget.h"
#include "UI/CustomizePanel.h"
#include "Game/GGGameInstance.h"

void UCustomizingWidget::NativeConstruct()
{
	Super::NativeConstruct();

    SkinPanel->SetVisibility(ESlateVisibility::Visible);
    EmoPanel->SetVisibility(ESlateVisibility::Hidden);
    SelectedTypeButton = SkinBtn;
    SetClickType(SkinBtn);


	if (EmoBtn)
	{
		EmoBtn->OnClicked.AddDynamic(this, &UCustomizingWidget::OnEmoBtnClicked);
	}
	if (SkinBtn)
	{
		SkinBtn->OnClicked.AddDynamic(this, &UCustomizingWidget::OnSkinBtnClicked);
	}
    if (SaveBtn)
    {
        SaveBtn->OnClicked.AddDynamic(this, &UCustomizingWidget::OnSaveButtonClicked);
    }

}



void UCustomizingWidget::OnEmoBtnClicked()
{
    
    // EmoGrid를 보이도록 설정하고, SkinGrid는 숨기기
    EmoPanel->SetVisibility(ESlateVisibility::Visible);
    SkinPanel->SetVisibility(ESlateVisibility::Hidden);

    // 클릭 타입을 EmoBtn으로 설정
    SetClickType(EmoBtn);
}

void UCustomizingWidget::OnSkinBtnClicked()
{
    
    // SkinGrid를 보이도록 설정하고, EmoGrid는 숨기기
    SkinPanel->SetVisibility(ESlateVisibility::Visible);
    EmoPanel->SetVisibility(ESlateVisibility::Hidden);

    // 클릭 타입을 SkinBtn으로 설정
    SetClickType(SkinBtn);
}

void UCustomizingWidget::SetClickType(UButton* ClickBtn)
{
    if (SelectedTypeButton)
    {
        SelectedTypeButton->SetBackgroundColor(FLinearColor::White);
    }

    // 선택된 버튼의 배경 색상을 설정
    ClickBtn->SetBackgroundColor(FLinearColor(0.436016f,1.0f, 0.768094, 1.0f));

    // 현재 선택된 버튼을 SelectedTypeButton으로 설정
    SelectedTypeButton = ClickBtn;
}

void UCustomizingWidget::OnSaveButtonClicked()
{

    USaveFile* SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
    UGGGameInstance* GI = Cast<UGGGameInstance>(GetGameInstance());

    if (SaveGameObject)
    {
        GI->SavePlayerData(SkinPanel->GetSelectedButtonIndex(), EmoPanel->GetSelectedButtonIndex(),
            Cast<USkinButtonWidget>(SkinPanel->GetSelectedButton())->GetSkin(), Cast<UEmoButtonWidget>(EmoPanel->GetSelectedButton())->GetEmo());
    }
}
void UCustomizingWidget::InitSelectedButton(int32 EmoIndex, int32 SkinIndex)
{
    SkinPanel->SetSelectedButton(SkinIndex);
    EmoPanel->SetSelectedButton(EmoIndex);
}