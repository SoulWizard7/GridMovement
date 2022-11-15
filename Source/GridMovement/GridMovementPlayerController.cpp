// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridMovementPlayerController.h"
#include "AIController.h"
#include "GMAttackCalculatorComponent.h"
#include "GMCoverIconDisplay.h"
#include "GMGridPositionCoverCheckComponent.h"
#include "GMWalkableSurfaceComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Combat/GMCombatManager.h"
#include "Combat/GMCombatUtils.h"
#include "UnitComponents/GMUnitGroundMarkerComponent.h"

AGridMovementPlayerController::AGridMovementPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	UnitSelectorComponent = CreateDefaultSubobject<UGMUnitSelectorComponent>("UnitSelector");
	
	AttackCalculatorComponent = CreateDefaultSubobject<UGMAttackCalculatorComponent>("AttackCalculator");
	//GridPositionCoverCheckComponent = CreateDefaultSubobject<UGMGridPositionCoverCheckComponent>("GridPositionCoverCheck") //Fix later?
	
}

void AGridMovementPlayerController::BeginPlay()
{
	//Super::BeginPlay();

	bShowMouseCursor = true;

	UnitSelectorComponent->GridMovementPlayerController = this;
	
	GridPositionCoverCheckComponent = FindComponentByClass<UGMGridPositionCoverCheckComponent>();
	AttackCalculatorComponent->GridPositionCoverCheckComponent = GridPositionCoverCheckComponent;
	
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
			DisablePlayerInput();
			
			CurrentUnit->MovePlayerUnitToLocation(CurrentMouseGridPosition);
			
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


void AGridMovementPlayerController::MarkUnitsOnHover()
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, "MarkUnitsOnHover", true, FVector2D(1.f));

	if(CurrentHoveredUnit != nullptr)
	{		
		if(CurrentUnit != nullptr)
		{			
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, "Current Hovered Unit is: " + CurrentHoveredUnit->GetName(), true, FVector2D(1.f));
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("HitChance of current hovered unit: %f"), CurrentUnit->GetHitChancePercentage(CurrentHoveredUnit)), false, FVector2D(1.f));
		}	
	}	

	if (CurrentMousePositionHitResult.GetActor())
	{
		if(CurrentHoveredUnit == CurrentMousePositionHitResult.GetActor())
		{
			return;
		}
		
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

void AGridMovementPlayerController::DisablePlayerInput()
{
	DisableInput(this);
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

void AGridMovementPlayerController::TryAttackUnit()
{
	if(CurrentUnit == nullptr) return;
	if(!CurrentUnit->hasAction) return;
	
	if(IsValid(CurrentHoveredUnit))
	{
		if(CurrentHoveredUnit->IsEnemy)
		{
			float percentage = CurrentUnit->GetHitChancePercentage(CurrentHoveredUnit);
			if (percentage != 0)
			{
				CurrentUnit->UnitIsBusy(true);
				CurrentUnit->AttackUnit(CurrentHoveredUnit, percentage);
			}			
		}		
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

void AGridMovementPlayerController::SelectUnit(AGMUnit* Unit) //Combat manager uses this function
{
	DeSelectUnit();
	CurrentUnit = Unit;
	CurrentUnit->RebuildNavigation(false);
	CurrentUnit->UnitGroundMarkerController->SetDecalSelected();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, FString::Printf(TEXT("Selected new Unit from combat manager")), false, FVector2D(1.f));
}

void AGridMovementPlayerController::DeSelectUnit()
{
	if(IsValid(CurrentUnit))
	{
		CurrentUnit->FindComponentByClass<UGMUnitGroundMarkerComponent>()->SetDecalDefault();
		CurrentUnit->RebuildNavigation(true);
	}
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

	CurrentMouseGridPosition = RoundVectorXY(CurrentMouseGridPosition);
	
	CurrentMouseGridPosition.Z = round(CurrentMouseGridPosition.Z);

	FString msg1 = CurrentMouseGridPosition.ToString();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::White, msg1, true, FVector2D(1.f));

	//If possition is new, set cover icons display
	
	if (CurrentMouseGridPosition.Size2D() != LastMouseGridPosition.Size2D() || CurrentMouseGridPosition.Z != LastMouseGridPosition.Z)
	{
		LastMouseGridPosition = CurrentMouseGridPosition;
		GridPositionCoverCheckComponent->DeactivateAllCoverBools();
		CoverIconDisplay->DisplayCoverIcons(GridPositionCoverCheckComponent->CurrentMousePositionCover, CurrentMouseGridPosition);

		if(IsCurrentMouseGridPositionValidMovementPosition(CurrentMousePositionHitResult))
		{			
			GridPositionCoverCheckComponent->CheckGridPositionForCover(CurrentMouseGridPosition, GridPositionCoverCheckComponent->ComponentSetIgnoreActors);
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

