#pragma once

#include "CoreMinimal.h"
#include "GMGridPositionCoverCheckComponent.h"
#include "Components/DecalComponent.h"

#include "GMCoverIconDisplay.generated.h"


UCLASS()
class AGMCoverIconDisplay : public AActor
{
	GENERATED_BODY()

public:
	AGMCoverIconDisplay();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DisplayCoverIcons(FCover Cover, FVector GridPosition);
	
};