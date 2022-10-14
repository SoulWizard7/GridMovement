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
	CombatManager->SetState(States::NewRound);
}

void UGMCombatStart::GetAllUnits()
{
	TArray<AActor*> units;	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CombatManager->Unit, units);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, TEXT("Found units = ") + units.Num(), true,
									 FVector2D(1.f));


	for (auto Unit : units)
	{
		AGMUnit* U = Cast<AGMUnit>(Unit);
		U->CombatManager = CombatManager;
		if(U->IsEnemy)
		{			
			CombatManager->EnemyUnits.Add(U);
		}
		else
		{
			CombatManager->PlayerUnits.Add(U);
		}
	}
}
