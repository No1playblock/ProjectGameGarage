// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "PressurePlate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonPressed, APressurePlate*, Button, bool, bIsPressed);

UCLASS()
class PROJECT_GAMEGARAGE_API APressurePlate : public AActor
{
	GENERATED_BODY()

public:	

	UFUNCTION()
	void PressButton(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UnPressButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnButtonPressed OnButtonPressed;

private:

	APressurePlate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> FloorCube;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> SwitchCube;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterial> SwitchDefaultMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterial> SwitchPushMaterial;

};
