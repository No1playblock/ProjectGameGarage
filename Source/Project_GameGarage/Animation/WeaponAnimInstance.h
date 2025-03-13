// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon/WeaponActor.h"
#include "WeaponAnimInstance.generated.h"

/**
 * 
 */
class APlayableCharacter;
UCLASS()
class PROJECT_GAMEGARAGE_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

    UWeaponAnimInstance();


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TObjectPtr<UAnimSequenceBase> Idle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsWeapon;

protected:

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
    TObjectPtr<class APlayableCharacter> player;

    

  
    

    /** 이동 속도 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed;

    TObjectPtr<UAnimSequenceBase> BaseIdle;

};
