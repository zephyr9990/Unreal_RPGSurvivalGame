// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataComponent.h"

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

	// ...
	
}


// Called every frame
void UCharacterDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterDataComponent::SetData(UCharacterInfoDataAsset* DataAsset)
{
	UE_LOG(LogTemp, Warning, TEXT("Data asset received"))
}