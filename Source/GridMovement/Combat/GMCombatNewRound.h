#pragma once
#include "GMCombatNewRound.generated.h"

class AGMCombatManager;

UCLASS()
class UGMCombatNewRound : public UActorComponent
{
	GENERATED_BODY()
public:
	UGMCombatNewRound();
	
	UFUNCTION()
	void CombatNewRound();

	UPROPERTY()
	AGMCombatManager* CombatManager;	
};