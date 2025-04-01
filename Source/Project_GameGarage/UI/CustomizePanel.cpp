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
	// ������ ���õ� ��Ų ��ư�� ��ȿ�ϴٸ� Ŭ�� ���� ����
	if (SelectedButton)
	{
		SelectedButton->SetClicked(false);
	}		

	// ���ο� ��ư�� Ŭ�� ���·� �����ϰ�, SelectedSkin�� ����
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

		// Mesh�� SkinImage �ε�
		USkeletalMesh* LoadedMesh = Row->MeshName.Get();
		UTexture2D* LoadedTexture = Row->SkinImage.Get();

		if (!LoadedMesh || !LoadedTexture)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load assets skin index : %d"), i);
			continue;
		}
		// UUserWidget ����
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

		// `Mesh`�� `emoImage` �ε�
		UMaterialInstance* LoadedMaterial = Row->EmoMaterial.Get();
		UTexture2D* LoadedTexture = Row->EmoImage.Get();

		if (!LoadedMaterial || !LoadedTexture)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load assets for emo index : %d"), i);
			continue;
		}


		// UUserWidget ����
		UEmoButtonWidget* NewButton = CreateWidget<UEmoButtonWidget>(GetWorld(), CustomButtonWidget);
		if (!NewButton) continue;

		NewButton->SetEmoBtnData(LoadedMaterial, LoadedTexture);

		GridPanel->AddChildToGrid(NewButton, i / 4, i % 4);

		Buttons.Add(NewButton);
		NewButton->SetOwner(this);
	}
}
void UCustomizePanel::SetSelectedButton(int32 index)		//playerController���� SaveFile�� ���� �� Ŵ.
{
	//Create���� ������ index�� �ޱ����� widget�� �����Ǿ ���߿� �̷��� ��ߵ�.
	Buttons[index]->OnButtonClicked();
	SelectedButtonIndex = index;
}