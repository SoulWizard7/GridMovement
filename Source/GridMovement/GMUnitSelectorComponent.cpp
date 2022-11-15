#include "GMUnitSelectorComponent.h"
#include "GridMovementPlayerController.h"
#include "UnitComponents/GMUnitGroundMarkerComponent.h"

UGMUnitSelectorComponent::UGMUnitSelectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGMUnitSelectorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGMUnitSelectorComponent::TryGetUnit()
{
	if (GridMovementPlayerController->CurrentMousePositionHitResult.GetActor())
	{
		if(GridMovementPlayerController->CurrentMousePositionHitResult.GetActor()->IsA<AGMUnit>())
		{
			AGMUnit* Unit = Cast<AGMUnit>(GridMovementPlayerController->CurrentMousePositionHitResult.GetActor());
			if(!Unit->IsEnemy)
			{				
				if(IsValid(GridMovementPlayerController->CurrentUnit))
				{
					GridMovementPlayerController->CurrentUnit->UnitGroundMarkerController->SetDecalDefault();
					GridMovementPlayerController->CurrentUnit->RebuildNavigation(true);
				}

				GridMovementPlayerController->CameraController->CenterOnUnit(Unit);
				
				GridMovementPlayerController->CurrentUnit = Unit;
				GridMovementPlayerController->CurrentUnit->UnitGroundMarkerController->SetDecalSelected();
				Unit->RebuildNavigation(false);
			}
			else
			{
				GridMovementPlayerController->DeSelectUnit();
			}
		}
		else
		{
			GridMovementPlayerController->DeSelectUnit();
		}
	}
}

void UGMUnitSelectorComponent::TryGetUnitCombat()
{	
	if (GridMovementPlayerController->CurrentMousePositionHitResult.GetActor())
	{
		if(GridMovementPlayerController->CurrentMousePositionHitResult.GetActor()->IsA<AGMUnit>())
		{
			AGMUnit* Unit = Cast<AGMUnit>(GridMovementPlayerController->CurrentMousePositionHitResult.GetActor());
			
			if(!Unit->IsEnemy)
			{
				if(Unit->hasAction || Unit->CurrentMovementUnits <= 0)
				{
					if(IsValid(GridMovementPlayerController->CurrentUnit))
					{
						GridMovementPlayerController->CurrentUnit->UnitGroundMarkerController->SetDecalDefault();
						GridMovementPlayerController->CurrentUnit->RebuildNavigation(true);
					}
				
					GridMovementPlayerController->CameraController->CenterOnUnit(Unit);
				
					GridMovementPlayerController->CurrentUnit = Unit;
					GridMovementPlayerController->CurrentUnit->UnitGroundMarkerController->SetDecalSelected();
					Unit->RebuildNavigation(false);
				}
				else
				{
					GridMovementPlayerController->DeSelectUnit();
				}
			}
			else
			{
				GridMovementPlayerController->DeSelectUnit();
			}
		}		
	}
}

