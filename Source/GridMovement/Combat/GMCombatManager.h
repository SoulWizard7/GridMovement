#pragma once
#include "GMCombatEnd.h"
#include "GMCombatEnemyTurn.h"
#include "GMCombatNewRound.h"
#include "GMCombatPlayerTurn.h"
#include "GMCombatStart.h"
#include "GridMovement/GridMovementPlayerController.h"
#include "GMCombatManager.generated.h"

class AGMUnit;

UENUM(BlueprintType)
enum class States : uint8
{
	StartState,
	NewRound,
	PlayerTurnStart,
	PlayerTurn,
	EnemyTurnStart,
	EnemyTurn,
	EndCombat
};

UCLASS()
class AGMCombatManager : public AActor
{
	GENERATED_BODY()
public:
	AGMCombatManager();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	AGridMovementPlayerController* GridMovementPlayerController;

	UFUNCTION(BlueprintCallable)
	void SpawnButtons();

	UFUNCTION(BlueprintCallable)
	void DeSpawnButtons();

	UFUNCTION(BlueprintCallable)
	void CombatManagerEndEnemyTurn();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// **** COMBAT STATE MACHINE ****
	UPROPERTY(VisibleAnywhere)
	UGMCombatStart* CombatStart;

	UPROPERTY(VisibleAnywhere)
	UGMCombatNewRound* CombatNewRound;

	UPROPERTY(VisibleAnywhere)
	UGMCombatPlayerTurn* CombatPlayerTurn;

	UPROPERTY(VisibleAnywhere)
	UGMCombatEnemyTurn* CombatEnemyTurn;

	UPROPERTY(VisibleAnywhere)
	UGMCombatEnd* CombatEnd;
	
	States CombatState;

	UFUNCTION()
	void SetState(States NewState);

	// ******************************
	
	UPROPERTY(EditAnywhere, Category = UnitType)
	TSubclassOf<AGMUnit>Unit;		

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<AGMUnit*> EnemyUnits;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<AGMUnit*> PlayerUnits;

	UFUNCTION(BlueprintCallable)
	void EndPlayerTurn();

	
/*
	UPROPERTY()
	AGMUnitController* UnitController;*/

	//void SetUnitState(int ButtonIndex);

};
