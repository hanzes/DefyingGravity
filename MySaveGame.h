// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DEFYINGGRAVITY_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 StagePlayerIn;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UMySaveGame();

	
	
};
