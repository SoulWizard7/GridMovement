#pragma once

#include "CoreMinimal.h"
#include "GMUnit.h"
#include "Components/ActorComponent.h"
#include "GMAttackCalculatorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGMAttackCalculatorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	

public:	
	UGMAttackCalculatorComponent();

	// THIS SCRIPT IS DEPRICATED
	// ALL FUNCTIONALITY MOVED TO COMBAT UTILS SCRIPTS

	// Components

	UPROPERTY(BlueprintReadOnly)
	UGMGridPositionCoverCheckComponent* GridPositionCoverCheckComponent;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (DisplayName = "HitChanceDistancePenaltyCurve"))
	UCurveFloat* HitChanceCurve;	

	//Functions

	UFUNCTION(BlueprintCallable)
	float CalculatePercentageOld(AGMUnit* attacker, AGMUnit* toAttack);

	UFUNCTION(BlueprintCallable)
	bool CanAttackUnitOld(AGMUnit* attacker, AGMUnit* toAttack);

	UFUNCTION(BlueprintCallable)
	bool RollToHitOld(float HitChance);

	UFUNCTION()
	float RTCalculatePercentageOld(AGMUnit* attacker, AGMUnit* toAttack);


	// Variables

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartPercentage = 80.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OutOfWeaponRangePenalty = 40.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HalfCoverBonus = 25.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FullCoverBonus = 40.f;
};