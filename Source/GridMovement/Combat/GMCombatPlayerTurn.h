#pragma once

#include "GMCombatPlayerTurn.generated.h"

class AGMCombatManager;
class AGMUnit;

UCLASS()
class UGMCombatPlayerTurn : public UActorComponent
{
	GENERATED_BODY()
public:
	UGMCombatPlayerTurn();

	UFUNCTION()
	void CombatPlayerTurnStart();
	
	UFUNCTION()
	void CombatPlayerTurn();
	
	UFUNCTION()
	void EndTurn();

	UFUNCTION(BlueprintCallable)
	void PlayerDidAction();

	UPROPERTY()
	AGMCombatManager* CombatManager;
	
};