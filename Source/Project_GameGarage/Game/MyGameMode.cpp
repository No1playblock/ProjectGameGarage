// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UI/CrosshairWidget.h"
#include "Character/PlayableCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Game/MyGameState.h"
#include "Engine/World.h"

#define FIGHTTIME 60
#define WAITTIME 30
#define BUTTONTIME 10

void AMyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerArr.Add(Cast<APlayableCharacter>(NewPlayer->GetPawn()));
	if (PlayerNum == 1)
	{
		NewPlayer->GetPawn()->SetActorLocation(SpawnPoint1);
		PlayerNum++;
	}
	else
	{
		NewPlayer->GetPawn()->SetActorLocation(SpawnPoint2);
	}
}

void AMyGameMode::TimerProgress()
{
	if (Timer <= 0)
	{
		Timer = 0.0f;
		MyGS->SetTimer(Timer);
		TimerStop();
	}
	else
	{
		Timer -= 1;
		MyGS->SetTimer(Timer);
	}
}

void AMyGameMode::TimerStart()
{
	GetWorld()->GetTimerManager().SetTimer(Timerhandle, this, &AMyGameMode::TimerProgress, 1.0, true);
}

void AMyGameMode::TimerStop()
{

	GetWorld()->GetTimerManager().ClearTimer(Timerhandle);
	bGameStart = true;

	if (RoundType == ERoundType::WatingTime)		//���� ������ Ÿ���̸� ���� �����ؾߵ�
	{
		RoundType = ERoundType::PlayTime;
		Timer = FIGHTTIME;				//���� �ð�
		MyGS->SetTimer(Timer);

		GotoBattleField();;
	}
	else	//���� ��
	{
		RoundCnt++;
		if (RoundCnt == 2)		//��� ���� ������
		{
			GameFinish();	//���â �� �ɿ������ �̵�
		}
		else
		{
			RoundType = ERoundType::WatingTime;
			Timer = WAITTIME;		//Waiting�ð� ��ŭ Ÿ�̸� ����
			MyGS->SetTimer(Timer);

			PlayerArr[0]->SetActorLocation(SpawnPoint1);
			PlayerArr[1]->SetActorLocation(SpawnPoint2);
			TimerStart();
		}
	}
}
void AMyGameMode::GameFinish()
{
	GetWorld()->GetTimerManager().ClearTimer(Timerhandle);
	//�������� �ڵ�
}
void AMyGameMode::GotoBattleField()
{

	FVector spawnPosition;
	for (int i = 0; i < 2; i++)
	{
		if (PlayerArr[i])
		{
			switch (RoundCnt) {
			case 0:
				// player1  position
				if (i == 0) {
					spawnPosition = Map1SpawnPoint1;
				}

				// player2  position
				if (i == 1) {
					spawnPosition = Map1SpawnPoint2;
				}
				break;
			case 1:
				// player1  position
				if (i == 0) {
					spawnPosition = Map2SpawnPoint1;
				}

				// player2  position
				if (i == 1) {
					spawnPosition = Map2SpawnPoint2;
				}
				break;
			}
			PlayerArr[i]->SetActorLocation(spawnPosition);
		}
	}

	TimerStart();
}
// Called when the game starts or when spawned
void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundButtons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APressurePlate::StaticClass(), FoundButtons);

	for (AActor* FoundActor : FoundButtons)
	{
		APressurePlate* Button = Cast<APressurePlate>(FoundActor);
		if (Button)
		{
			Buttons.Add(Button);
			ButtonStates.Add(Button, false); // ��� ��ư�� "������ ����" ���·� �ʱ�ȭ

			// ��ư �̺�Ʈ ���ε�
			Button->OnButtonPressed.AddDynamic(this, &AMyGameMode::OnButtonPressed);
		}
	}
	MyGS = GetGameState<AMyGameState>();

}
void AMyGameMode::OnButtonPressed(APressurePlate* Button, bool bIsPressed)
{
	if (ButtonStates.Contains(Button))
	{
		ButtonStates[Button] = bIsPressed;
		CheckAllButtonsPressed();
	}
	if (!bIsPressed && !bGameStart)		//��ư�� ������		������ �����Ѱ� �ƴϸ�
	{

		GetWorld()->GetTimerManager().ClearTimer(Timerhandle);

	}
}

void AMyGameMode::CheckAllButtonsPressed()
{
	for (const auto& Pair : ButtonStates)
	{
		if (!Pair.Value)
		{
			return; // �ϳ��� �� ���� ������ ����
		}
	}
	// ��� ��ư�� ���ȴٸ� RoundStart ���

	Timer = BUTTONTIME;
	MyGS->SetTimer(Timer);
	TimerStart();

}