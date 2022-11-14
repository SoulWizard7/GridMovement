
#include "EnvQueryTest_HitChance.h"

#include "Engine/World.h"
#include "EnvironmentQuery/EQSTestingPawn.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "GridMovement/GMUnit.h"
#include "GridMovement/Combat/GMCombatUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_HitChance::UEnvQueryTest_HitChance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	//SetWorkOnFloatValues(false);
	
	Context = UEnvQueryContext_Querier::StaticClass();
	
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
}

void UEnvQueryTest_HitChance::RunTest(FEnvQueryInstance& QueryInstance) const
{	
	UObject* QueryOwner = QueryInstance.Owner.Get();

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	//bool bWantsHit = BoolValue.GetValue();
	float ItemZ = ItemHeightOffset.GetValue();	
	UWorld* curWorld = GetWorld();	

	TArray<AActor*> ContextActors;
	if(!QueryInstance.PrepareContext(Context, ContextActors))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, "Prep Context failed in EQT HitChance", true, FVector2D(1.f));
		return;
	}
	
	AGMUnit* self = Cast<AGMUnit>(ContextActors[0]);	

	if(self == nullptr)
	{
		// if used by TestingPawn
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(curWorld, AGMUnit::StaticClass(), FoundActors);
		for (auto FoundActor : FoundActors)
		{
			AGMUnit* playerUnit = Cast<AGMUnit>(FoundActor);
			if(!playerUnit->IsEnemy)
			{
				self = playerUnit;
				break;
			}
		}		
		if(self == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, "Testingpawn cast failed in EQT HitChance", true, FVector2D(1.f));
			return;
		}		
	}	

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex()) + FVector(0, 0, ItemZ);

		float maxHitChance = 0;
		
		bool bHit = false;

		for (int32 Unit = 1; Unit < ContextActors.Num(); Unit++)
		{
			AGMUnit* other = Cast<AGMUnit>(ContextActors[Unit]);
			float temp = CalculatePercentage(self, ItemLocation, other, 25.f, curWorld, TraceObjectTypes, ignoreActors);

			if(temp > maxHitChance)
			{
				maxHitChance = temp;
			}
		}

		//if(maxHitChance > 0) bHit = true;

		if (maxHitChance > 0)
		{
			It.SetScore(TestPurpose, FilterType, maxHitChance/100, MinThresholdValue, MaxThresholdValue);
			//It.SetScore(TestPurpose, FilterType, bHit, bWantsHit);
			//maxHitChance -= 10.f;
		}				
	}
}
/*
void UEnvQueryTest_HitChance::PostLoad()
{
	Super::PostLoad();
}
*/
FText UEnvQueryTest_HitChance::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("HitChance")));
}

FText UEnvQueryTest_HitChance::GetDescriptionDetails() const
{
	return FText::FromString("HitChance");
}

#undef LOCTEXT_NAMESPACE
