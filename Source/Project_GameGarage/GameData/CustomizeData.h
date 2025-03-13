// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomizeData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCustomizeData : public FTableRowBase
 {
	 GENERATED_BODY()

 public:


	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Customize)
	 TObjectPtr<class UTexture2D> SkinImage;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Customize)
	 TObjectPtr<class USkeletalMesh> MeshName;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Customize)
	 TObjectPtr<class UTexture2D> EmoImage;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Customize)
	 TObjectPtr<class UMaterialInstance> EmoMaterial;



 };
