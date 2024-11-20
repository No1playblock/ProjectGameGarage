// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomButtonWidget.generated.h"

/**
 * 
 */
class UButton;
class UImage;
class UBorder;
class UCustomizingWidget;
UCLASS()
class PROJECT_GAMEGARAGE_API UCustomButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    // 버튼 클릭 이벤트 핸들러
    UFUNCTION()
    void OnButtonPressed();

    UFUNCTION()
    void OnButtonReleased();

    UFUNCTION()
    void OnButtonHovered();

    UFUNCTION()
    void OnButtonUnhovered();

    UFUNCTION()
    virtual void OnButtonClicked();


    void SetCharacterImageColorAndOpacity(FLinearColor NewColor);

public:

    // 클릭 상태 설정 함수
    UFUNCTION(BlueprintCallable)
    virtual void SetClicked(bool bIsClicked);

    // 소유자 설정 함수
    UFUNCTION(BlueprintCallable)
    void SetOwner(UCustomizingWidget* NewOwner);

protected:

    // Button 타입의 버튼 위젯
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    UButton* Button;

    // PreviewImage: UImage 타입의 캐릭터 이미지를 나타내는 변수
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    UImage* PreviewImage;

    // ClickedBorder: 클릭 시 보더 효과를 나타내는 UBorder 타입의 변수
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    UBorder* ClickedBorder;

    // MeshImage: Texture2D 타입의 텍스처를 표시하는 이미지
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
    UTexture2D* PreviewTexture;

    //// Owner: 상위 WBP_CustomizeCharacter 위젯을 가리키는 변수
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    UCustomizingWidget* Owner;

   
};
