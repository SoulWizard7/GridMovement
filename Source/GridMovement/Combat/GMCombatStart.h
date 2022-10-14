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

	UPROPERTY(VisibleAnywhere)
	AGMCombatManager* CombatManager;
	
};