// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "CustomizingWidget.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/CustomizePanel.h"


void UCustomButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnPressed.AddDynamic(this, &UCustomButtonWidget::OnButtonPressed);
		Button->OnReleased.AddDynamic(this, &UCustomButtonWidget::OnButtonReleased);
		Button->OnHovered.AddDynamic(this, &UCustomButtonWidget::OnButtonHovered);
		Button->OnUnhovered.AddDynamic(this, &UCustomButtonWidget::OnButtonUnhovered);
		Button->OnClicked.AddDynamic(this, &UCustomButtonWidget::OnButtonClicked);
	
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
	if (Owner)
	{
		Owner->ButtonClicked(this); // Self로 전달
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerNot FOUND_Emo"));
	}
}

void UCustomButtonWidget::SetClicked(bool bIsClicked)
{

}

void UCustomButtonWidget::SetOwner(UCustomizePanel* NewOwner)
{
	Owner = NewOwner;
}
void UCustomButtonWidget::SetButtonImage(UTexture2D* NewImage)
{
	if (PreviewImage && NewImage)
	{
		PreviewImage->SetBrushFromTexture(NewImage);
		
	}
}
void UCustomButtonWidget::SetPreviewTexture(UTexture2D* texture)
{
	PreviewTexture = texture;
	SetButtonImage(PreviewTexture);
}



void UCustomButtonWidget::SetCharacterImageColorAndOpacity(FLinearColor NewColor)
{
	if (PreviewImage)
	{
		PreviewImage->SetColorAndOpacity(NewColor);
	}
}