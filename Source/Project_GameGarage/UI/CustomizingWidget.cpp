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

    
    InitializeButtonArrays();
   // LoadSaveGame();
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
void UCustomizingWidget::InitializeButtonArrays()
{
    
}
void UCustomizingWidget::LoadSaveGame()
{
  

    // ����� ���̺� �����͸� ���Կ��� �ҷ���
    //if (USaveFile* LoadedGame = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0)))
    //{
    //    SaveGameObject = LoadedGame;

    //    // ����� Emo ��ư �ε����� ����Ͽ� �迭���� ��ư ��������
    //    int32 EmoButtonIndex = SaveGameObject->EmoButtonIndex;
    //    if (EmoButtonArray.IsValidIndex(EmoButtonIndex))
    //    {
    //        SelectedEmo = EmoButtonArray[EmoButtonIndex];
    //        SelectedEmo->SetClicked(true);
    //    }

    //    // ����� Skin ��ư �ε����� ����Ͽ� �迭���� ��ư ��������
    //    int32 SkinButtonIndex = SaveGameObject->SkinButtonIndex;
    //    if (SkinButtonArray.IsValidIndex(SkinButtonIndex))
    //    {
    //        SelectedSkin = SkinButtonArray[SkinButtonIndex];
    //        SelectedSkin->SetClicked(true);
    //    }
    //}
    //else
    //{
    //    SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
    //    SelectedEmo = EmoButtonArray[0];
    //    SetEmotionButton(SelectedEmo);

    //    SelectedSkin = SkinButtonArray[0];
    //    SetSkinButton(SelectedSkin);
    //}
    //UE_LOG(LogTemp, Warning, TEXT("LoadFinish"));
    //
}


void UCustomizingWidget::OnEmoBtnClicked()
{
    
    // EmoGrid�� ���̵��� �����ϰ�, SkinGrid�� �����
    EmoPanel->SetVisibility(ESlateVisibility::Visible);
    SkinPanel->SetVisibility(ESlateVisibility::Hidden);

    // Ŭ�� Ÿ���� EmoBtn���� ����
    SetClickType(EmoBtn);
}

void UCustomizingWidget::OnSkinBtnClicked()
{
    
    // SkinGrid�� ���̵��� �����ϰ�, EmoGrid�� �����
    SkinPanel->SetVisibility(ESlateVisibility::Visible);
    EmoPanel->SetVisibility(ESlateVisibility::Hidden);

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
 

    USaveFile* SaveGameObject = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
    UGGGameInstance* GI = Cast<UGGGameInstance>(GetGameInstance());

    if (SaveGameObject)
    {
        GI->SavePlayerData(SkinPanel->GetSelectedButtonIndex(), EmoPanel->GetSelectedButtonIndex(),
            Cast<USkinButtonWidget>(SkinPanel->GetSelectedButton())->GetSkin(), Cast<UEmoButtonWidget>(EmoPanel->GetSelectedButton())->GetEmo());
        // SaveGameObject�� ���õ� ������ ����
        
        
        
        //SaveGameObject->SaveEmo = Cast<UEmoButtonWidget>(EmoPanel->GetSelectedButton())->GetEmo();
        //SaveGameObject->SaveSkin = Cast<USkinButtonWidget>(SkinPanel->GetSelectedButton())->GetSkin();
        ////���� ���常��
        //SaveGameObject->SkinButtonIndex = SkinPanel->GetSelectedButtonIndex();
        //SaveGameObject->EmoButtonIndex = EmoPanel->GetSelectedButtonIndex();

 

        // SaveGameObject�� ���Կ� ����
        //UE_LOG(LogTemp, Warning, TEXT("Save"));
       // bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("PlayerSaveSlot"), 0);
        UE_LOG(LogTemp, Warning, TEXT("UserIndex_Save: %d "), userIndex);
        /*if (bSaveSuccess)
        {
            UE_LOG(LogTemp, Warning, TEXT("Game saved successfully"));
            UE_LOG(LogTemp, Warning, TEXT("SaveEmo: %s, SaveSkin: %s"), *SaveGameObject->SaveEmo->GetFName().ToString(), *SaveGameObject->SaveSkin->GetFName().ToString());
            UE_LOG(LogTemp, Warning, TEXT("SaveEmoIndex: %d, SaveSkinIndex: %d"), SaveGameObject->EmoButtonIndex, SaveGameObject->SkinButtonIndex);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save game"));
        }*/

    }
}
void UCustomizingWidget::InitSelectedButton(int32 EmoIndex, int32 SkinIndex)
{
    SkinPanel->SetSelectedButton(SkinIndex);
    EmoPanel->SetSelectedButton(EmoIndex);
}