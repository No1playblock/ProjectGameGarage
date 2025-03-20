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


protected:

	APressurePlate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPriavetAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> FloorCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPriavetAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> SwitchCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPriavetAccess = "true"))
	TObjectPtr<class UMaterial> SwitchDefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPriavetAccess = "true"))
	TObjectPtr<class UMaterial> SwitchPushMaterial;
	
public:	
	// Called every frame
	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnButtonPressed OnButtonPressed;

	UFUNCTION(BlueprintCallable)
	void PressButton(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(BlueprintCallable)
	void UnPressButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
