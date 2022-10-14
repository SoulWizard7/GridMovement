#include "GMCombatEnemyTurn.h"

#include "GMCombatManager.h"
#include "GridMovement/GMUnit.h"

UGMCombatEnemyTurn::UGMCombatEnemyTurn()
{
	
}


void UGMCombatEnemyTurn::CombatEnemyTurn()
{	
	CombatEnemyTurnEvent.Broadcast();
}

void UGMCombatEnemyTurn::EndEnemyTurn()
{
	CombatManager->SetState(States::NewRound);
}

void UGMCombatEnemyTurn::CombatEnemyTurnStart()
{
	for (AGMUnit* Units : CombatManager->EnemyUnits)
	{
		Units->RegainActions();
	}
	CombatManager->SetState(States::EnemyTurn);
}
