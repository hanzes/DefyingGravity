// Fill out your copyright notice in the Description page of Project Settings.

#include "DefyingGravity.h"
#include "DefyingGravityGameMode.h"
#include "DefyingGravityEndingStageDoor.h"
#include "MySaveGame.h"


// Sets default values
ADefyingGravityEndingStageDoor::ADefyingGravityEndingStageDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	inStage = 0;
	stagesLocation.Add(FVector(300.f, 0.0f, 188.0f));
	stagesLocation.Add(FVector(8000.f, 0.0f, 50.0f));
	doorActive = true;
}

// Called when the game starts or when spawned
void ADefyingGravityEndingStageDoor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ADefyingGravityEndingStageDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


}

void ADefyingGravityEndingStageDoor::endStage (){
	//save game = depends on which stages player's going to end occur 2 cases : 1. end with +1 stage from save 2. end with equal or lower stages from save 
	ADefyingGravityGameMode* myGameMode = Cast<ADefyingGravityGameMode>(GetWorld()->GetAuthGameMode());
	if (myGameMode) {

		inStage = myGameMode->getInstage();
		inStage = inStage + 1;
		myGameMode->setInstage(inStage);
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		SaveGameInstance->StagePlayerIn = inStage;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);


		UE_LOG(LogClass, Log, TEXT("Save and now the player is in stage %d"), inStage)
		doorActive = false;

	}
}

bool  ADefyingGravityEndingStageDoor::isActive() {
	return doorActive;
}

