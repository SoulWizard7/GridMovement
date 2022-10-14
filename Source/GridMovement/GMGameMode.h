#pragma once

#include "GameFramework/GameMode.h"
#include "GMGameMode.generated.h"

class UGMOverlayWidget;

UCLASS()
class AGMGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AGMGameMode();

	UFUNCTION(BlueprintPure, Category = "GM|Game Mode", Meta = (DisplayName="GetCombatGameMode", WorldContext="WorldContext"))
	static AGMGameMode* Get(UObject* WorldContext);

	void StartMatch() override;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<UGMOverlayWidget> OverlayWidgetClass;

	UPROPERTY()
	UGMOverlayWidget* OverlayWidget;
	
};
