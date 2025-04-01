// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveFile.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayableCharacter.h"
#include "Components/SkeletalMeshComponent.h"
USaveFile::USaveFile()
{
    SkinButtonIndex = 0;
    EmoButtonIndex = 0;

    APlayableCharacter* DefaultCharacter = GetMutableDefault<APlayableCharacter>();
    if (DefaultCharacter && DefaultCharacter->GetMesh())
    {
        SaveSkin = DefaultCharacter->GetMesh()->GetSkeletalMeshAsset();
        SaveEmo = Cast<UMaterialInstance>(DefaultCharacter->GetMesh()->GetMaterial(1));
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("SaveFile NULLNULL"));


}