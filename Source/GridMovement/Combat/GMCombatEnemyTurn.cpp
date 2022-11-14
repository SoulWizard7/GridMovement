#include "GMCombatEnemyTurn.h"
#include "GMCombatManager.h"
#include "GridMovement/GMUnit.h"

UGMCombatEnemyTurn::UGMCombatEnemyTurn()
{
	
}


void UGMCombatEnemyTurn::CombatEnemyTurn()
{	
	//CombatEnemyTurnEvent.Broadcast();
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Yellow, FString("Current Enemy Index is: %s ", CurrentEnemyIndex ), true, FVector2D(1.f));
		

	if (ExecuteEnemyTurn)
	{
		ExecuteEnemyTurn = false;
		CurrentEnemyUnitsTurn->AIController->SetEnemyState(EEnemyState::CombatAction);
	}
}

void UGMCombatEnemyTurn::EndEnemyTurn()
{
	CombatManager->SetState(States::NewRound);
}

void UGMCombatEnemyTurn::NextUnit()
{
	CurrentEnemyUnitsTurn->AIController->SetEnemyState(EEnemyState::CombatIdle);
	CurrentEnemyIndex += 1;
	if(CurrentEnemyIndex <= EnemyIndexMax)
	{
		CurrentEnemyUnitsTurn = CombatManager->EnemyUnits[CurrentEnemyIndex];
		ExecuteEnemyTurn = true;
	}
	else
	{
		EndEnemyTurn();
	}
}

void UGMCombatEnemyTurn::CombatEnemyTurnStart()
{
	for (AGMUnit* Units : CombatManager->EnemyUnits)
	{
		Units->RegainActions();
	}
	CurrentEnemyIndex = 0;
	EnemyIndexMax = GetEnemyIndexMax();
	CurrentEnemyUnitsTurn = CombatManager->EnemyUnits[CurrentEnemyIndex];
	CombatManager->SetState(States::EnemyTurn);
	ExecuteEnemyTurn = true;
}

int UGMCombatEnemyTurn::GetEnemyIndexMax()
{
	return CombatManager->EnemyUnits.Num() - 1;
}
