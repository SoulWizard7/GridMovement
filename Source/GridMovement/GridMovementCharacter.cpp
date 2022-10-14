// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridMovementCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AIController.h"
#include "GridMovementPlayerController.h"
#include "GameFramework/Pawn.h"

AGridMovementCharacter::AGridMovementCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGridMovementCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AGridMovementCharacter::BeginPlay()
{
	Super::BeginPlay();
	AGridMovementPlayerController* GridMovementPlayerController = Cast<AGridMovementPlayerController>(GetController());
	if(GridMovementPlayerController == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "Could not cast Controller to GridMovementPlayerController", true, FVector2D(1.f));
	}
	else
	{
		//GridMovementPlayerController->Char = this;
		//GridMovementPlayerController->PathPointCheckComponent = FindComponentByClass<UGMPathPointCheckComponent>();
	}
}

void AGridMovementCharacter::DebugCover()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current NorthHalfCover: %hs"), CurrentCover.NorthHalfCover ? ("true") : ("false")), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current EastHalfCover: %hs"), CurrentCover.EastHalfCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current SouthHalfCover: %hs"), CurrentCover.SouthHalfCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current WestHalfCover: %hs"), CurrentCover.WestHalfCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current NorthFullCover: %hs"), CurrentCover.NorthFullCover ? ("true") : ("false")), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current EastFullCover: %hs"), CurrentCover.EastFullCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current SouthFullCover: %hs"), CurrentCover.SouthFullCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current WestFullCover: %hs"), CurrentCover.WestFullCover ? "true" : "false"), false, FVector2D(1.f));
}

