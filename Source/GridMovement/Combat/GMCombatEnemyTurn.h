#pragma once
#include "GMCombatEnemyTurn.generated.h"

class AGMCombatManager;
class AGMUnit;

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

	UPROPERTY(BlueprintReadOnly)
	AGMUnit* CurrentEnemyUnitsTurn;

	UPROPERTY()
	bool ExecuteEnemyTurn = false;

	UFUNCTION()
	int GetEnemyIndexMax();

	UPROPERTY()
	int CurrentEnemyIndex = 0;

	UPROPERTY()
	int EnemyIndexMax = 0;

	UFUNCTION(BlueprintCallable)
	void NextUnit();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatEnemyTurnEvent);
	UPROPERTY(BlueprintAssignable)
	FCombatEnemyTurnEvent CombatEnemyTurnEvent;
	
	UPROPERTY()
	AGMCombatManager* CombatManager;

	UFUNCTION()
	void CombatEnemyTurnStart();

	UFUNCTION(BlueprintCallable)
	void EnemyDidAction();
};