// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizingWidget.h"
#include "EmoButtonWidget.h"
#include "SkinButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "SaveFile.h"

void UCustomizingWidget::NativeConstruct()
{
	//Super::NativeConstruct();

    
    InitializeButtonArrays();
    LoadSaveGame();
    SkinGrid->SetVisibility(ESlateVisibility::Visible);
    EmoGrid->SetVisibility(ESlateVisibility::Hidden);
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
void UCustomizingWidget::InitializeButtonArrays()
{
    // CustomButtonArray 초기화
    SkinButtonArray = {
        SkinButton_0,
        SkinButton_1,
        SkinButton_2,
        SkinButton_3,
        SkinButton_4,
        SkinButton_5,
        SkinButton_6,
        SkinButton_7,
        SkinButton_8
    };

    // 각 버튼의 Owner를 설정
    for (USkinButtonWidget* Button : SkinButtonArray)
    {
        if (Button)
        {
            Button->SetOwner(this);
        }
    }

    // CustomEmoButtonArray 초기화
    EmoButtonArray = {
        EmotionButton_0,
        EmotionButton_1,
        EmotionButton_2,
        EmotionButton_3,
        EmotionButton_4,
        EmotionButton_5,
        EmotionButton_6,
        EmotionButton_7,
        EmotionButton_8,
        EmotionButton_9,
        EmotionButton_10,
        EmotionButton_11,
        EmotionButton_12
    };

    // 각 감정 버튼의 Owner를 설정
    for (UEmoButtonWidget* EmotionButton : EmoButtonArray)
    {
        if (EmotionButton)
        {
            EmotionButton->SetOwner(this);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("ArrayFinish"));

}
void UCustomizingWidget::LoadSaveGame()
{
  

    // 저장된 세이브 데이터를 슬롯에서 불러옴
    if (USaveFile* LoadedGame = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0)))
    {
        SaveGameObject = LoadedGame;

        // 저장된 Emo 버튼 인덱스를 사용하여 배열에서 버튼 가져오기
        int32 EmoButtonIndex = SaveGameObject->EmoButtonIndex;
        if (EmoButtonArray.IsValidIndex(EmoButtonIndex))
        {
            SelectedEmo = EmoButtonArray[EmoButtonIndex];
            SelectedEmo->SetClicked(true);
        }

        // 저장된 Skin 버튼 인덱스를 사용하여 배열에서 버튼 가져오기
        int32 SkinButtonIndex = SaveGameObject->SkinButtonIndex;
        if (SkinButtonArray.IsValidIndex(SkinButtonIndex))
        {
            SelectedSkin = SkinButtonArray[SkinButtonIndex];
            SelectedSkin->SetClicked(true);
        }
    }
    else
    {
        SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
        SelectedEmo = EmoButtonArray[0];
        SetEmotionButton(SelectedEmo);

        SelectedSkin = SkinButtonArray[0];
        SetSkinButton(SelectedSkin);
    }
    UE_LOG(LogTemp, Warning, TEXT("LoadFinish"));
    
}
void UCustomizingWidget::SetSkinButton(USkinButtonWidget* NewButton)
{
    if (!NewButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("NULLPOINT"));
        return;
    }

    // 이전에 선택된 감정 버튼이 유효하다면 클릭 상태 해제
    if (SelectedSkin)
    {
        SelectedSkin->SetClicked(false);
    }

    // 새로운 버튼을 클릭 상태로 설정하고, SelectedEmo로 저장
    NewButton->SetClicked(true);
    SelectedSkin = NewButton;

    FindClickSkinButton();
}
void UCustomizingWidget::SetEmotionButton(UEmoButtonWidget* NewButton)
{
    if (!NewButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("NULLPOINT"));
        return;
    }

    // 이전에 선택된 감정 버튼이 유효하다면 클릭 상태 해제
    if (SelectedEmo)
    {
        SelectedEmo->SetClicked(false);
    }

    // 새로운 버튼을 클릭 상태로 설정하고, SelectedEmo로 저장
    NewButton->SetClicked(true);
    SelectedEmo = NewButton;

    FindClickEmoButton();
}
void UCustomizingWidget::FindClickEmoButton()
{
    if (!SaveGameObject || !SelectedEmo)
    {
        return; // SaveGameObject 또는 SelectedEmo가 nullptr인 경우 함수 종료
    }

    for (int32 Index = 0; Index < EmoButtonArray.Num(); ++Index)
    {
        // 현재 배열의 요소가 SelectedEmo와 같은지 검사
        if (EmoButtonArray[Index] == SelectedEmo)
        {
            // SaveGameObject에 인덱스를 저장
            SaveGameObject->EmoButtonIndex = Index;
            break; // 일치하는 버튼을 찾았으므로 루프 종료
        }
    }
}
void UCustomizingWidget::FindClickSkinButton()
{
    if (!SaveGameObject || !SelectedSkin)
    {
        return; // SaveGameObject 또는 SelectedEmo가 nullptr인 경우 함수 종료
    }

    for (int32 Index = 0; Index < SkinButtonArray.Num(); ++Index)
    {
        // 현재 배열의 요소가 SelectedEmo와 같은지 검사
        if (SkinButtonArray[Index] == SelectedSkin)
        {
            // SaveGameObject에 인덱스를 저장
            SaveGameObject->SkinButtonIndex = Index;
            break; // 일치하는 버튼을 찾았으므로 루프 종료
        }
    }
}
void UCustomizingWidget::OnEmoBtnClicked()
{
    if (EmoGrid && SkinGrid)
    {
        // EmoGrid를 보이도록 설정하고, SkinGrid는 숨기기
        EmoGrid->SetVisibility(ESlateVisibility::Visible);
        SkinGrid->SetVisibility(ESlateVisibility::Hidden);
    }

    // 클릭 타입을 EmoBtn으로 설정
    SetClickType(EmoBtn);
}

