// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridMovementPlayerController.h"
#include "AIController.h"
#include "GMCoverIconDisplay.h"
#include "GMGridPositionCoverCheckComponent.h"
#include "GMWalkableSurfaceComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Combat/GMCombatManager.h"
#include "UnitComponents/GMUnitGroundMarkerComponent.h"

AGridMovementPlayerController::AGridMovementPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	UnitSelectorComponent = CreateDefaultSubobject<UGMUnitSelectorComponent>("UnitSelector");
}

void AGridMovementPlayerController::BeginPlay()
{
	//Super::BeginPlay();

	bShowMouseCursor = true;

	UnitSelectorComponent->GridMovementPlayerController = this;
	
	GridPositionCoverCheckComponent = FindComponentByClass<UGMGridPositionCoverCheckComponent>();
	CoverIconDisplay = Cast<AGMCoverIconDisplay>(GetWorld()->SpawnActor(CoverIconDisplayBlueprint));
	CameraController = Cast<AGMCameraContoller>(GetWorld()-> SpawnActor(CameraControllerBlueprint));
	CameraController->SetCameraPositionAtStart();

	
	//CameraController->CenterOnUnit()
}

void AGridMovementPlayerController::TryMoveUnit()
{
	if(CombatIsOn && CurrentUnit->CurrentMovementUnits == 0) return;
	
	if(IsCurrentMouseGridPositionValidMovementPosition(CurrentMousePositionHitResult))
	{
		ACharacter* C = Cast<AGMUnit>(CurrentUnit);
		PathToCurrentMouseGridPosition = GridPositionCoverCheckComponent->FindPathToLocation(CurrentMouseGridPosition, C);

		if (PathToCurrentMouseGridPosition.Num() >= 1)
		{
			DisableInput(this);
			CurrentUnit->UnitIsBusy(false);
			CurrentUnit->CurrentCover = GridPositionCoverCheckComponent->CurrentMousePositionCover;
			CurrentUnit->CurrentMovementUnits = 0;
			CurrentUnit->MoveToLocation(CurrentMouseGridPosition);			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CurrentMouseGridPosition, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}
}

void AGridMovementPlayerController::StartCombat()
{
	if(!CombatManager)
	{
		CombatManager = Cast<AGMCombatManager>(GetWorld()->SpawnActor(CombatManagerBluePrint));
		CombatManager->GridMovementPlayerController = this;
		CombatIsOn = true;
	}	
}

void AGridMovementPlayerController::EndCombat()
{
	CombatManager->DeSpawnButtons();
	GetWorld()->DestroyActor(CombatManager);
	CombatManager = nullptr;
	CombatIsOn = false;
}

void AGridMovementPlayerController::CombatManagerEndPlayerTurn()
{
	if(CombatManager)
	{
		CombatManager->EndPlayerTurn();
	}	
}

void AGridMovementPlayerController::TryAttackUnit()
{
	if(!CurrentUnit->hasAction) return;
	
	if(IsValid(CurrentHoveredUnit))
	{
		if(CurrentHoveredUnit->IsEnemy)
		{
			CurrentUnit->UnitIsBusy(true);
			CurrentUnit->AttackUnit(CurrentHoveredUnit);
		}
	}
}

void AGridMovementPlayerController::MarkUnitsOnHover()
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, "MarkUnitsOnHover", true, FVector2D(1.f));	

	if (CurrentMousePositionHitResult.GetActor())
	{
		if(CurrentHoveredUnit == CurrentMousePositionHitResult.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, "MarkUnitsOnHover return", true, FVector2D(1.f));
			return;
		}
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Yellow, "Current Hovered Unit is: " + CurrentMousePositionHitResult.GetActor()->GetName(), true, FVector2D(1.f));
		
		if(CurrentMousePositionHitResult.GetActor()->IsA<AGMUnit>())
		{
			CurrentHoveredUnit = Cast<AGMUnit>(CurrentMousePositionHitResult.GetActor());
			
			if(CurrentHoveredUnit->IsEnemy)
			{				
				CurrentHoveredUnit->UnitGroundMarkerController->ActivateDecalHoverToAttack();
			}
			else
			{
				if(CurrentUnit != CurrentHoveredUnit)
				{
					CurrentHoveredUnit->UnitGroundMarkerController->ActivateDecalHoverToSelect();
				}
			}
		}
		else
		{
			CurrentHoveredUnit = nullptr;
		}
	}	
}

void AGridMovementPlayerController::DebugStuff()
{
	if(IsValid(CurrentUnit))
	{
		CurrentUnit->DebugCover();
	}
}

