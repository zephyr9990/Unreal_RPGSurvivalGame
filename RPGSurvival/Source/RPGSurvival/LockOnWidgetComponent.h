// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LockOnWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGSURVIVAL_API ULockOnWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	/** Shows the target information */
	void ShowInformation(bool Show);
};
