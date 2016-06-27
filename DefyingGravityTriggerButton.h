// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteActor.h"
#include "DefyingGravityTriggerButton.generated.h"

UCLASS()
class DEFYINGGRAVITY_API ADefyingGravityTriggerButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefyingGravityTriggerButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	FORCEINLINE class UPaperSpriteComponent* GetS() const { return Sprite; };

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* Sprite;
	
	
};
