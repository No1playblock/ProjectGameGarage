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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	class UAnimSequence* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	float AttackSpeedRate;

	FComboAttack()
		: Animation(nullptr)
		, DamageMultiplier(1.0f)
		, AttackSpeedRate(1.0f)
	{
	}
};


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GAMEGARAGE_API UMeleeWeaponComponent : public	UStaticMeshComponent
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

	TObjectPtr<class APlayableCharacter> Owner;

public:

	FORCEINLINE UAnimSequence* GetIdleAnim() const { return IdleAnimation; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(APlayableCharacter* TargetCharacter);

	
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float animSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTransform RelativeTransform;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackCombo")
	TArray<FComboAttack> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;


	//ComboAction
	UPROPERTY(Replicated)
	int32 CurrentCombo = 0;

	void PressComboCommand();

	void ComboActionBegin();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ComboActionBegin(float ComboActionStartTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ComboActionBegin();


	UFUNCTION(Server, Reliable)
	void Server_RequestNextCombo(bool bNextCombo);


	UFUNCTION(Client, Reliable)
	void Client_CorrectComboTimer(float LagTime);

	void ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetComboCheckTimer();

	void ComboCheck();

	float ClientDifferenceTime=0.02f;

	float ServerDifferenceTime = 0.02f;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	FTimerHandle ComboTimerHandle;

	UPROPERTY(Replicated)
	bool bHasNextComboCommand=false;


	//ComboActionData
	UPROPERTY(EditAnywhere, Category = "Combo")
	FString MontageSectionNamePrefix = "Combo";

	UPROPERTY(EditAnywhere, Category = "Combo")
	uint8 MaxComboCount = 4;

	UPROPERTY(EditAnywhere, Category = "Combo")
	float FrameRate = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Combo")
	TArray<float> EffectiveFrameCount;

	float AttackTime = 0.0f;
	float LastComboActionStartTime = 0.0f;

};
