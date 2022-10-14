
#pragma once
#include "GMUnitGroundMarkerComponent.generated.h"

class AGridMovementPlayerController;
class AGMUnit;

UCLASS( Blueprintable )

class UGMUnitGroundMarkerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UGMUnitGroundMarkerComponent();
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	AGridMovementPlayerController* GridMovementPlayerController;

	UPROPERTY()
	AGMUnit* Owner;

	UFUNCTION(BlueprintImplementableEvent)
	void SetDecalDefault();

	UFUNCTION(BlueprintImplementableEvent)
	void SetDecalSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void SetDecalHoverToAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void SetDecalHoverToSelect();

	UPROPERTY()
	bool GroundMarkerActivated = false;

	UFUNCTION(BlueprintCallable)
	void ActivateDecalHoverToAttack();

	UFUNCTION(BlueprintCallable)
	void ActivateDecalHoverToSelect();
	
};

