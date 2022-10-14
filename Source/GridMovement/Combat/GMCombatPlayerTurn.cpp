#include "GMCombatPlayerTurn.h"

#include "GMCombatManager.h"
#include "GridMovement/GMUnit.h"

UGMCombatPlayerTurn::UGMCombatPlayerTurn()
{
}

void UGMCombatPlayerTurn::CombatPlayerTurnStart()
{
	for (AGMUnit* Unit : CombatManager->PlayerUnits)
	{
		Unit->RegainActions();
	}
	CombatManager->GridMovementPlayerController->SelectUnit(CombatManager->PlayerUnits[0]);
	CombatManager->SetState(States::PlayerTurn);
}

void UGMCombatPlayerTurn::CombatPlayerTurn()
{
	
}

void UGMCombatPlayerTurn::EndTurn()
{	
	CombatManager->SetState(States::EnemyTurn);
}

void UGMCombatPlayerTurn::PlayerDidAction()
{
	if(!CombatManager->GridMovementPlayerController->CurrentUnit->hasAction && CombatManager->GridMovementPlayerController->CurrentUnit->CurrentMovementUnits <= 0)
	{
		CombatManager->GridMovementPlayerController->DeSelectUnit();
		for (AGMUnit* Unit : CombatManager->PlayerUnits)
		{
			if(Unit->hasAction || Unit->CurrentMovementUnits > 0)
			{
				CombatManager->GridMovementPlayerController->SelectUnit(Unit);
				break;
			}
		}
		if(CombatManager->GridMovementPlayerController->CurrentUnit == nullptr)
		{
			EndTurn();
		}
	}
}
