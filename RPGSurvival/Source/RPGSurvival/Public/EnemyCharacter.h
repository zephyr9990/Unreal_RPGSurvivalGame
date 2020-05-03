// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatInfo.h"
#include "CharacterUIInterface.h"
#include "EnemyCharacter.generated.h"

// Forward declaration
class UWidgetComponent;
class UCharacterInfoDataAsset;
class UCharacterDataComponent;

UCLASS()
class RPGSURVIVAL_API AEnemyCharacter : public ACharacter, public ICharacterUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// The target widget component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* LockOnWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* StatWidget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Shows enemy information */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowInformation(bool Show);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStats(FStatInfo StatInfo);

	/** Show that this enemy is currently locked onto */
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleLockOn();

private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	UCharacterInfoDataAsset* EnemyDataAsset;

	UCharacterDataComponent* EnemyDataComponent;
};
