// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyGameState.h"
#include "Net/UnrealNetwork.h"
#include "UI/CrosshairWidget.h"
#include "Character/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyGameState::AMyGameState()
{
    Timer = 4;
}
void AMyGameState::BeginPlay()
{
    Super::BeginPlay();

}
void AMyGameState::OnRep_TimerChanged()
{
    APlayableCharacter* player = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    UUserWidget* widget = player->GetCrosshairWidgetInstance();
    
    if (widget)
    {
        Cast<UCrosshairWidget>(widget)->SetTimerText(FText::FromString(FString::FromInt(Timer)));
    }

}

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyGameState, Timer);
}

void AMyGameState::SetTimer(int time)
{
   
    if (time < 0)
    {
        Timer = 0;
    }
    else
    {
        Timer = time;
    }
    
    //서버에서 자체 실행
    OnRep_TimerChanged();
}
