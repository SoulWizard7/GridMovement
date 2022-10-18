

#pragma once

#include "CoreMinimal.h"
#include "GMGridPositionCoverCheckComponent.h"
#include "GameFramework/Character.h"
#include "UnitComponents/GMUnitGroundMarkerComponent.h"
#include "GMUnit.generated.h"

class AGMCombatManager;

UCLASS()
class AGMUnit : public ACharacter
{
	GENERATED_BODY()
public:
	AGMUnit();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	AGridMovementPlayerController* GridMovementPlayerController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDecalComponent* UnitGroundMarker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGMUnitGroundMarkerComponent* UnitGroundMarkerController;

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToLocation(FVector NewLocation);

	UFUNCTION()
	void DebugCover();

	UPROPERTY(EditAnywhere)
	bool IsEnemy;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCover CurrentCover;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayCoverIconsBlueprintEvent(FCover Cover, FVector GridPosition);

	UFUNCTION(BlueprintCallable)
	void RemoveUnitFromCombat();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AGMCombatManager* CombatManager;	

	// Combat Functions

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

	// Unit Stats

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool hasAction = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxMovementUnits = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentMovementUnits = 0;


};