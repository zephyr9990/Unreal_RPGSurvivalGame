// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "LockOnWidgetComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup LockOnWidget defaults
	LockOnWidget = CreateDefaultSubobject<ULockOnWidgetComponent>(FName("LockOnWidget"));
	LockOnWidget->SetupAttachment(GetCapsuleComponent());

	FVector DefaultLockOnWidgetLocation = FVector(0.0f, 0.0f, 35.0f);
	LockOnWidget->SetRelativeLocation(DefaultLockOnWidgetLocation);
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (LockOnWidget->GetWidgetClass() == nullptr)
	{ 
		UE_LOG(LogTemp, Error, TEXT("No WidgetClass on %s"), *GetName());
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

void AEnemyCharacter::ShowInformation(bool Show)
{
	if (LockOnWidget)
	{
		LockOnWidget->ShowInformation(Show);
	}
}

