// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GMPathPointCheckComponent.h"
#include "GMUnitSelectorComponent.h"
#include "GridMovementCharacter.h"
#include "GridMovementPlayerController.generated.h"

class AGMUnit;
class UNiagaraSystem;
class AGMCoverIconDisplay;
class AGMCombatManager;

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


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGMCoverIconDisplay> CoverIconDisplayBlueprint;

	UPROPERTY()
	AGMCoverIconDisplay* CoverIconDisplay;

	UFUNCTION()
	void TryMoveUnit();	

	///////////////////////////////////////////
	//Combat Manager and Combat Functions////
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGMCombatManager> CombatManagerBluePrint;

	UPROPERTY()
	AGMCombatManager* CombatManager;

	UFUNCTION(BlueprintCallable)
	void StartCombat();

	UFUNCTION(BlueprintCallable)
	void EndCombat();

	UPROPERTY(BlueprintReadWrite)
	bool CombatIsOn;

	UFUNCTION()
	void CombatManagerEndPlayerTurn();
	
	UPROPERTY(EditAnywhere)
	AGMUnit* CurrentUnit;

	UPROPERTY(BlueprintReadOnly)
	UGMPathPointCheckComponent* PathPointCheckComponent;

	UPROPERTY(BlueprintReadOnly)
	UGMUnitSelectorComponent* UnitSelectorComponent;

	UFUNCTION()
	void TryAttackUnit();

	///////////////////////////////////////////
	// Mouse position functions            ////

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentMouseGridPosition;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentMousePosition;
	
	UPROPERTY(BlueprintReadOnly)
	FVector LastMouseGridPosition;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> PathToCurrentMouseGridPosition;

	UFUNCTION()
	void SetCurrentMousePositionHitResult();

	UPROPERTY()
	FHitResult CurrentMousePositionHitResult;

	UFUNCTION()
	void SetCurrentMouseGridPosition();

	UFUNCTION()
	bool IsCurrentMouseGridPositionValidMovementPosition(FHitResult Hit);
	
	UFUNCTION()
	void MarkUnitsOnHover();

	UPROPERTY()
	AGMUnit* CurrentHoveredUnit;

	UFUNCTION()
	void DebugStuff();

	///////////////////////////////////////////
	// INPUT
	
	UFUNCTION(BlueprintCallable)
	void EnablePlayerInput();
	
	UFUNCTION()
	void LeftMouseButton();

	UFUNCTION()
	void RightMouseButton();

	///////////////////////////////////////////

	UFUNCTION()
	void SelectUnit();	
	void SelectUnit(AGMUnit* Unit);

	UFUNCTION()
	void DeSelectUnit();	

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


