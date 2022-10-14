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
	FHitResult Hit;
	GridMovementPlayerController->GetHitResultUnderCursor(ECC_Pawn, true, Hit);

	if (Hit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::White, Hit.GetActor()->GetName(), true, FVector2D(1.f));	
		
		if(Hit.GetActor()->IsA<AGMUnit>())
		{
			AGMUnit* Unit = Cast<AGMUnit>(Hit.GetActor());
			if(!Unit->IsEnemy)
			{				
				if(IsValid(GridMovementPlayerController->CurrentUnit))
				{
					GridMovementPlayerController->CurrentUnit->UnitGroundMarkerController->SetDecalDefault();
				}
				
				GridMovementPlayerController->CurrentUnit = Cast<AGMUnit>(Hit.GetActor());
				GridMovementPlayerController->CurrentUnit->UnitGroundMarkerController->SetDecalSelected();
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

