// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PlayableCharacter.h"
#include "Weapon/WeaponActor.h"
#include "MeleeWeaponActor.generated.h"



class UPickUpComponent;
class UMeleeWeaponComponent;

UCLASS()
class PROJECT_GAMEGARAGE_API AMeleeWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMeleeWeaponComponent* meleeComponent;

	virtual void PickUpWeapon(APlayableCharacter* PickUpCharacter) override;
public:	
	// Sets default values for this actor's properties
	AMeleeWeaponActor();

	UFUNCTION()
	void OverlapmeleeComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)	//충돌 한번만 확인하기 위해서
	bool bisHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UMeleeWeaponComponent* GetMeleeComponent();

};
