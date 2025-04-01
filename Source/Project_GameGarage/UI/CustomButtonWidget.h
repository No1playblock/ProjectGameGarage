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

    // ������ ���� �Լ�
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

    // Ŭ�� �� ���� ȿ��
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<UBorder> ClickedBorder;

    
private:

    UFUNCTION()
    void SetButtonImage(UTexture2D* NewImage);

    // ��ư Ŭ�� �̺�Ʈ �ڵ鷯
    UFUNCTION()
    void OnButtonPressed();

    UFUNCTION()
    void OnButtonReleased();

    UFUNCTION()
    void OnButtonHovered();

    UFUNCTION()
    void OnButtonUnhovered();

    void SetCharacterImageColorAndOpacity(FLinearColor NewColor);

    //������Ʈ
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<USizeBox> SizeBox;

    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<UButton> Button;

    // ���� UI Image
    UPROPERTY(EditAnyWhere, meta = (BindWidget))
    TObjectPtr<UImage> PreviewImage;

    //�������
    UPROPERTY()
    TObjectPtr<UTexture2D> PreviewTexture;

    UPROPERTY()
    // ���� WBP_CustomizeCharacter ������ ����Ű�� ����
    TObjectPtr<UCustomizePanel> Owner;
    
};
