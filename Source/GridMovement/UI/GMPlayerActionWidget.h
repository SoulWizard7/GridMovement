#pragma once
#include "Blueprint/UserWidget.h"
#include "GMPlayerActionWidget.generated.h"

class UTextBlock;
class AGMCombatManager;

UCLASS()
class UGMPlayerActionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	UTextBlock* GetNameTextBlock();

	UFUNCTION(BlueprintImplementableEvent)
	UTextBlock* GetActionTextBlock();

	void SetButtonName(const FString& Name);

	UPROPERTY(VisibleAnywhere)
	int ButtonIndex = -1;

	UPROPERTY(VisibleAnywhere)
	AGMCombatManager* CombatManager;

	UFUNCTION(BlueprintCallable)
	void ClickButton();	
};
