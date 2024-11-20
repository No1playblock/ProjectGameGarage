// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"
#include "Net/UnrealNetwork.h"
#include "CrosshairWidget.h"
#include "PlayableCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values
ARoundManager::ARoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	roundtype = ERoundType::WatingTime;
	timer = 4;
	roundcnt=1;
}

void ARoundManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoundManager, timer);
}

void ARoundManager::timerProgress()
{
	if (timer < 0)
	{
		timer = 0.0f;
		timerStop();
	}
	else
	{
		if (HUDWidget)
		{
			
			Cast<UCrosshairWidget>(HUDWidget)->timerText->SetText(FText::FromString(FString::FromInt(timer)));
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("%f"), timer);
		timer -= 1;
	}
}

void ARoundManager::timerStart()
{
	GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &ARoundManager::timerProgress, 1.0, true);
	
}

void ARoundManager::timerStop()
{
	UE_LOG(LogTemp, Warning, TEXT("TimerStop"));

	GetWorld()->GetTimerManager().ClearTimer(timerhandle);
	if (roundtype == ERoundType::WatingTime)
	{
		switch (roundcnt)
		{
			case 1:
				spawnPosition= FVector(49.f, 1150.f, 126.f);
				break;
			case 2:
				spawnPosition= FVector(-1646.f, 392.f, 943.f);
				break;
			case 3:
				spawnPosition= FVector(-44860.f, 290.f, 100.f);
				break;
		}
		roundtype = ERoundType::PlayTime;
		timer = 7;				//라운드 시간
	}
	else
	{
		roundtype = ERoundType::WatingTime;
		timer = 4;		//Waiting시간 만큼 타이머 설정
		spawnPosition=FVector(360.f, -10.f, 122.f);
		roundcnt++;
	}

	if (roundcnt <= 3)
	{
		GotoRoom();
	}
	else
	{
		GameFinish();	//결과창 및 능욕룸으로 이동
	}
	
}
void ARoundManager::GameFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Finish"));
}
void ARoundManager::GotoRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("GotoRoom"));

	/*ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (player)
	{
		player->SetActorLocation(spawnPosition);
	}*/
	int i = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		UE_LOG(LogTemp, Warning, TEXT("i : %d"), i);

		if (PlayerController)
		{
			switch (roundcnt) {
				case 1:	
					// player1  position
					if (i == 0) {
						spawnPosition = FVector(1230.f, 22830.f, 1133.f);
					}

					// player2  position
					if (i == 1) {
						spawnPosition = FVector(1380.f, 24170.f, 1142.f);
					}
					break;
				case 2: 
					// player1  position
					if (i == 0) {
						spawnPosition = FVector(700.f, 2100.f, 943.f);
					}

					// player2  position
					if (i == 1) {
						spawnPosition = FVector(180.f, -2110.f, 873.f);
					}
					break;
				case 3: 
					// player1  position
					if (i == 0) {
						spawnPosition = FVector(-44700.f, 1806.f, 100.f);
					}

					// player2  position
					if (i == 1) {
						spawnPosition = FVector(-44600.f, -2280.f, 242.f);
					}
					break;
			}
			PlayerController->GetCharacter()->SetActorLocation(spawnPosition);
		}
		i++;
	}
	timerStart();
}
// Called when the game starts or when spawned
void ARoundManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("player"));
		UUserWidget* userwidget = Cast<APlayableCharacter>(player)->CrosshairWidgetInstance;
		if (userwidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("userwidget"));
		}
		HUDWidget = Cast<UCrosshairWidget>(userwidget);
		if (userwidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDWidget"));
		}
	}*/
}

