#include "GMCombatPlayerTurn.h"

#include "GMCombatManager.h"
#include "GridMovement/GMUnit.h"

UGMCombatPlayerTurn::UGMCombatPlayerTurn()
{
}

void UGMCombatPlayerTurn::CombatPlayerTurnStart()
{
	CombatManager->GridMovementPlayerController->EnablePlayerInput();
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
	//TODO wait until input is enabled
	
	CombatManager->GridMovementPlayerController->DisablePlayerInput();
	CombatManager->SetState(States::EnemyTurnStart);
}

void UGMCombatPlayerTurn::PlayerDidAction()
{
	for (AGMUnit* Unit : CombatManager->EnemyUnits)
	{
		Unit->RTFindCurrentTarget();
	}
	
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
