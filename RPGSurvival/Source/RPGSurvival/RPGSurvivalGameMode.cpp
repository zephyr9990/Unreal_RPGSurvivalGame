// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RPGSurvivalGameMode.h"
#include "Hero.h"
#include "UObject/ConstructorHelpers.h"

ARPGSurvivalGameMode::ARPGSurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/RPGSurvival/Blueprints/Hero_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
