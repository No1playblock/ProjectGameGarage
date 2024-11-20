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
    void SetOwner(UCustomizingWidget* NewOwner);

protected:

    // Button Ÿ���� ��ư ����
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    UButton* Button;

    // PreviewImage: UImage Ÿ���� ĳ���� �̹����� ��Ÿ���� ����
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    UImage* PreviewImage;

    // ClickedBorder: Ŭ�� �� ���� ȿ���� ��Ÿ���� UBorder Ÿ���� ����
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
    UBorder* ClickedBorder;

    // MeshImage: Texture2D Ÿ���� �ؽ�ó�� ǥ���ϴ� �̹���
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
    UTexture2D* PreviewTexture;

    //// Owner: ���� WBP_CustomizeCharacter ������ ����Ű�� ����
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    UCustomizingWidget* Owner;

   
};
