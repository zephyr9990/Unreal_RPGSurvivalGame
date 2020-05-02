// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterInfoDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RPGSURVIVAL_API UCharacterInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartingHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartingMP;
};
