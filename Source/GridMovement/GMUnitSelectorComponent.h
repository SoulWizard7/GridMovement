
#pragma once

#include "CoreMinimal.h"
#include "GMUnit.h"
//#include "GridMovementPlayerController.h"
#include "Components/ActorComponent.h"
#include "GMUnitSelectorComponent.generated.h"

class AGridMovementPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class UGMUnitSelectorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UGMUnitSelectorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TryGetUnit();
	UFUNCTION()
	void TryGetUnitCombat();

	UPROPERTY()
	AGridMovementPlayerController* GridMovementPlayerController;
	
};