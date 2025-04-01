// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"

UPickUpComponent::UPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;

	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnSphereBeginOverlap);
}

void UPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	APlayableCharacter* Character = Cast<APlayableCharacter>(OtherActor);
	if (Character != nullptr)
	{
		OnComponentBeginOverlap.RemoveAll(this);

		//�������� �浹�� OnPickUp�� ���ε��س��� �޼ҵ带 ����
		OnPickUp.Broadcast(Character);
	}
}