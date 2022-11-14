#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GMAIController.generated.h"

UENUM(BlueprintType)
enum class ECombatMorale : uint8
{
	Aggressive,
	Normal,
	Defensive
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	CombatIdle,
	CombatAction
};


UCLASS(Meta = (BlueprintSpawnableComponent))
class AGMAIController : public AAIController
{
	GENERATED_BODY()
	AGMAIController();
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadOnly)
	UBlackboardComponent* BlackboardComponent;

	UFUNCTION(BlueprintCallable)
	void SetEnemyState(EEnemyState EnemyState);

	UFUNCTION(BlueprintCallable)
	void SetMoraleState(ECombatMorale MoraleState);


	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviourTreeBlueprint;
};


