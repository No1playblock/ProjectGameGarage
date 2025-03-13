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

    // ��ư Ŭ�� �̺�Ʈ �ڵ鷯
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


    // Ŭ�� ���� ���� �Լ�
    UFUNCTION(BlueprintCallable)
    virtual void SetClicked(bool bIsClicked);

    // ������ ���� �Լ�
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

    // PreviewImage: ���� UI
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> PreviewImage;

    // ClickedBorder: Ŭ�� �� ���� ȿ��
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UBorder> ClickedBorder;

    //�̹����� ���� ����
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTexture2D> PreviewTexture;

    //// Owner: ���� WBP_CustomizeCharacter ������ ����Ű�� ����
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    TObjectPtr<UCustomizePanel> Owner;

};
