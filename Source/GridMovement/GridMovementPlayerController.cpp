// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridMovementPlayerController.h"

#include "AIController.h"
#include "GMCoverIconDisplay.h"
#include "GMPathPointCheckComponent.h"
#include "GMWalkableSurfaceComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GridMovementCharacter.h"
#include "NavigationSystem.h"
#include "ToolBuilderUtil.h"

AGridMovementPlayerController::AGridMovementPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AGridMovementPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PathPointCheckComponent = FindComponentByClass<UGMPathPointCheckComponent>();
}

void AGridMovementPlayerController::DebugStuff()
{
	Char->DebugCover();	
}

void AGridMovementPlayerController::LeftMouseButton()
{
}

void AGridMovementPlayerController::RightMouseButton()
{
}

void AGridMovementPlayerController::CheckCurrentMousePosition(bool MouseGridPositionIsValid)
{
	if(MouseGridPositionIsValid)
	{
		if(hasNewPosition)
		{
			hasNewPosition = false;			

			ACharacter* C = Cast<AGridMovementCharacter>(Char);
			PathToCurrentMouseGridPosition = PathPointCheckComponent->FindPathToLocation(CurrentMouseGridPosition, C);

			if(PathToCurrentMouseGridPosition.Num() >= 1)
			{
				if(PathPointCheckComponent->CheckIfMouseGridPositionIsValid(CurrentMouseGridPosition))
				{				
					PathPointCheckComponent->CheckGridPositionForCover(CurrentMouseGridPosition);
					Char->DisplayCoverIconsBlueprintEvent(PathPointCheckComponent->CurrentMousePositionCover, CurrentMouseGridPosition);
					CanMoveToPosition = true;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::White, "Not valid mouse position2", true, FVector2D(1.f));
					PathPointCheckComponent->DeactivateAllCoverBools();
					Char->DisplayCoverIconsBlueprintEvent(PathPointCheckComponent->CurrentMousePositionCover, CurrentMouseGridPosition);
					return;
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::White, "Not valid mouse position1", true, FVector2D(1.f));
				PathPointCheckComponent->DeactivateAllCoverBools();
				Char->DisplayCoverIconsBlueprintEvent(PathPointCheckComponent->CurrentMousePositionCover, CurrentMouseGridPosition);				
				return;
			}			
		}			
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, FString::Printf(TEXT("NorthHalfCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.NorthHalfCover ? ("true") : ("false")), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, FString::Printf(TEXT("EastHalfCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.EastHalfCover ? "true" : "false"), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, FString::Printf(TEXT("SouthHalfCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.SouthHalfCover ? "true" : "false"), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, FString::Printf(TEXT("WestHalfCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.WestHalfCover ? "true" : "false"), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Orange, FString::Printf(TEXT("NorthFullCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.NorthFullCover ? ("true") : ("false")), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Orange, FString::Printf(TEXT("EastFullCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.EastFullCover ? "true" : "false"), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Orange, FString::Printf(TEXT("SouthFullCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.SouthFullCover ? "true" : "false"), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Orange, FString::Printf(TEXT("WestFullCover: %hs"), PathPointCheckComponent->CurrentMousePositionCover.WestFullCover ? "true" : "false"), false, FVector2D(1.f));
	}
	else
	{
		CanMoveToPosition = false;
	}
}

bool AGridMovementPlayerController::GetCurrentMouseGridPosition()
{
	//I have stacked checks here, to be able to use GetHitResultUnderCursor helper function from Controller
	
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	CurrentMouseGridPosition = Hit.Location;	

	//Check if mousepos is Walkable
	
	if(Hit.GetActor())
	{		
		if(!Hit.GetActor()->FindComponentByClass<UGMWalkableSurfaceComponent>())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, "MousePosition is not valid", true, FVector2D(1.f));
			return false;
		}
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, "Actor has WalkComponent ", true, FVector2D(1.f));		
	}

	//Check if mousepos normal is valid

	float normalZ = abs(Hit.Normal.Z);
	if(normalZ < 0.9f) return false;

	//Round the mousepos to 1m

	float x = CurrentMouseGridPosition.X / 100;
	float y = CurrentMouseGridPosition.Y / 100;
	x = round(x);
	y = round(y);
	CurrentMouseGridPosition.X = x * 100;
	CurrentMouseGridPosition.Y = y * 100;	

	FString msg1 = CurrentMouseGridPosition.ToString();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, msg1, true, FVector2D(1.f));
	
	if (CurrentMouseGridPosition.Size2D() != LastMouseGridPosition.Size2D())
	{
		LastMouseGridPosition = CurrentMouseGridPosition;
		hasNewPosition = true;		
	}	
	return true;
}

void AGridMovementPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//MouseGridPositionIsValid = GetCurrentMouseGridPosition();

	CheckCurrentMousePosition(GetCurrentMouseGridPosition());

	if(bInputPressed)
	{
		FollowTime += DeltaTime;
	}
	else
	{
		FollowTime = 0.f;
	}	
}

void AGridMovementPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AGridMovementPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AGridMovementPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("DebugCover", IE_Pressed, this, &AGridMovementPlayerController::DebugStuff);
}

void AGridMovementPlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AGridMovementPlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if(FollowTime <= ShortPressThreshold)
	{		
		if(PathPointCheckComponent)
		{
			if(CanMoveToPosition)
			{
				Char->CurrentCover = PathPointCheckComponent->CurrentMousePositionCover;
				Char->MoveToLocation(CurrentMouseGridPosition);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CurrentMouseGridPosition, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
			}
		}		
	}
}
