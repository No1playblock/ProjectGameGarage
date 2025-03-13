// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CustomizePlayerController.h"
#include "Game/SaveFile.h"
#include "Character/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CustomizingWidget.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Game/GGGameInstance.h"


ACustomizePlayerController::ACustomizePlayerController()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/Blueprints/Widget/WBP_CustomizeWidget.WBP_CustomizeWidget_C")); // 경로를 수정하세요

    if (WidgetFinder.Succeeded())
    {
        CustomizeWidgetClass = WidgetFinder.Class;
        UE_LOG(LogTemp, Log, TEXT("Customizing Widget Class Load Success"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Customizing Widget Class Load Failed"));
    }
}

void ACustomizePlayerController::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    // SaveFile 로드 및 UI 초기화

    GetCharacter()->DisableInput(this);

    SetCustomizeLevel();
    LoadSaveFileAndInitUI();
}
void ACustomizePlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ACharacter* PlayerCharacter = GetCharacter();
    if (PlayerCharacter)
    {
        FRotator DeltaRotation(0.0f, 1.0f, 0.0f);
        PlayerCharacter->AddActorLocalRotation(DeltaRotation);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Don't find PlayerCharcter"));
    }
}


void ACustomizePlayerController::LoadSaveFileAndInitUI()
{
    // SaveFile 로드
    //USaveFile* LoadedSave = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

    UGGGameInstance* GI = Cast<UGGGameInstance>(GetGameInstance());
    USaveFile* LoadedSave = GI->LoadPlayerData();

   
    //없으면 생성하는거 안넣음

    int32 EmoIndex = 0;
    int32 SkinIndex = 0;

    if (LoadedSave->EmoButtonIndex)
    {
        EmoIndex = LoadedSave->EmoButtonIndex;
        SkinIndex = LoadedSave->SkinButtonIndex;

        UE_LOG(LogTemp, Log, TEXT("Loaded EmoIndex: %d, SkinIndex: %d"), EmoIndex, SkinIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No SaveFile found. Using default indices."));
    }

    // UserWidget 생성 및 초기화
    if (CustomizeWidgetClass)
    {
        CustomizeWidget = CreateWidget<UCustomizingWidget>(this, CustomizeWidgetClass);

        if (CustomizeWidget)
        {
            CustomizeWidget->AddToViewport();

            // 저장된 인덱스 전달
            CustomizeWidget->InitSelectedButton(EmoIndex, SkinIndex);
            SetShowMouseCursor(true);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CustomizeWidgetClass not set in PlayerController."));
    }
}

//Change the Camera, Invisible hpwidget
void ACustomizePlayerController::SetCustomizeLevel()        
{
    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Camera"), FoundCameras);

    for (AActor* Actor : FoundCameras)
    {
        if (Actor)
        {
            FViewTargetTransitionParams TransitionParams;
            SetViewTarget(Actor, TransitionParams);
            break; // 첫 번째 카메라만 사용
        }
    }
    Cast<APlayableCharacter>(GetPawn())->GetHPWidget()->SetVisibility(false);
}