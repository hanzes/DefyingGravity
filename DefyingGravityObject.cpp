// Fill out your copyright notice in the Description page of Project Settings.

#include "DefyingGravity.h"
#include "DefyingGravityObject.h"


// Sets default values
ADefyingGravityObject::ADefyingGravityObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bDefy = false;
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	GetS()->SetSimulatePhysics(true);
	GetS()->SetEnableGravity(true);
	bCanBeDamaged = false;
	moveZ = 0.0f;
	firstVelocity = FVector(0.0f,0.0f,0.0f);
	changingGravityDir = false;
	gravityDir = "down";
	seconds = 0;


}

// Called when the game starts or when spawned
void ADefyingGravityObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefyingGravityObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//check if moveZ was declared lenght yet? 
	if (moveZ > 0.0f) {
		FVector newLocation = GetActorLocation();
		FString BlockDebugString = GetName();
		if ((newLocation.Z - oldLocation.Z) > 50.0f && (newLocation.Z - oldLocation.Z) < 52.0f) {
			firstVelocity = GetS()->GetPhysicsLinearVelocity();
		}
		UE_LOG(LogClass, Log, TEXT("%s %f"), *BlockDebugString, moveZ)
			// check if new location is more than old location or not (to lower the velocity)
			if ((newLocation.Z - oldLocation.Z) > moveZ) {
				FVector floatingVelo = GetS()->GetPhysicsLinearVelocity();
				UE_LOG(LogClass, Log, TEXT("floating velocity %s %f"), *BlockDebugString, floatingVelo.Z)
					//check if the object has same verocity (velo on Z has to be decreased up to time)
					if (floatingVelo.X == firstVelocity.X && floatingVelo.Z <= firstVelocity.Z) {
						//UE_LOG(LogClass, Log, TEXT("floating velocity %s %f"), *BlockDebugString, floatingVelo.Z)
						if (moveZ == 250.0f) {

							GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 10.0f), false, "NAME_None");
							UE_LOG(LogClass, Log, TEXT("You set velocity to 20.0f %s"), *BlockDebugString)
								moveZ = 30.0f;
							firstVelocity.Z = 10.0f;
							oldLocation = GetActorLocation();
							UE_LOG(LogClass, Log, TEXT("Location z of %s is %f and velo is %f"), *BlockDebugString, oldLocation.Z, firstVelocity.Z)
						}
						else if (moveZ == 30.0f) {
							GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 3.0f), false, "NAME_None");
							UE_LOG(LogClass, Log, TEXT("You set velocity to 0.0f %s"), *BlockDebugString)
								moveZ = 0.0f;
						}
						else {
							UE_LOG(LogClass, Log, TEXT("Nothing happen %s"), *BlockDebugString)
						}

					}
					else {

						moveZ = 0.0f;
					}
			}

	}

	// check if gravity dir is changing
	/*FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
	if (checkVelocity != FVector(0.0f, 0.0f, 0.0f)) {
		UE_LOG(LogClass, Log, TEXT("moving"))
	}
	else {
		UE_LOG(LogClass, Log, TEXT("steady"))
	}*/
	if (changingGravityDir) {
		//int seconds = 0;
		if (gravityDir == "up") {
			FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
			if (checkVelocity.Z <= 0) {
				++seconds;
				UE_LOG(LogClass, Log, TEXT("seconds= %d"), seconds)
					if (seconds == 9) {
						UE_LOG(LogClass, Log, TEXT("bounce"))
						GetS()->AddForce(FVector(0.0f, 0.0f, 2500000.0f), "NAME_None", false);
					}

					if (seconds >= 12) {
						UE_LOG(LogClass, Log, TEXT("stop"))
						GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false, "NAME_None");
						changingGravityDir = false;
						seconds = 0;
						
					}
					

			}
			else {
				float Acceleration = 9.8f;
				FVector floatingVelo = GetS()->GetPhysicsLinearVelocity();
				GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, floatingVelo.Z + Acceleration), false, "NAME_None");
			}


		}
		if (gravityDir == "left") {
			FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
			//UE_LOG(LogClass, Log, TEXT("x checkvelocity= %f"), checkVelocity.X)
			if (checkVelocity.X >= 0) {
				++seconds;
				UE_LOG(LogClass, Log, TEXT("seconds= %d"), seconds)
					if (seconds == 9) {
						UE_LOG(LogClass, Log, TEXT("bounce"))
							GetS()->AddForce(FVector(-2500000.0f, 0.0f, 0.0f), "NAME_None", false);


					}
				if (seconds == 12) {
					UE_LOG(LogClass, Log, TEXT("stop"))
						GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false, "NAME_None");
					changingGravityDir = false;
					seconds = 0;
				}

			}
			else {
				float Acceleration = -9.8f; // left is -x
				FVector floatingVelo = GetS()->GetPhysicsLinearVelocity();
				//UE_LOG(LogClass, Log, TEXT("x floating velocity= %f"), floatingVelo.X)
				GetS()->SetPhysicsLinearVelocity(FVector(floatingVelo.X + Acceleration, 0.0f, 0.0f), false, "NAME_None");
			}


		}
		if (gravityDir == "right") {
			FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
			//UE_LOG(LogClass, Log, TEXT("x checkvelocity= %f"), checkVelocity.X)
			if (checkVelocity.X <= 0) {
				++seconds;
				UE_LOG(LogClass, Log, TEXT("seconds= %d"), seconds)
					if (seconds == 9) {
						UE_LOG(LogClass, Log, TEXT("bounce"))
						GetS()->AddForce(FVector(2500000.0f, 0.0f, 0.0f), "NAME_None", false);


					}
				if (seconds == 12) {
					UE_LOG(LogClass, Log, TEXT("stop"))
					GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false, "NAME_None");
					changingGravityDir = false;
					seconds = 0;
				}

			}
			else {
				float Acceleration = 9.8f; // left is -x
				FVector floatingVelo = GetS()->GetPhysicsLinearVelocity();
				//UE_LOG(LogClass, Log, TEXT("x floating velocity= %f"), floatingVelo.X)
				GetS()->SetPhysicsLinearVelocity(FVector(floatingVelo.X + Acceleration, 0.0f, 0.0f), false, "NAME_None");
			}


		}
	}

}

