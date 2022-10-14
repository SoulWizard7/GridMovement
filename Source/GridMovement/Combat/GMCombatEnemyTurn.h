#pragma once
#include "GMCombatEnemyTurn.generated.h"

class AGMCombatManager;

UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class UGMCombatEnemyTurn : public UActorComponent
{
	GENERATED_BODY()
public:
	UGMCombatEnemyTurn();
	
	UFUNCTION()
	void CombatEnemyTurn();
	
	UFUNCTION(BlueprintCallable)
	void EndEnemyTurn();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatEnemyTurnEvent);
	UPROPERTY(BlueprintAssignable)
	FCombatEnemyTurnEvent CombatEnemyTurnEvent;
	
	UPROPERTY()
	AGMCombatManager* CombatManager;

	UFUNCTION()
	void CombatEnemyTurnStart();
};