// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteActor.h"
#include "DefyingGravityEndingStageDoor.generated.h"

UCLASS()
class DEFYINGGRAVITY_API ADefyingGravityEndingStageDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefyingGravityEndingStageDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Stage")
	void endStage();

	FORCEINLINE class UPaperSpriteComponent* GetS() const { return Sprite; };

	bool isActive();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* Sprite;

	TArray<FVector> stagesLocation;
	int32 inStage;
	bool doorActive;
};
