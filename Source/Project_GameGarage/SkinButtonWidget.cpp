// Fill out your copyright notice in the Description page of Project Settings.


#include "SkinButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter.h"
#include "CustomizingWidget.h"

void USkinButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ư �̺�Ʈ ���ε�
    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &USkinButtonWidget::OnButtonClicked);
    }
}
void USkinButtonWidget::OnButtonClicked()
{
    if (Owner)
    {
        Owner->SetSkinButton(this); // Self�� ����
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerNot FOUND_Skin"));
    }
    
}
void USkinButtonWidget::SetClicked(bool bIsClicked)
{

    // Ŭ�� ���¿� ���� ���� ����
    FLinearColor Color = bIsClicked ? FLinearColor(0.0f, 0.973157f, 1.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    if (ClickedBorder)
    {
        ClickedBorder->SetBrushColor(Color);
    }

    // ĳ������ �޽� ����
    if (bIsClicked)
    {
        ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        Cast<APlayableCharacter>(character)->SetSkin(Skin);
    }
}
USkeletalMesh* USkinButtonWidget::GetSkin()
{
    return Skin;
}