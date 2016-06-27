// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DefyingGravityObject.generated.h"


UCLASS()
class DEFYINGGRAVITY_API ADefyingGravityObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefyingGravityObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// return the sprite for the object
	FORCEINLINE class UPaperSpriteComponent* GetS() const { return Sprite; };

	

	// in case the object is gravity = 0 and normal down gravity
	UFUNCTION(BlueprintCallable, Category = "Object")
		void WasDefied();
	UFUNCTION(BlueprintCallable, Category = "Object")
		void WasNotDefied();
	UFUNCTION(BlueprintCallable, Category = "Object")
		bool isDefying();
	UFUNCTION(BlueprintCallable, Category = "Object")
		void SetDefy(bool NewBlockState);
	
	// function to handle change of gravity directions (Down(default) up left right)
	UFUNCTION(BlueprintCallable, Category = "Object")
		void SetGravityDir(FString gravityDir);






protected:
	// boolean for check now is Defying or not
	bool bDefy;
	// Z lenght for moving the object
	float moveZ;
	// old location of the object before was defied
	FVector oldLocation;
	// first velocity after being defied
	FVector firstVelocity;
	FString gravityDir;
	bool changingGravityDir;
	int seconds;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* Sprite;




};
