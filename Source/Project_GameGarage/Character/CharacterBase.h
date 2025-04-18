// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/CharacterWidgetInterface.h"
#include "CharacterBase.generated.h"

class UWidgetComponent;
UCLASS()
class PROJECT_GAMEGARAGE_API ACharacterBase : public ACharacter, public ICharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	

	UWidgetComponent* GetHPWidget();

protected:

	ACharacterBase();

	//�޼ҵ�
	virtual void PostInitializeComponents() override;

	virtual void SetupCharacterWidget(class UGGUserWidget* InUserWidget);

	virtual void SetDead();

	void PlayDeadAnimation();

	//������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HPWidget;
	
	//�������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

};
