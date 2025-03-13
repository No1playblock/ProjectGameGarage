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
class USizeBox;
class UCanvasPanel;
class UCustomizePanel;

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
    void SetOwner(UCustomizePanel* NewOwner);

    UFUNCTION()
    void SetButtonImage(UTexture2D* NewImage);

    UFUNCTION()
    void SetPreviewTexture(UTexture2D* texture);

protected:


    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<USizeBox> SizeBox;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UButton> Button;

    // PreviewImage: 띄우는 UI
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> PreviewImage;

    // ClickedBorder: 클릭 시 보더 효과
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UBorder> ClickedBorder;

    //이미지를 담을 변수
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTexture2D> PreviewTexture;

    //// Owner: 상위 WBP_CustomizeCharacter 위젯을 가리키는 변수
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    TObjectPtr<UCustomizePanel> Owner;

};
