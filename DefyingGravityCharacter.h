// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
#include "DefyingGravityCharacter.generated.h"

// This class is the default character for DefyingGravity, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals

class UTextRenderComponent;

UCLASS(config=Game)
class ADefyingGravityCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	/** Collection Sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Block", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* catWalkAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* idleCatAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpCatAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	bool bPast;
	bool bChangeMap;
	bool bActive;
	FString gravityDir;

public:
	ADefyingGravityCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Return CollectionSphere subobject*/
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }

	/**  called when we press a key to collect any pickups inside the CollectionSphere*/
	UFUNCTION(BlueprintCallable, Category = "Block")
		void changeGravity();
	UFUNCTION(BlueprintCallable, Category = "Time")
		void changeTime();
	UFUNCTION(BlueprintCallable, Category = "Time")
		void setPast(bool bPast);
	UFUNCTION(BlueprintPure, Category = "Time")
		bool isPast();
	UFUNCTION(BlueprintPure, Category = "Power")
	float getMaxPower();
	UFUNCTION(BlueprintPure, Category = "Power")
	float getPower();

	bool isChangingMap();
	void setbChangeMap(bool bChangeMap);
	void setGravityDir(FString gravityDir);
	FString getGravityDir();
	void upGravity();
	void downGravity();
	void leftGravity();
	void rightGravity();
	UFUNCTION(BlueprintCallable, Category = "State")
	void getActiveFromGameMode(bool bActive);
	float power;
	float maxPower;
	float decayrate;
	bool readyJump;
};
