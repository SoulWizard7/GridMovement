#include "GMCombatStart.h"
#include "GMCombatManager.h"
#include "GridMovement/GMUnit.h"
#include "Kismet/GameplayStatics.h"

UGMCombatStart::UGMCombatStart()
{	
}

void UGMCombatStart::CombatStart()
{
	CombatManager->GridMovementPlayerController->DeSelectUnit();
	GetAllUnits();
	InitAllUnitCombatStartFunctions();
	CombatManager->SetState(States::NewRound);
}

void UGMCombatStart::GetAllUnits()
{
	TArray<AActor*> units;	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CombatManager->Unit, units);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.f, FColor::Green, TEXT("Found units = ") + units.Num(), true,
									 FVector2D(1.f));


	for (auto Unit : units)
	{
		AGMUnit* U = Cast<AGMUnit>(Unit);
		
		if(U->IsEnemy)
		{			
			CombatManager->EnemyUnits.Add(U);
		}
		else
		{
			CombatManager->PlayerUnits.Add(U);
		}
		U->CombatManager = CombatManager;		
	}
}

void UGMCombatStart::InitAllUnitCombatStartFunctions()
{
	for (AGMUnit* Unit : CombatManager->PlayerUnits)
	{
		Unit->SetCurrentCoverFromCurrentPosition();
		Unit->RTShootTimerReset();
		Unit->RTFindCurrentTarget();
		Unit->IsInCombat = true;
	}
	for (AGMUnit* Unit : CombatManager->EnemyUnits)
	{
		Unit->SetCurrentCoverFromCurrentPosition();
		Unit->RTShootTimerReset();
		Unit->RTFindCurrentTarget();
		Unit->IsInCombat = true;
		Unit->AIController->SetEnemyState(EEnemyState::CombatIdle);
	}	
}
