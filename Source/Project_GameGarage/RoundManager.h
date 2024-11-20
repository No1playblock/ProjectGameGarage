// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundManager.generated.h"


UENUM(BlueprintType)
enum class ERoundType : uint8
{
	WatingTime UMETA(DisplayName = "WatingTime"),
	PlayTime UMETA(DisplayName = "PlayTime")
};

UCLASS()
class PROJECT_GAMEGARAGE_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	UFUNCTION(BlueprintCallable)
	void timerStart();

	UPROPERTY(Replicated)
	int timer = 5;

	class UUserWidget* HUDWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle timerhandle;

	void timerProgress();
	void timerStop();

	void GameFinish();

	void GotoRoom();

	FVector spawnPosition;

	ERoundType roundtype;
	int roundcnt=0;


};
