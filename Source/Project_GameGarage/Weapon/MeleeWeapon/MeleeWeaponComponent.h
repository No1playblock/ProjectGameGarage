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

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GAMEGARAGE_API UMeleeWeaponComponent : public	UStaticMeshComponent
{
	GENERATED_BODY()

public:

	FORCEINLINE UAnimSequence* GetIdleAnim() const { return IdleAnimation; }

	bool AttachWeapon(APlayableCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void StartSimpleAutoSwing(float Interval = 2.0f);

private:

	UMeleeWeaponComponent();

	//콤보 액션
	void PressComboCommand();

	void ExecuteSimpleAutoSwing();

	UFUNCTION(Server, Reliable)
	void Server_SimpleAutoSwing();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SimpleAutoSwing();

	UFUNCTION(Server, Reliable)
	void Server_ComboActionBegin();


	UFUNCTION(Server, Reliable)
	void Server_RequestNextCombo(bool bNextCombo);

	UFUNCTION()
	void OnRep_CurrentCombo();

	void ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetComboCheckTimer();

	void ComboCheck();

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//ComboAction
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentCombo)
	int32 CurrentCombo = 0;

	bool bIsCombo;


	UPROPERTY(Replicated)
	bool bHasNextComboCommand = false;

	FTimerHandle ComboTimerHandle;

	FTimerHandle AutoSwingTimerHandle;


	//캐릭터
	UPROPERTY(Replicated)
	APlayableCharacter* Character;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimSequence> IdleAnimation;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> SwingMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SwingAction;


	UPROPERTY(EditAnywhere)
	float animSpeed;

	UPROPERTY(EditAnywhere)
	FTransform RelativeTransform;

	TObjectPtr<class APlayableCharacter> Owner;

	//ComboActionData
	UPROPERTY(EditAnywhere, Category = "Combo")
	FString MontageSectionNamePrefix = "Combo";

	UPROPERTY(EditAnywhere, Category = "Combo")
	uint8 MaxComboCount = 4;

	UPROPERTY(EditAnywhere, Category = "Combo")
	float FrameRate = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Combo")
	TArray<float> EffectiveFrameCount;

};
