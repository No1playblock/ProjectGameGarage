// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MeleeWeaponComponent.generated.h"

/**
 * 
 */
class APlayableCharacter;



USTRUCT(BlueprintType)
struct FComboAttack
{
	GENERATED_BODY()

	// 애니메이션 시퀀스를 저장할 필드입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	class UAnimSequence* Animation;

	// 데미지 배수 (float 형식)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	float DamageMultiplier;

	// 공격 속도 비율 (float 형식)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	float AttackSpeedRate;

	// 구조체의 기본 생성자
	FComboAttack()
		: Animation(nullptr)
		, DamageMultiplier(1.0f)
		, AttackSpeedRate(1.0f)
	{
	}
};


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GAMEGARAGE_API UMeleeWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	

private:
	UMeleeWeaponComponent();

	APlayableCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	USoundBase* SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* SwingMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwingAction;

public:

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(APlayableCharacter* TargetCharacter);

	UFUNCTION(Category = "Weapon")
	void Swing();

	UFUNCTION(Server, Reliable, WithValidation, Category = "Weapon")
	void Server_Swing();

	UFUNCTION(NetMulticast, Reliable, WithValidation, Category = "Weapon")
	void Multicast_Swing();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float animSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector EulerRotation;

	
protected:

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackCombo")
	TArray<FComboAttack> Attacks;


};
