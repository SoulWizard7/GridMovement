#include "GMCombatEnd.h"

#include "GridMovement/GridMovementPlayerController.h"

UGMCombatEnd::UGMCombatEnd()
{
}

void UGMCombatEnd::CombatEnd()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, TEXT("Combat End Script DING"), true,
									 FVector2D(1.f));
	
	AGridMovementPlayerController* pc = Cast<AGridMovementPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->EndCombat();	
}
