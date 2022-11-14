#pragma once
#include "GMCombatStart.generated.h"

class AGMCombatManager;

UCLASS()
class UGMCombatStart : public UActorComponent
{
	GENERATED_BODY()
public:
	UGMCombatStart();
	
	UFUNCTION()
	void CombatStart();
	
	UFUNCTION()
	void GetAllUnits();

	UFUNCTION()
	void InitAllUnitCombatStartFunctions();

	UPROPERTY(VisibleAnywhere)
	AGMCombatManager* CombatManager;
	
};