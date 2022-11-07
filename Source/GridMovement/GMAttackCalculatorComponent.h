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

	// Components

	UPROPERTY(BlueprintReadOnly)
	UGMGridPositionCoverCheckComponent* GridPositionCoverCheckComponent;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (DisplayName = "HitChanceCurve"))
	UCurveFloat* HitChanceCurve;	

	//Functions

	UFUNCTION(BlueprintCallable)
	float CalculatePercentage(AGMUnit* attacker, AGMUnit* toAttack);

	UFUNCTION(BlueprintCallable)
	bool CanAttackUnit(AGMUnit* attacker, AGMUnit* toAttack);


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