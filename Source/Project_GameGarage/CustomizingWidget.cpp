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
    // CustomButtonArray �ʱ�ȭ
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

    // �� ��ư�� Owner�� ����
    for (USkinButtonWidget* Button : SkinButtonArray)
    {
        if (Button)
        {
            Button->SetOwner(this);
        }
    }

    // CustomEmoButtonArray �ʱ�ȭ
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

    // �� ���� ��ư�� Owner�� ����
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
  

    // ����� ���̺� �����͸� ���Կ��� �ҷ���
    if (USaveFile* LoadedGame = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0)))
    {
        SaveGameObject = LoadedGame;

        // ����� Emo ��ư �ε����� ����Ͽ� �迭���� ��ư ��������
        int32 EmoButtonIndex = SaveGameObject->EmoButtonIndex;
        if (EmoButtonArray.IsValidIndex(EmoButtonIndex))
        {
            SelectedEmo = EmoButtonArray[EmoButtonIndex];
            SelectedEmo->SetClicked(true);
        }

        // ����� Skin ��ư �ε����� ����Ͽ� �迭���� ��ư ��������
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

    // ������ ���õ� ���� ��ư�� ��ȿ�ϴٸ� Ŭ�� ���� ����
    if (SelectedSkin)
    {
        SelectedSkin->SetClicked(false);
    }

    // ���ο� ��ư�� Ŭ�� ���·� �����ϰ�, SelectedEmo�� ����
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

    // ������ ���õ� ���� ��ư�� ��ȿ�ϴٸ� Ŭ�� ���� ����
    if (SelectedEmo)
    {
        SelectedEmo->SetClicked(false);
    }

    // ���ο� ��ư�� Ŭ�� ���·� �����ϰ�, SelectedEmo�� ����
    NewButton->SetClicked(true);
    SelectedEmo = NewButton;

    FindClickEmoButton();
}
void UCustomizingWidget::FindClickEmoButton()
{
    if (!SaveGameObject || !SelectedEmo)
    {
        return; // SaveGameObject �Ǵ� SelectedEmo�� nullptr�� ��� �Լ� ����
    }

    for (int32 Index = 0; Index < EmoButtonArray.Num(); ++Index)
    {
        // ���� �迭�� ��Ұ� SelectedEmo�� ������ �˻�
        if (EmoButtonArray[Index] == SelectedEmo)
        {
            // SaveGameObject�� �ε����� ����
            SaveGameObject->EmoButtonIndex = Index;
            break; // ��ġ�ϴ� ��ư�� ã�����Ƿ� ���� ����
        }
    }
}
void UCustomizingWidget::FindClickSkinButton()
{
    if (!SaveGameObject || !SelectedSkin)
    {
        return; // SaveGameObject �Ǵ� SelectedEmo�� nullptr�� ��� �Լ� ����
    }

    for (int32 Index = 0; Index < SkinButtonArray.Num(); ++Index)
    {
        // ���� �迭�� ��Ұ� SelectedEmo�� ������ �˻�
        if (SkinButtonArray[Index] == SelectedSkin)
        {
            // SaveGameObject�� �ε����� ����
            SaveGameObject->SkinButtonIndex = Index;
            break; // ��ġ�ϴ� ��ư�� ã�����Ƿ� ���� ����
        }
    }
}
void UCustomizingWidget::OnEmoBtnClicked()
{
    if (EmoGrid && SkinGrid)
    {
        // EmoGrid�� ���̵��� �����ϰ�, SkinGrid�� �����
        EmoGrid->SetVisibility(ESlateVisibility::Visible);
        SkinGrid->SetVisibility(ESlateVisibility::Hidden);
    }

    // Ŭ�� Ÿ���� EmoBtn���� ����
    SetClickType(EmoBtn);
}

void UCustomizingWidget::OnSkinBtnClicked()
{
    if (EmoGrid && SkinGrid)
    {
        // SkinGrid�� ���̵��� �����ϰ�, EmoGrid�� �����
        EmoGrid->SetVisibility(ESlateVisibility::Hidden);
        SkinGrid->SetVisibility(ESlateVisibility::Visible);
    }

    // Ŭ�� Ÿ���� SkinBtn���� ����
    SetClickType(SkinBtn);
}

void UCustomizingWidget::SetClickType(UButton* ClickBtn)
{
    if (SelectedTypeButton)
    {
        SelectedTypeButton->SetBackgroundColor(FLinearColor::White);
    }

    // ���� ���õ� ��ư�� ��� ������ ���� (��: ������� ����)
    ClickBtn->SetBackgroundColor(FLinearColor(0.436016f,1.0f, 0.768094, 1.0f));

    // ���� ���õ� ��ư�� SelectedTypeButton���� ����
    SelectedTypeButton = ClickBtn;
}

void UCustomizingWidget::OnSaveButtonClicked()
{

    // SaveGameObject ���� �Ǵ� �ε�
    /*if (!SaveGameObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateSaveGameObject"));
        SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
    }*/
    UE_LOG(LogTemp, Warning, TEXT("CreateSaveGameObject"));
    SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));

    if (SaveGameObject)
    {
        // SaveGameObject�� ���õ� ������ ����
       
        SaveGameObject->SaveSkin = SelectedSkin->GetSkin();
        SaveGameObject->SaveEmo = SelectedEmo->GetEmo();

        //Ȥ�� �𸣴� �ѹ� �� ȣ��
        FindClickEmoButton();
        FindClickSkinButton();

        // SaveGameObject�� ���Կ� ����
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
