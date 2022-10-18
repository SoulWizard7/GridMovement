
#pragma once
#include "GridMovement/GMUnit.h"

#include "GMCameraController.generated.h"

UCLASS()
class AGMCameraContoller : public APawn
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void CenterOnUnit(AGMUnit* Unit);

	UFUNCTION(BlueprintCallable)
	void SetCameraPositionAtStart();
};


