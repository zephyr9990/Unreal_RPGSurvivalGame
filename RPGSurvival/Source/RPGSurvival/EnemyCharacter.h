// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

// Forward declaration
class ULockOnWidgetComponent;

UCLASS()
class RPGSURVIVAL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// The target widget component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	ULockOnWidgetComponent* LockOnWidget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Shows enemy information */
	void ShowInformation(bool Show);
};