void UCustomizingWidget::OnSkinBtnClicked()
{
    if (EmoGrid && SkinGrid)
    {
        // SkinGrid를 보이도록 설정하고, EmoGrid는 숨기기
        EmoGrid->SetVisibility(ESlateVisibility::Hidden);
        SkinGrid->SetVisibility(ESlateVisibility::Visible);
    }

    // 클릭 타입을 SkinBtn으로 설정
    SetClickType(SkinBtn);
}

void UCustomizingWidget::SetClickType(UButton* ClickBtn)
{
    if (SelectedTypeButton)
    {
        SelectedTypeButton->SetBackgroundColor(FLinearColor::White);
    }

    // 현재 선택된 버튼의 배경 색상을 설정 (예: 녹색으로 설정)
    ClickBtn->SetBackgroundColor(FLinearColor(0.436016f,1.0f, 0.768094, 1.0f));

    // 현재 선택된 버튼을 SelectedTypeButton으로 설정
    SelectedTypeButton = ClickBtn;
}

void UCustomizingWidget::OnSaveButtonClicked()
{

    // SaveGameObject 생성 또는 로드
    /*if (!SaveGameObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateSaveGameObject"));
        SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
    }*/
    UE_LOG(LogTemp, Warning, TEXT("CreateSaveGameObject"));
    SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));

    if (SaveGameObject)
    {
        // SaveGameObject에 선택된 데이터 설정
       
        SaveGameObject->SaveSkin = SelectedSkin->GetSkin();
        SaveGameObject->SaveEmo = SelectedEmo->GetEmo();

        //혹시 모르니 한번 더 호출
        FindClickEmoButton();
        FindClickSkinButton();

        // SaveGameObject를 슬롯에 저장
        //UE_LOG(LogTemp, Warning, TEXT("Save"));
        bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("PlayerSaveSlot"), 0);
        UE_LOG(LogTemp, Warning, TEXT("UserIndex_Save: %d "), userIndex);
        if (bSaveSuccess)
        {
            UE_LOG(LogTemp, Warning, TEXT("Game saved successfully"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save game"));
        }
    }
}
