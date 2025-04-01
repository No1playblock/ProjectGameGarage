// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PressurePlate.h"
#include "Components/BoxComponent.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	FloorCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorCube"));
	SwitchCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchCube"));
	
	SwitchCube->SetupAttachment(BoxCollision);
	FloorCube->SetupAttachment(SwitchCube);
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::PressButton);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::UnPressButton);
}

void APressurePlate::PressButton(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwitchCube->SetMaterial(0, SwitchPushMaterial);
	OnButtonPressed.Broadcast(this, true); // 버튼이 눌렸음을 알림
}

void APressurePlate::UnPressButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	SwitchCube->SetMaterial(0, SwitchDefaultMaterial);
	OnButtonPressed.Broadcast(this, false); // 버튼이 눌렸음을 알림
}

