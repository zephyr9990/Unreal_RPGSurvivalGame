// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatInfo.h"
#include "CharacterUIInterface.h"
#include "Hero.generated.h"

// Forward Declaration
class USphereComponent;
class UCharacterInfoDataAsset;
class UCharacterDataComponent;
class AEnemyCharacter;
class APlayerController;

UCLASS(config=Game)
class AHero : public ACharacter, public ICharacterUIInterface
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

	/** Called when the player presses the attack button. */
	void Attack();

	/** Called when player presses the confirm button */
	void Confirm();

	/** Called when player presses the dodge button */
	void Dodge();

	/** Locks onto the enemy with it's lock-on UI enabled. */
	void ToggleLockOn();

	/** Switches lock on target to the enemy left of the currently locked on target. */
	void TargetLeftEnemy();

	/** Switches lock on target to the enemy right of the currently locked on target. */
	void TargetRightEnemy();

	UFUNCTION()
		/** Detects overlap events */
		void OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		/** Detects end overlap events */
		void OnOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Tells blueprint to start combat animations */
	UFUNCTION(BlueprintImplementableEvent)
	void EnterCombatMode(bool bInCombat);

	/** Tells the UI to fire animations for a button press */
	UFUNCTION(BlueprintImplementableEvent)
		void UIButtonPressed(int32 ButtonPressed);

	/** Tells the UI to change the player stat values. */
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStats(FStatInfo StatInfo);

	UFUNCTION(BlueprintImplementableEvent)
		void ResetCamera(FRotator CurrentCameraRotation, FRotator OriginalCameraRotation);

	UFUNCTION(BlueprintImplementableEvent)
		void GetWithinStrikingDistance(FVector ActorLocation, FVector DesiredLocation, float StrikingDistance);

	UFUNCTION(BlueprintCallable)
		/** Sets whether or not the player can move */
	void EnableMovement(bool bCanMove);

	UFUNCTION(BlueprintCallable)
		/** Sets whether the attack is saved or not. 
		* @param bool bIsSaved - To save the attack or not
		*/
	void SaveAttack(bool bIsSaved);

	UFUNCTION(BlueprintCallable)
		/** Gets whether the attack is saved or not. 
		* @return True if the attack is saved, false if not.
		*/
		bool GetSaveAttack() const;

	UFUNCTION(BlueprintCallable)
		/** Resets the combo counter. */
		void ResetCombo();

	UFUNCTION(BlueprintCallable)
		/** Stops the roll and allows for player input */
		void StopRoll();

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

	/** Rotates to face the enemy */
	void FaceEnemy();

	/** Move incrementally towards the enemy */
	void InchTowardsEnemy();

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

	/** Find the closest enemy to the left 
	* @param Target - The currently locked on target.
	* @return - The enemy to the left of the target.
	*/
	AEnemyCharacter* FindClosestEnemyToTheLeftOfTarget();

	/** Find the closest enemy to the right
	* @param Target - The currently locked on target.
	* @return - The enemy to the right of the target.
	*/
	AEnemyCharacter* FindClosestEnemyToTheRightOfTarget();

	/** Gets the vector to the given enemy 
	* @param Enemy - The enemy to calculate a vector to.
	* @return A vector towards the enemy
	*/
	FVector GetVectorTo(AEnemyCharacter* Enemy);

	/** Gets the angle to the enemy from the player's forward facing direction. 
	* @param Enemy - The enemy to calculate the angle for.
	* @param ForwardDirection - The direction acting as 0.
	* @return The angle towards the enemy.
	*/
	float GetAngleTo(AEnemyCharacter* Enemy, FVector ForwardDirection);

	/** Checks to see if the given enemy is infront of the player 
	* @param ToEnemy - The enemy to get the vector to.
	* @return true if enemy is infront and false if not.
	*/
	bool IsInFront(AEnemyCharacter* Enemy);

	/** Tracks the currently locked on enemy. */
	void TrackLockedOnEnemy(float DeltaTime);

	/** Checks to see if the player is in view */
	bool PlayerInView();

	/** Adjusts the camera boom so that the player stays in view. */
	void AdjustCameraBoomToSeePlayerAndEnemy(float DeltaTime);

	/** Switches lock on target to new target. */
	void SwitchTargetTo(AEnemyCharacter* NewTarget);

	/** Sets whether or not the player is in combat 
	* @param bInCombat - the value for whether player is in combat or not. 
	*/
	void SetIsInCombat(bool bInCombat);

	// Used to see if the character is in combat.
	bool bIsInCombat;

	// Used to see if the character is locked onto an enemy.
	bool bIsLockedOntoEnemy;

	// Used to see if the player can move.
	bool bMovementEnabled;

	// Used to see if the player can initiate the next attack or not.
	bool bSaveAttack;

	// Used to see if the player has initiated a roll.
	bool bIsRolling;

	// Used to see which combo attack the player is currently on.
	int32 ComboCounter;
	
	// Used to tell the UI which button is pressed.
	TMap<FString, int32> UIButtons;

	// Keeps track of player stats.
	UCharacterDataComponent* HeroData;

	// Used to initialize hero data.
	UPROPERTY(EditAnywhere, Category = "Stats")
		UCharacterInfoDataAsset* HeroDataAsset;

	// Used to see if the player is in lunging distance.
	UPROPERTY(EditAnywhere, Category = "Combat")
		float LungingDistance;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float DistancePlayerCanHitEnemy;

	// Used to reset the camera after locking on.
	FRotator DefaultCameraRotation;

	// Used to track which enemy is the closest and infront of the player
	AEnemyCharacter* ClosestEnemyInFront;

	// Used to track the lock on target.
	AEnemyCharacter* LockOnTarget;

	// The animation blueprint.
	UAnimInstance* AnimInstance;

	// Montage animations
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimMontage*> ComboAttacks;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DodgeMontage;
};

