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

    // 버튼 이벤트 바인딩
    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &USkinButtonWidget::OnButtonClicked);
    }
}
void USkinButtonWidget::OnButtonClicked()
{
    if (Owner)
    {
        Owner->SetSkinButton(this); // Self로 전달
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerNot FOUND_Skin"));
    }
    
}
void USkinButtonWidget::SetClicked(bool bIsClicked)
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
        ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        Cast<APlayableCharacter>(character)->SetSkin(Skin);
    }
}
USkeletalMesh* USkinButtonWidget::GetSkin()
{
    return Skin;
}