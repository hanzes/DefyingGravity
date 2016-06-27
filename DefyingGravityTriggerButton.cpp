// Fill out your copyright notice in the Description page of Project Settings.

#include "DefyingGravity.h"
#include "DefyingGravityTriggerButton.h"


// Sets default values
ADefyingGravityTriggerButton::ADefyingGravityTriggerButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
}

// Called when the game starts or when spawned
void ADefyingGravityTriggerButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefyingGravityTriggerButton::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


