// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

// Forward Declaration
class USphereComponent;
class AEnemyCharacter;
class APlayerController;

UCLASS(config=Game)
class AHero : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** The box that detects if enemies are in front of this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* EnemyDetectionSphere;
public:
	AHero();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	* Detects overlap events
	*/
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	* Detects end overlap events 
	*/
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Box subobject **/
	FORCEINLINE class USphereComponent* GetEnemyDetectionSphere() const { return EnemyDetectionSphere; }

private:									   
	/** Called once game is started **/
	virtual void BeginPlay() override;
	
	/** Called every frame **/
	virtual void Tick(float DeltaTime) override;

	/** Finds the closest enemy infront of the player 
	* @param Enemies - The array of enemies to iterate through
	*/
	AEnemyCharacter* FindClosestEnemyInFront(TArray<AActor*>& Enemies);

	/** Compares the distance between ClosestEnemy and Enemy, and returns the closest enemy to the player.
	* @param ClosestEnemy - The current closest enemy.
	* @param Enemy - The enemy to check against.
	* @return The enemy closest between the two.
	*/
	AEnemyCharacter* FindClosestEnemyBetween(AEnemyCharacter* ClosestEnemy, AEnemyCharacter* Enemy);

	/** Gets the vector to the given enemy 
	* @param Enemy - The enemy to calculate a vector to.
	* @return A vector towards the enemy
	*/
	FVector GetVectorTo(AEnemyCharacter* Enemy);

	/** Checks to see if the given enemy is infront of the player 
	* @param ToEnemy - The enemy to get the vector to.
	* @return true if enemy is infront and false if not.
	*/
	bool IsInFront(AEnemyCharacter* Enemy);

	// Used to see if the character is in battle.
	bool bInBattle;
};

