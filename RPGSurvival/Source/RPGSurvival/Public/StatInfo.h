// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RPGSURVIVAL_API FStatInfo
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentMP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HPPercent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MPPercent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaggerPercent;

	FStatInfo();
	~FStatInfo();
};
