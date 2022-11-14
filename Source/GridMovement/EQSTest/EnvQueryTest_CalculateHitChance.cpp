#include "EnvQueryTest_CalculateHitChance.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "GridMovement/GMUnit.h"
#include "GridMovement/Combat/GMCombatUtils.h"
#include "Kismet/GameplayStatics.h"

#define ENVQUERYTEST_DISTANCE_NAN_DETECTION 1

namespace
{
	FORCEINLINE float CalcDistance3D(const FVector& PosA, const FVector& PosB)
	{
		return (PosB - PosA).Size();
	}

	FORCEINLINE float CalcDistance2D(const FVector& PosA, const FVector& PosB)
	{
		return (PosB - PosA).Size2D();
	}

	FORCEINLINE float CalcDistanceZ(const FVector& PosA, const FVector& PosB)
	{
		return PosB.Z - PosA.Z;
	}

	FORCEINLINE float CalcDistanceAbsoluteZ(const FVector& PosA, const FVector& PosB)
	{
		return FMath::Abs(PosB.Z - PosA.Z);
	}

	FORCEINLINE void CheckItemLocationForNaN(const FVector& ItemLocation, UObject* QueryOwner, int32 Index, uint8 TestMode)
	{
#if ENVQUERYTEST_DISTANCE_NAN_DETECTION
		ensureMsgf(!ItemLocation.ContainsNaN(), TEXT("EnvQueryTest_Distance NaN in ItemLocation with owner %s. X=%f,Y=%f,Z=%f. Index:%d, TesMode:%d"), *GetPathNameSafe(QueryOwner), ItemLocation.X, ItemLocation.Y, ItemLocation.Z, Index, TestMode);
#endif
	}

	FORCEINLINE void CheckContextLocationForNaN(const FVector& ContextLocation, UObject* QueryOwner, int32 Index, uint8 TestMode)
	{
#if ENVQUERYTEST_DISTANCE_NAN_DETECTION
		ensureMsgf(!ContextLocation.ContainsNaN(), TEXT("EnvQueryTest_Distance NaN in ContextLocations with owner %s. X=%f,Y=%f,Z=%f. Index:%d, TesMode:%d"), *GetPathNameSafe(QueryOwner), ContextLocation.X, ContextLocation.Y, ContextLocation.Z, Index, TestMode);
#endif
	}
}

UEnvQueryTest_CalculateHitChance::UEnvQueryTest_CalculateHitChance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Context = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
}

void UEnvQueryTest_CalculateHitChance::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	UWorld* curWorld = GetWorld();

	// don't support context Item here, it doesn't make any sense ---- YES IT DOES NOW!!
	/*TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}*/

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
	

	
	switch (TestMode)
	{
		case EEnvTestCalculateHitChance::Distance3D:	
			for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
			{
				/*
				const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
				CheckItemLocationForNaN(ItemLocation, QueryOwner, It.GetIndex(), TestMode);
				for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
				{
					CheckContextLocationForNaN(ContextLocations[ContextIndex], QueryOwner, ContextIndex, TestMode);
					const float Distance = CalcDistance3D(ItemLocation, ContextLocations[ContextIndex]);
					It.SetScore(TestPurpose, FilterType, Distance, MinThresholdValue, MaxThresholdValue);
				}*/
				const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

				float maxHitChance = 0;
		
				//bool bHit = false;

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
			break;
		

		default:
			checkNoEntry();
			return;
	}
}

FText UEnvQueryTest_CalculateHitChance::GetDescriptionTitle() const
{
	FString ModeDesc;
	switch (TestMode)
	{
		case EEnvTestCalculateHitChance::Distance3D:
			ModeDesc = TEXT("");
			break;

		case EEnvTestCalculateHitChance::Distance2D:
			ModeDesc = TEXT(" 2D");
			break;

		case EEnvTestCalculateHitChance::DistanceZ:
			ModeDesc = TEXT(" Z");
			break;

		default:
			break;
	}

	return FText::FromString(FString::Printf(TEXT("%s%s: to %s"), 
		*Super::GetDescriptionTitle().ToString(), *ModeDesc,
		*UEnvQueryTypes::DescribeContext(Context).ToString()));
}

FText UEnvQueryTest_CalculateHitChance::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}