// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DefyingGravity.h"
#include "DefyingGravityGameMode.h"
#include "MySaveGame.h"
#include "DefyingGravityCharacter.h"



ADefyingGravityGameMode::ADefyingGravityGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = ADefyingGravityCharacter::StaticClass();	
	inStage = 0;
}
void ADefyingGravityGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
	setCurrentState(DGPlayState::EMainMenu);

	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	inStage = LoadGameInstance->StagePlayerIn;
	if (GEngine)
	{
		FString stringStage = FString::FromInt(inStage);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Load Game! in stage " + stringStage);
	}

}
void ADefyingGravityGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADefyingGravityGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}


	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			//if (getCurrentState() == DGPlayState::EMainMenu) {
				CurrentWidget->AddToViewport();
			//	UGameplayStatics::OpenLevel(GetWorld(), "EmptyMap");
			//}
		
				
		
		}
	}

}
DGPlayState ADefyingGravityGameMode::getCurrentState() const
{
	return currentState;
}
void ADefyingGravityGameMode::setCurrentState(DGPlayState newState)
{
	currentState = newState;
}
int32 ADefyingGravityGameMode::getInstage()
{
	return inStage;
}
void ADefyingGravityGameMode::setInstage(int32 newstage) {
	inStage = newstage;

}