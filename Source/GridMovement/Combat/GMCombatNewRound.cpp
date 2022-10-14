#include "GMCombatNewRound.h"

#include "GMCombatManager.h"
#include "GridMovement/GMUnit.h"

UGMCombatNewRound::UGMCombatNewRound()
{
}

void UGMCombatNewRound::CombatNewRound()
{
	if(CombatManager->PlayerUnits.Num() == 0 || CombatManager->EnemyUnits.Num() == 0)
	{
		CombatManager->SetState(States::EndCombat);
		
	}
	else
	{		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.f, FColor::Green, "player Turn", true,
									 FVector2D(1.f));

		for (AGMUnit* Units : CombatManager->PlayerUnits)
		{
			Units->RegainActions();
		}
		for (AGMUnit* Units : CombatManager->EnemyUnits)
		{
			Units->RegainActions();
		}
		
		CombatManager->SetState(States::PlayerTurnStart);
	}
}
