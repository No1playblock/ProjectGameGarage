// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Character/PlayableCharacter.h"
#include "PickUpComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, APlayableCharacter*, PickUpCharacter);		//FOnPickUp라는 이름의 델리게이트 선언

UCLASS()
class PROJECT_GAMEGARAGE_API UPickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnyWhere, Category = "Interaction")
	FOnPickUp OnPickUp;
	
private:
	UPickUpComponent();

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	bool bHasExecutedOnce = false;
};