void AGridMovementPlayerController::EnablePlayerInput()
{		
	EnableInput(this);
}

void AGridMovementPlayerController::LeftMouseButton()
{
	SelectUnit();
}

void AGridMovementPlayerController::RightMouseButton()
{
	if(IsValid(CurrentHoveredUnit))
	{
		TryAttackUnit();
	}
	if(IsValid(CurrentUnit))
	{
		TryMoveUnit();
	}	
}

void AGridMovementPlayerController::SelectUnit()
{
	if(CombatIsOn)
	{
		UnitSelectorComponent->TryGetUnitCombat();
	}
	else
	{
		UnitSelectorComponent->TryGetUnit();		
	}	
}

void AGridMovementPlayerController::SelectUnit(AGMUnit* Unit)
{
	DeSelectUnit();
	CurrentUnit = Unit;
	CurrentUnit->UnitGroundMarkerController->SetDecalSelected();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, FString::Printf(TEXT("Select Unit w overload")), false, FVector2D(1.f));
}

void AGridMovementPlayerController::DeSelectUnit()
{
	if(IsValid(CurrentUnit)) CurrentUnit->FindComponentByClass<UGMUnitGroundMarkerComponent>()->SetDecalDefault();
	CurrentUnit = nullptr;
}

void AGridMovementPlayerController::SetCurrentMousePositionHitResult()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	CurrentMousePositionHitResult = Hit;
}

void AGridMovementPlayerController::SetCurrentMouseGridPosition()
{		
	CurrentMouseGridPosition = CurrentMousePositionHitResult.Location;

	//Round the mousepos to 1m

	float x = CurrentMouseGridPosition.X / 100;
	float y = CurrentMouseGridPosition.Y / 100;
	x = round(x);
	y = round(y);
	CurrentMouseGridPosition.X = x * 100;
	CurrentMouseGridPosition.Y = y * 100;
	CurrentMouseGridPosition.Z = round(CurrentMouseGridPosition.Z);

	FString msg1 = CurrentMouseGridPosition.ToString();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, msg1, true, FVector2D(1.f));

	//If possition is new, set cover icons display
	
	if (CurrentMouseGridPosition.Size2D() != LastMouseGridPosition.Size2D() || CurrentMouseGridPosition.Z != LastMouseGridPosition.Z)
	{
		LastMouseGridPosition = CurrentMouseGridPosition;
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::White, "NewPosition", true, FVector2D(1.f));

		GridPositionCoverCheckComponent->DeactivateAllCoverBools();
		CoverIconDisplay->DisplayCoverIcons(GridPositionCoverCheckComponent->CurrentMousePositionCover, CurrentMouseGridPosition);

		if(IsCurrentMouseGridPositionValidMovementPosition(CurrentMousePositionHitResult))
		{			
			GridPositionCoverCheckComponent->CheckGridPositionForCover(CurrentMouseGridPosition);
			CoverIconDisplay->DisplayCoverIcons(GridPositionCoverCheckComponent->CurrentMousePositionCover, CurrentMouseGridPosition);			
		}		
	}
}

bool AGridMovementPlayerController::IsCurrentMouseGridPositionValidMovementPosition(FHitResult Hit)
{
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

	//Check if GridPosition has actors on position (pawns mainly)

	if (!GridPositionCoverCheckComponent->CheckIfMouseGridPositionHasActorsOnTop(CurrentMouseGridPosition))
	{
		return false;
	}
	
	return true;
}

void AGridMovementPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	SetCurrentMousePositionHitResult();		

	SetCurrentMouseGridPosition();

	MarkUnitsOnHover();
	
	if(IsValid(CurrentUnit))
	{		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Yellow, "CurrentUnit is: " + CurrentUnit->GetName(), true, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Orange, FString::Printf(TEXT("CurrentUnit has action: %hs"), CurrentUnit->hasAction ? "true" : "false"), false, FVector2D(1.f));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Orange, FString::Printf(TEXT("CurrentUnit current moveUnits: %f"), CurrentUnit->CurrentMovementUnits), false, FVector2D(1.f));
	
	}
}

void AGridMovementPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	InputComponent->BindAction("DebugCover", IE_Pressed, this, &AGridMovementPlayerController::DebugStuff);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AGridMovementPlayerController::LeftMouseButton);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AGridMovementPlayerController::RightMouseButton);
	InputComponent->BindAction("StartCombat", IE_Pressed, this, &AGridMovementPlayerController::StartCombat);
	InputComponent->BindAction("EndPlayerTurn", IE_Pressed, this, &AGridMovementPlayerController::CombatManagerEndPlayerTurn);
}

