// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomizePanel.h"
#include "SkinButtonWidget.h"
#include "EmoButtonWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameData/CustomizeData.h"
#include "Engine/DataTable.h"
#include "Components/GridPanel.h"



void UCustomizePanel::NativeConstruct()
{
	CustomizeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Assets/DataTable/CharacterCusomtize.CharacterCusomtize"));

	int32 num;
	switch (CustomizeType)
	{
	case ECustomizeUIType::Skin:
		num = GetColumnCount(FName(TEXT("SkinImage")));
		CreateCustomizeSkinPanel(num);
		break;
	case ECustomizeUIType::Emo:
		num = GetColumnCount(FName(TEXT("EmoImage")));
		CreateCustomizeEmoPanel(num);
		break;
	}

}

void UCustomizePanel::ButtonClicked(UCustomButtonWidget* NewButton)
{
	if (!NewButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Button NULL"));
		return;
	}

	SelectedButtonIndex = Buttons.Find(NewButton);
	// 이전에 선택된 스킨 버튼이 유효하다면 클릭 상태 해제
	if (SelectedButton)
	{
		SelectedButton->SetClicked(false);
	}		

	// 새로운 버튼을 클릭 상태로 설정하고, SelectedSkin로 저장
	NewButton->SetClicked(true);
	SelectedButton = NewButton;

}

int32 UCustomizePanel::GetColumnCount(FName ColumnName)
{

	if (!CustomizeDataTable) return 0;

	int32 Count = 0;
	const UScriptStruct* RowStruct = CustomizeDataTable->GetRowStruct();

	for (const auto& RowPair : CustomizeDataTable->GetRowMap())
	{
		uint8* RowData = (uint8*)RowPair.Value;
		FProperty* Property = RowStruct->FindPropertyByName(ColumnName);

		if (Property)
		{
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(RowData);
			if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Property))
			{
				UObject* ObjValue = ObjProperty->GetObjectPropertyValue(ValuePtr);
				if (ObjValue)
				{
					Count++;
				}
			}
		}
	}

	return Count;

}
void UCustomizePanel::CreateCustomizeSkinPanel(int32 ColumnNum)
{
	const int32 NumButtons = ColumnNum;

	for (int32 i = 0; i < NumButtons; i++)
	{
		FCustomizeData* Row = CustomizeDataTable->FindRow<FCustomizeData>(FName(*FString::FromInt(i + 1)), TEXT(""));
		if (!Row) continue;

		// Mesh와 SkinImage 로드
		USkeletalMesh* LoadedMesh = Row->MeshName.Get();
		UTexture2D* LoadedTexture = Row->SkinImage.Get();

		if (!LoadedMesh || !LoadedTexture)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load assets skin index : %d"), i);
			continue;
		}
		// UUserWidget 생성
		USkinButtonWidget* NewButton = CreateWidget<USkinButtonWidget>(GetWorld(), CustomButtonWidget);
		if (!NewButton) continue;

		NewButton->SetSkinBtnData(LoadedMesh, LoadedTexture);

		GridPanel->AddChildToGrid(NewButton, i / 4, i % 4);
		
		Buttons.Add(NewButton);
		NewButton->SetOwner(this);
	}
}
void UCustomizePanel::CreateCustomizeEmoPanel(int32 ColumnNum)
{

	const int32 NumButtons = ColumnNum;

	for (int32 i = 0; i < NumButtons; i++)
	{

		FCustomizeData* Row = CustomizeDataTable->FindRow<FCustomizeData>(FName(*FString::FromInt(i + 1)), TEXT(""));
		if (!Row) continue;

		// `Mesh`와 `emoImage` 로드
		UMaterialInstance* LoadedMaterial = Row->EmoMaterial.Get();
		UTexture2D* LoadedTexture = Row->EmoImage.Get();

		if (!LoadedMaterial || !LoadedTexture)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load assets for emo index : %d"), i);
			continue;
		}


		// UUserWidget 생성
		UEmoButtonWidget* NewButton = CreateWidget<UEmoButtonWidget>(GetWorld(), CustomButtonWidget);
		if (!NewButton) continue;

		NewButton->SetEmoBtnData(LoadedMaterial, LoadedTexture);

		GridPanel->AddChildToGrid(NewButton, i / 4, i % 4);

		Buttons.Add(NewButton);
		NewButton->SetOwner(this);
	}
}
void UCustomizePanel::SetSelectedButton(int32 index)		//playerController에서 SaveFile을 받을 때 킴.
{
	//Create에서 넣으면 index를 받기전에 widget이 생성되어서 나중에 이렇게 줘야됨.
	Buttons[index]->OnButtonClicked();
	SelectedButtonIndex = index;
}