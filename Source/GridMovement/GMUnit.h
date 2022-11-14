

#pragma once

#include "CoreMinimal.h"
#include "GMAIController.h"
#include "GMGridPositionCoverCheckComponent.h"
#include "GameFramework/Character.h"
#include "UnitComponents/GMUnitGroundMarkerComponent.h"
#include "GMUnit.generated.h"

class AGMCombatManager;
class UGMAttackCalculatorComponent;

UCLASS()
class AGMUnit : public ACharacter
{
	GENERATED_BODY()
public:
	AGMUnit();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// Components

	UPROPERTY()
	AGridMovementPlayerController* GridMovementPlayerController;

	UPROPERTY()
	UGMAttackCalculatorComponent* AttackCalculator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDecalComponent* UnitGroundMarker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGMUnitGroundMarkerComponent* UnitGroundMarkerController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AGMCombatManager* CombatManager;
	
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<AGMAIController> AIControllerBlueprint;

	UPROPERTY()
	AGMAIController* AIController;
	
	/// Stuff

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToLocation(FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	void DebugCover();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsEnemy;
	
	UFUNCTION(BlueprintCallable)
	void NextEnemyTurn();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCover CurrentCover;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayCoverIconsBlueprintEvent(FCover Cover, FVector GridPosition);

	UFUNCTION(BlueprintCallable)
	void RemoveUnitFromCombat();

	UFUNCTION(BlueprintCallable)
	void SetCurrentCoverFromCurrentPosition();

	// Attack calculation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (DisplayName = "HitChanceDistancePenaltyCurve"))
	UCurveFloat* HitChanceCurve;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ignoreActors;
	
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartPercentage = 80.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OutOfWeaponRangePenalty = 40.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HalfCoverBonus = 25.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FullCoverBonus = 40.f;

	// Combat Functions & Variables

	UPROPERTY(BlueprintReadWrite)
	bool IsInCombat = false;

	UFUNCTION()
	void UnitIsBusy(bool Action);

	UPROPERTY()
	bool WaitForUnit = false;

	UPROPERTY()
	bool UnitIsBusyWithAction = false;

	UPROPERTY()
	float UnitIsBusyTimer = 0.f;

	UFUNCTION()
	void RegainActions();

	UFUNCTION()
	void AttackUnit(AGMUnit* UnitToAttack);

	// RealTime combat Functions & Variables

	UFUNCTION()
	void RTFindCurrentTarget();

	UFUNCTION()
	void RTShootTimerReset();

	UPROPERTY()
	AGMUnit* RTCurrentTarget;

	UFUNCTION(BlueprintImplementableEvent)
	void RTAttack(AGMUnit* toAttack, bool hit);

	UPROPERTY()
	float RTShootTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RTShootTimeMax = 5.f;

	// Unit Stats

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool hasAction = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxMovementUnits = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentMovementUnits = 0.f;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimBonus = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DodgeBonus = 5.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float attackDistanceInMeter = 25.f;

};