// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DefyingGravity.h"
#include "DefyingGravityCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "DefyingGravityObject.h"
#include "DefyingGravityEndingStageDoor.h"




DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);
//////////////////////////////////////////////////////////////////////////
// ADefyingGravityCharacter

ADefyingGravityCharacter::ADefyingGravityCharacter()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> catWalkAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> idleCatAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> JumpCatAnimationAsset;


		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/RunningAnimation.RunningAnimation"))
			, IdleAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/IdleAnimation.IdleAnimation"))
			, catWalkAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/catWalkAnimation.catWalkAnimation"))
			, idleCatAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/idleCatAnimation.idleCatAnimation"))
			, JumpCatAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/JumpCatAnimation.JumpCatAnimation"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	catWalkAnimation = ConstructorStatics.catWalkAnimationAsset.Get();
	idleCatAnimation = ConstructorStatics.idleCatAnimationAsset.Get();
	JumpCatAnimation = ConstructorStatics.JumpCatAnimationAsset.Get();
	GetSprite()->SetFlipbook(idleCatAnimation);

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
// 	TextComponent->AttachTo(RootComponent);

	// Create the collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
	bPast = false;
	bChangeMap = false;
	gravityDir = "down";
	bActive = false;
	maxPower = 100.0f;
	power = maxPower;
	decayrate = 1.0f;
	readyJump = false;
	
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ADefyingGravityCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? catWalkAnimation : idleCatAnimation;
	if (GetCharacterMovement()->IsFalling())
	{
		DesiredAnimation = JumpCatAnimation;
	}
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void ADefyingGravityCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bActive) {

		UpdateCharacter();
		// Get all overlapping Actors and store them in an array
		TArray<AActor*> CollectedActors;
		CollectionSphere->GetOverlappingActors(CollectedActors);
		// for each Actor we collected
		for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
		{
			// Cast the actor to ADefyingGravityEndingStageDoor
			ADefyingGravityEndingStageDoor* const testDoor = Cast<ADefyingGravityEndingStageDoor>(CollectedActors[iCollected]);
			// if the cast is successful and the Door is valid and active
			if (testDoor && testDoor->isActive())
			{
				testDoor->endStage();
			}
		}



		FVector Located = GetActorLocation();

		if (isChangingMap()) {

			// check what map we're going to change to play
			if (isPast()) {

				//origin of past map
				FVector pastMap = FVector(Located.X, 0.0f, 5150.0f); // Z up to the ground
				SetActorLocation(pastMap, false, nullptr, ETeleportType::None);

			}
			else {
				FVector Located = GetActorLocation();
				// origin of default map
				FVector defMap = FVector(Located.X, 0.0f, 150.0f);
				SetActorLocation(defMap, false, nullptr, ETeleportType::None);


			}
			setbChangeMap(false);
		}

		if (isPast()) {
			power = power - decayrate;
		}
	}

	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ADefyingGravityCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &ADefyingGravityCharacter::MoveRight);
	InputComponent->BindAction("Defy", IE_Pressed, this, &ADefyingGravityCharacter::changeGravity);
	InputComponent->BindAction("changeTime", IE_Pressed, this, &ADefyingGravityCharacter::changeTime);
	InputComponent->BindAction("upGravity", IE_Pressed, this, &ADefyingGravityCharacter::upGravity);
	InputComponent->BindAction("downGravity", IE_Pressed, this, &ADefyingGravityCharacter::downGravity);
	InputComponent->BindAction("leftGravity", IE_Pressed, this, &ADefyingGravityCharacter::leftGravity);
	InputComponent->BindAction("rightGravity", IE_Pressed, this, &ADefyingGravityCharacter::rightGravity);
	InputComponent->BindTouch(IE_Pressed, this, &ADefyingGravityCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ADefyingGravityCharacter::TouchStopped);
}


void ADefyingGravityCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	if (bActive) {
		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void ADefyingGravityCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{

	// jump on any touch
	Jump();
	

}

void ADefyingGravityCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();

}

void ADefyingGravityCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

// defy/undefy -> undefy/defy
void ADefyingGravityCharacter::changeGravity()
{
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);
	// for each Actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		// Cast the actor to ABlock
		ADefyingGravityObject* const testBlock = Cast<ADefyingGravityObject>(CollectedActors[iCollected]);
		// if the cast is successful and the pickup is valid and active
		if (testBlock)
		{
			// change defy block or not defy block WasCollected function
			if (testBlock->isDefying())
			{
				testBlock->SetDefy(false);
				testBlock->WasNotDefied();

			}
			else
			{
				testBlock->SetDefy(true);
				testBlock->WasDefied();
			}


			//TestPickup->SetActive(false);
		}
	}

}

void ADefyingGravityCharacter::changeTime() {
	// check if current map is Past
	if (isPast()) {
		// Past will change to be default map
		setPast(false);
		UE_LOG(LogClass, Log, TEXT("SET TO DEFAULT"))
	}
	else
	{	// not Past will change to be Past map
		setPast(true);
		UE_LOG(LogClass, Log, TEXT("SET TO PAST"))
	}
	// now it's changing
	setbChangeMap(true);

}
void ADefyingGravityCharacter::setPast(bool bSetTime) {

	bPast = bSetTime;

}
bool ADefyingGravityCharacter::isPast() {

	return bPast;

}
void ADefyingGravityCharacter::setbChangeMap(bool bSetChange) {

	bChangeMap = bSetChange;

}
bool ADefyingGravityCharacter::isChangingMap() {

	return bChangeMap;

}

void ADefyingGravityCharacter::setGravityDir(FString newDir) {
	UE_LOG(LogClass, Log, TEXT("gravityDir = %s and newDir = %s"),*gravityDir,*newDir)
		if (gravityDir != newDir) {
			if (power >= 30) {
				gravityDir = newDir;
				power = power - 30;
				// Get all overlapping Actors and store them in an array
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

				for (AActor* TActor : FoundActors)
				{
					ADefyingGravityObject* MyActor = Cast<ADefyingGravityObject>(TActor);

					if (MyActor != nullptr) {
						MyActor->SetGravityDir(newDir);

					}
				}
			}
		}
}

FString  ADefyingGravityCharacter::getGravityDir() {
	return gravityDir;
}

void ADefyingGravityCharacter::upGravity() {
	setGravityDir("up");
}
void ADefyingGravityCharacter::downGravity() {
	setGravityDir("down");
}
void ADefyingGravityCharacter::leftGravity() {
	setGravityDir("left");
}
void ADefyingGravityCharacter::rightGravity() {
	setGravityDir("right");
}
void ADefyingGravityCharacter::getActiveFromGameMode(bool bAct)
{
	bActive = bAct;
}

float ADefyingGravityCharacter::getMaxPower() {

	return maxPower;

}
float ADefyingGravityCharacter::getPower() {

	return power;

}