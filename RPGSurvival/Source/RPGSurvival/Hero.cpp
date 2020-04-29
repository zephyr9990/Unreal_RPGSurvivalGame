// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Hero.h"
#include "EnemyCharacter.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AHero

AHero::AHero()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the detection box
	FVector DefaultDetectionSphereSize = FVector(0.0f, 0.0f, 0.0f);
	float DefaultDetectionSphereRadius = 1000.0f;
	EnemyDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyDetectionSphere"));
	EnemyDetectionSphere->SetupAttachment(GetCapsuleComponent());
	EnemyDetectionSphere->SetRelativeLocation(DefaultDetectionSphereSize);
	EnemyDetectionSphere->SetSphereRadius(DefaultDetectionSphereRadius);

	EnemyDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHero::OnOverlapBegin);
	EnemyDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AHero::OnOverlapEnd);

	// Initialize class variables
	bInBattle = false;
	bIsLockedOntoEnemy = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AHero::BeginPlay()
{
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////
// Input

void AHero::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Actions
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("LockOn", EInputEvent::IE_Pressed, this, &AHero::ToggleLockOn);

	// Cardinal Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHero::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHero::LookUpAtRate);
}

void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bInBattle && !bIsLockedOntoEnemy)
	{
		TArray<AActor*> Enemies;
		EnemyDetectionSphere->GetOverlappingActors(Enemies, TSubclassOf<AEnemyCharacter>());
		ClosestEnemyInFront = FindClosestEnemyInFront(Enemies);

		if (ClosestEnemyInFront)
		{
			ClosestEnemyInFront->ShowInformation(true);
		}

	}

	if (bIsLockedOntoEnemy)
	{
		TrackLockedOnEnemy();
		if (!PlayerInView())
		{
			AdjustCameraBoomToSeePlayerAndEnemy(DeltaTime);
		}
	}
}

void AHero::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHero::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHero::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHero::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AHero::ToggleLockOn()
{
	if (ClosestEnemyInFront && bInBattle)
	{
		CameraBoom->bUsePawnControlRotation = false;

		bIsLockedOntoEnemy = !bIsLockedOntoEnemy;
		ClosestEnemyInFront->ToggleLockOn();
	}

	if (!bIsLockedOntoEnemy)
	{
		CameraBoom->bUsePawnControlRotation = true;

		FollowCamera->SetRelativeRotation(FQuat(FRotator::ZeroRotator));
		CameraBoom->SetWorldRotation(FQuat(FRotator::ZeroRotator));
	}
}

void AHero::TrackLockedOnEnemy()
{
	FVector ToMidPoint = GetVectorTo(ClosestEnemyInFront);
	FVector LocationOfMidPoint = GetActorLocation() + ToMidPoint / 2;
	FRotator ToEnemyRotation = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), LocationOfMidPoint);

	FollowCamera->SetWorldRotation(ToEnemyRotation);
	Controller->SetControlRotation(ToEnemyRotation);
}

bool AHero::PlayerInView()
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector2D ScreenLocation;
	PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenLocation);

	int32 ScreenWidth = 0;
	int32 ScreenHeight = 0;
	PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

	// Define the upper and right boundaries that the player can remain in
	int32 ScreenWidthRightBoundary = ScreenWidth * .75;
	int32 ScreenHeightLowerBoundary = ScreenHeight * .75;

	// Define the left and lower boundaries that the player can remain in
	int32 ScreenWidthLeftBoundary = ScreenWidth * .25;
	int32 ScreenHeightUpperBoundary = ScreenHeight * .25;

	int32 ScreenX = (int32)ScreenLocation.X;
	int32 ScreenY = (int32)ScreenLocation.Y;

	return ScreenX >= ScreenWidthLeftBoundary && ScreenY >= ScreenHeightUpperBoundary
		&& ScreenX < ScreenWidthRightBoundary && ScreenY < ScreenHeightLowerBoundary;
}

void AHero::AdjustCameraBoomToSeePlayerAndEnemy(float DeltaTime)
{
	FVector ToMidPoint = GetVectorTo(ClosestEnemyInFront);
	FVector LocationOfMidPoint = GetActorLocation() + ToMidPoint / 2;
	FRotator ToEnemyRotation = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), LocationOfMidPoint);

	FRotator CameraBoomTargetRotation =
		UKismetMathLibrary::RLerp(CameraBoom->GetComponentRotation(), ToEnemyRotation, DeltaTime * 2, true);

	CameraBoom->SetWorldRotation(CameraBoomTargetRotation);
	UE_LOG(LogTemp, Warning, TEXT("Player in view: %s"), *CameraBoom->GetComponentRotation().ToString());
}

AEnemyCharacter* AHero::FindClosestEnemyInFront(TArray<AActor*>& Enemies)
{
	AEnemyCharacter* ClosestEnemy = nullptr;
	for (AActor* EnemyInRange : Enemies)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyInRange);
		if (Enemy)
		{
			Enemy->ShowInformation(false);
			if (IsInFront(Enemy))
			{
				ClosestEnemy = FindClosestEnemyBetween(ClosestEnemy, Enemy);
			}
		}
	}
	return ClosestEnemy;
}

AEnemyCharacter* AHero::FindClosestEnemyBetween(AEnemyCharacter* ClosestEnemy, AEnemyCharacter* Enemy)
{
	AEnemyCharacter* NewClosestEnemy = nullptr;
	if (ClosestEnemy)
	{
		FVector ToEnemy = GetVectorTo(Enemy);
		FVector ToClosestEnemy = GetVectorTo(ClosestEnemy);
		if (ToEnemy.Size() < ToClosestEnemy.Size())
		{
			NewClosestEnemy = Enemy;
		}
		else
		{
			NewClosestEnemy = ClosestEnemy;
		}
	}
	else
	{
		NewClosestEnemy = Enemy;
	}

	return NewClosestEnemy;
}

bool AHero::IsInFront(AEnemyCharacter* Enemy)
{
	FVector ToEnemy = GetVectorTo(Enemy);
	ToEnemy.Normalize();

	FVector ActorForward = GetActorForwardVector();
	float AngleToEnemy = UKismetMathLibrary::DegAcos(FVector::DotProduct(ActorForward, ToEnemy));

	return AngleToEnemy < 90.0f;
}

FVector AHero::GetVectorTo(AEnemyCharacter* Enemy)
{
	return Enemy->GetActorLocation() - GetActorLocation();
}

void AHero::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		bInBattle = true;
	}
}

void AHero::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		Enemy->ShowInformation(false);
	}

	// Get the number of enemies in range.
	TArray<AActor*> EnemiesInRange;
	EnemyDetectionSphere->GetOverlappingActors(EnemiesInRange, TSubclassOf<AEnemyCharacter>());

	// The player is fleeing from battle. Stop battle mode and disengage lock on.

	if (EnemiesInRange.Num() == 1)
	{
		if (bIsLockedOntoEnemy && ClosestEnemyInFront) 
		{
			ToggleLockOn();
			bInBattle = false;
		}
	}
}