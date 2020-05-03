// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataComponent.h"
#include "Hero.h"
#include "CharacterInfoDataAsset.h"
#include "CharacterUIInterface.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UCharacterDataComponent::UCharacterDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// Tell the component who its owner is.
	OwnerInterface = Cast<ICharacterUIInterface>(GetOwner());
}


// Called every frame
void UCharacterDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterDataComponent::SetData(UCharacterInfoDataAsset* DataAsset)
{
	StatInfo.Name = DataAsset->Name;

	StatInfo.CurrentHP = DataAsset->StartingHealth;
	StatInfo.MaxHP = DataAsset->StartingHealth;
	StatInfo.HPPercent = 
		UKismetMathLibrary::Conv_IntToFloat(StatInfo.CurrentHP)/ StatInfo.MaxHP;

	StatInfo.MaxMP = DataAsset->StartingMP;
	StatInfo.CurrentMP = DataAsset->StartingMP;
	StatInfo.MPPercent =
		UKismetMathLibrary::Conv_IntToFloat(StatInfo.CurrentMP) / StatInfo.MaxMP;

	OwnerInterface->UpdateStats(StatInfo);
}	