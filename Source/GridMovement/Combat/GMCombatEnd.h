#pragma once
#include "GMCombatEnd.generated.h"

UCLASS()
class UGMCombatEnd : public UActorComponent
{
	GENERATED_BODY()
public:
	UGMCombatEnd();
	
	UFUNCTION()
	void CombatEnd();
};
