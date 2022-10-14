#pragma once

#include "Blueprint/UserWidget.h"
#include "GMEndTurnWidget.generated.h"

class AGMCombatManager;

UCLASS()
class UGMEndTurnWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ClickButton();

	UPROPERTY()
	AGMCombatManager* CombatManager;
};
