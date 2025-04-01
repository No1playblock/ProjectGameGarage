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

    // 소유자 설정 함수
    UFUNCTION()
    void SetOwner(UCustomizePanel* NewOwner);

    UFUNCTION()
    void SetPreviewTexture(UTexture2D* Texture);

    UFUNCTION()
    virtual void OnButtonClicked();

    UFUNCTION()
    virtual void SetClicked(bool bIsClicked);
protected:

    virtual void NativeConstruct() override;

    // 클릭 시 보더 효과
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<UBorder> ClickedBorder;

    
private:

    UFUNCTION()
    void SetButtonImage(UTexture2D* NewImage);

    // 버튼 클릭 이벤트 핸들러
    UFUNCTION()
    void OnButtonPressed();

    UFUNCTION()
    void OnButtonReleased();

    UFUNCTION()
    void OnButtonHovered();

    UFUNCTION()
    void OnButtonUnhovered();

    void SetCharacterImageColorAndOpacity(FLinearColor NewColor);

    //컴포넌트
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<USizeBox> SizeBox;

    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<UButton> Button;

    // 띄우는 UI Image
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<UImage> PreviewImage;

    //멤버변수
    UPROPERTY()
    TObjectPtr<UTexture2D> PreviewTexture;

    UPROPERTY()
    // 상위 WBP_CustomizeCharacter 위젯을 가리키는 변수
    TObjectPtr<UCustomizePanel> Owner;
    
};
