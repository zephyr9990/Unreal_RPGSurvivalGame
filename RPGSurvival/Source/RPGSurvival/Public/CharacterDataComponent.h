// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInfoDataAsset.h"
#include "Components/ActorComponent.h"
#include "StatInfo.h"
#include "CharacterDataComponent.generated.h"

// Forward declaration.
class UCharacterInfoDataAsset;
class ICharacterUIInterface;
class AHero;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class RPGSURVIVAL_API UCharacterDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Sets the data for the character */
	void SetData(UCharacterInfoDataAsset* DataAsset);

private:
	// The owner of this component.
	ICharacterUIInterface* OwnerInterface;

	// Keeps track of stat information.
	FStatInfo StatInfo;
};
