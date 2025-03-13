// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "UI/HPBarWidget.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "UI/HpBarWidgetComponent.h"
// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));

	HPWidget = CreateDefaultSubobject<UHpBarWidgetComponent>(TEXT("HPBar"));
	HPWidget->SetupAttachment(GetMesh());
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->OnHpZero.AddUObject(this, &ACharacterBase::SetDead);
}

void ACharacterBase::SetupCharacterWidget(UGGUserWidget* InUserWidget)
{
	UHPBarWidget* HpBarWidget = Cast<UHPBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->SetHPBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UHPBarWidget::SetHPBar);
	}
}

void ACharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HPWidget->SetHiddenInGame(true);
}

void ACharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}
UWidgetComponent* ACharacterBase::GetHPWidget()
{
	return HPWidget;
}
