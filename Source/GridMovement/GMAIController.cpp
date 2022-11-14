#include "GMAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AGMAIController::AGMAIController()
{
	
}

void AGMAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviourTreeBlueprint);
	BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
	SetMoraleState(ECombatMorale::Normal);
}

void AGMAIController::SetEnemyState(EEnemyState EnemyState)
{
	if(BlackboardComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, "BlackBoard is null ", true, FVector2D(1.f));
		return;
	}

	BlackboardComponent->SetValueAsEnum("EnemyState", (uint8)EnemyState);
}

void AGMAIController::SetMoraleState(ECombatMorale MoraleState)
{
	if(BlackboardComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, "BlackBoard is null ", true, FVector2D(1.f));
		return;
	}

	BlackboardComponent->SetValueAsEnum("CombatMorale", (uint8)MoraleState);	
}
