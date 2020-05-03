// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "CharacterDataComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup LockOnWidget defaults
	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(FName("LockOnWidget"));
	LockOnWidget->SetupAttachment(RootComponent);
	FVector DefaultLockOnWidgetLocation = FVector(0.0f, 0.0f, 35.0f);
	LockOnWidget->SetRelativeLocation(DefaultLockOnWidgetLocation);
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidget->SetVisibility(false);

	// Create Stat Widget defaults
	StatWidget = CreateDefaultSubobject<UWidgetComponent>(FName("StatWidget"));
	StatWidget->SetupAttachment(RootComponent);
	FVector DefaultStatWidgetLocation = FVector(0.0f, 0.0f, 97.0f);
	StatWidget->SetRelativeLocation(DefaultStatWidgetLocation);
	StatWidget->SetWidgetSpace(EWidgetSpace::Screen);
	StatWidget->SetVisibility(false);

	// Create Character Data Component
	EnemyDataComponent = CreateDefaultSubobject<UCharacterDataComponent>(FName("EnemyDataComponent"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (LockOnWidget->GetWidgetClass() == nullptr)
	{ 
		UE_LOG(LogTemp, Error, TEXT("No WidgetClass on %s"), *GetName());
	}

	if (EnemyDataAsset)
	{
		EnemyDataComponent->SetData(EnemyDataAsset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No data asset on: "), *GetName());
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
