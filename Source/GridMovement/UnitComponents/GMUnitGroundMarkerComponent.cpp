
#include "GMUnitGroundMarkerComponent.h"

#include "GridMovement/GridMovementPlayerController.h"

UGMUnitGroundMarkerComponent::UGMUnitGroundMarkerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGMUnitGroundMarkerComponent::BeginPlay()
{
	Super::BeginPlay();
	GridMovementPlayerController = Cast<AGridMovementPlayerController>(GetWorld()->GetFirstPlayerController());
	Owner = Cast<AGMUnit>(GetOwner());
}

void UGMUnitGroundMarkerComponent::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GroundMarkerActivated)
	{
		if(GridMovementPlayerController->CurrentHoveredUnit != Owner)
		{
			GroundMarkerActivated = false;
			if(GridMovementPlayerController->CurrentUnit != Owner)
			{
				SetDecalDefault();
			}			
		}
	}	
}

void UGMUnitGroundMarkerComponent::ActivateDecalHoverToAttack()
{
	SetDecalHoverToAttack();
	GroundMarkerActivated = true;
}

void UGMUnitGroundMarkerComponent::ActivateDecalHoverToSelect()
{
	SetDecalHoverToSelect();
	GroundMarkerActivated = true;	
}
