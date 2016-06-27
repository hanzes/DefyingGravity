// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "DefyingGravityGameMode.generated.h"



// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of DefyingGravityCharacter

UENUM(BlueprintType)
enum class DGPlayState
{
	EMainMenu,
	EPlaying,
	EGameOver,
	EWon,
	EUnknown

};

UCLASS(minimalapi)
class ADefyingGravityGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADefyingGravityGameMode();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	/** Return the current playing state */
	UFUNCTION(BlueprintPure, Category = "State")
	DGPlayState getCurrentState() const;

	/** Set a new playing state*/
	UFUNCTION(BlueprintCallable, Category = "State")
	void setCurrentState(DGPlayState newState);

	int32 getInstage();
	void setInstage(int32 newstage);

protected:



	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	UUserWidget* CurrentWidget;

	int32 inStage;


private:
	/** Keep track of the current playing state */
	DGPlayState currentState;



};