void ADefyingGravityObject::WasDefied()
{
	//Log a debug message
	FString BlockDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You defy %s"), *BlockDebugString)
	GetS()->SetEnableGravity(false);
	oldLocation = GetActorLocation();
	GetS()->AddForce(FVector(0.0f, 0.0f, 600000.0f), "NAME_None", false);
	moveZ = 250.0f;
	UE_LOG(LogClass, Log, TEXT("Add force %s by Z is on = %f"), *BlockDebugString, oldLocation.Z)

}
void ADefyingGravityObject::WasNotDefied()
{
	FString BlockDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You cancel defying %s"), *BlockDebugString)
	GetS()->SetEnableGravity(true);



}

bool ADefyingGravityObject::isDefying()
{

	return bDefy;
}
void ADefyingGravityObject::SetDefy(bool NewBlockState)
{
	bDefy = NewBlockState;
}
// set gravity Dir up down left right BTW can change even the object was defied we'll handle it by cancel defying the object after that we'll change gravity dir
void  ADefyingGravityObject::SetGravityDir(FString Direction) {
	if (Direction == "up") {
		// check if old velocity of the sprite is > 0
		FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
		if (checkVelocity != FVector(0.0f, 0.0f, 0.0f)) {
			GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false, "NAME_None");
		}
		gravityDir = Direction;
		FString BlockDebugString = GetName();
		UE_LOG(LogClass, Log, TEXT("You set gravity to upside with %s"), *BlockDebugString)
		GetS()->SetEnableGravity(false);
		GetS()->AddForce(FVector(0.0f, 0.0f,1000000.0f), "NAME_None", false);
		UE_LOG(LogClass, Log, TEXT("Add force to %s"), *BlockDebugString)
		changingGravityDir = true;

	}
	if (Direction == "down") {
		gravityDir = Direction;
		FString BlockDebugString = GetName();
		UE_LOG(LogClass, Log, TEXT("You set gravity to downside with %s"), *BlockDebugString)
		GetS()->SetEnableGravity(true);


	}
	if (Direction == "left") {
		// check if old velocity of the sprite is > 0
		FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
		if (checkVelocity != FVector(0.0f, 0.0f, 0.0f)) {
			GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false, "NAME_None");
		}
		gravityDir = Direction;
		FString BlockDebugString = GetName();
		UE_LOG(LogClass, Log, TEXT("You set gravity to leftside with %s"), *BlockDebugString)
		GetS()->SetEnableGravity(false);
		GetS()->AddForce(FVector(0.0f, 0.0f, 1000000.0f), "NAME_None", false);
		GetS()->AddForce(FVector(-2000000.0f, 0.0f, 0.0f), "NAME_None", false);
		UE_LOG(LogClass, Log, TEXT("Add force to %s"), *BlockDebugString)
		changingGravityDir = true;

	}
	if (Direction == "right") {
		// check if old velocity of the sprite is > 0
		FVector checkVelocity = GetS()->GetPhysicsLinearVelocity();
		if (checkVelocity != FVector(0.0f, 0.0f, 0.0f)) {
			GetS()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false, "NAME_None");
		}
		gravityDir = Direction;
		FString BlockDebugString = GetName();
		UE_LOG(LogClass, Log, TEXT("You set gravity to rightside with %s"), *BlockDebugString)
		GetS()->SetEnableGravity(false);
		GetS()->AddForce(FVector(0.0f, 0.0f, 1000000.0f), "NAME_None", false);
		GetS()->AddForce(FVector(2000000.0f, 0.0f, 0.0f), "NAME_None", false);
		UE_LOG(LogClass, Log, TEXT("Add force to %s"), *BlockDebugString)
		changingGravityDir = true;

	}

}

