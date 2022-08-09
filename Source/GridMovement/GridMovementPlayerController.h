// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
//#include "GameFramework/PlayerController.h"
#include "GMNavigatorComponent.h"
#include "GMPathPointCheckComponent.h"
#include "GridMovementCharacter.h"
#include "GridMovementPlayerController.generated.h"

class AGridMovementCharacter;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AGridMovementPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGridMovementPlayerController();
	virtual void BeginPlay() override;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere)
	AGridMovementCharacter* Char;

	UPROPERTY(BlueprintReadOnly)
	UGMPathPointCheckComponent* PathPointCheckComponent;

	UFUNCTION()
	bool GetCurrentMouseGridPosition();

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentMouseGridPosition;
	
	UPROPERTY(BlueprintReadOnly)
	FVector LastMouseGridPosition;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> PathToCurrentMouseGridPosition;

	UPROPERTY(BlueprintReadOnly)
	bool hasNewPosition;

	UPROPERTY(BlueprintReadWrite)
	bool MouseGridPositionIsValid;

	UPROPERTY(BlueprintReadWrite)
	bool CanMoveToPosition;

	UFUNCTION()
	void DebugStuff();
	

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


