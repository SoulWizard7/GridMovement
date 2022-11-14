
#include "EnvQueryTest_FindCover.h"

#include "Engine/World.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "GridMovement/GMFullCoverComponent.h"
#include "GridMovement/GMHalfCoverComponent.h"
#include "GridMovement/Combat/GMCombatManager.h"
#include "GridMovement/Combat/GMCombatUtils.h"
#include "Kismet/KismetSystemLibrary.h"


#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_FindCover::UEnvQueryTest_FindCover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	SetWorkOnFloatValues(false);
	
	Context = UEnvQueryContext_Querier::StaticClass();
	
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
}

void UEnvQueryTest_FindCover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	ItemHeightOffset.BindData(DataOwner, QueryInstance.QueryID);

	bool bWantsHit = BoolValue.GetValue();
	float ItemZ = ItemHeightOffset.GetValue();
	UWorld* curWorld = GetWorld();
	
	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex()) + FVector(0, 0, ItemZ);

		for (int32 Unit = 0; Unit < ContextLocations.Num(); Unit++)
		{				
			bool bHit = false;

			if(ItemLocation.X - 10 > ContextLocations[Unit].X)
			{
				//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Turquoise, FString::Printf(TEXT("Unit is South")), true, FVector2D(1.f));
				bHit = CheckPosAndDirForCover(ItemLocation, FVector::BackwardVector, ItemZ, curWorld, TraceObjectTypes, ignoreActors);
				
			}
			else if (ItemLocation.X + 10 < ContextLocations[Unit].X)
			{
				//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Turquoise, FString::Printf(TEXT("Unit is North")), true, FVector2D(1.f));
				bHit = CheckPosAndDirForCover(ItemLocation, FVector::ForwardVector, ItemZ, curWorld, TraceObjectTypes, ignoreActors);				
			}

			if(bHit)
			{
				It.SetScore(TestPurpose, FilterType, bHit, bWantsHit);
				continue;
			}
	
			if (ItemLocation.Y - 10 > ContextLocations[Unit].Y)
			{
				//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Turquoise, FString::Printf(TEXT("Unit is West")), true, FVector2D(1.f));
				bHit = CheckPosAndDirForCover(ItemLocation, FVector::LeftVector, ItemZ, curWorld, TraceObjectTypes, ignoreActors);
			}
			else if (ItemLocation.Y + 10 < ContextLocations[Unit].Y)
			{
				//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Turquoise, FString::Printf(TEXT("Unit is East")), true, FVector2D(1.f));
				bHit = CheckPosAndDirForCover(ItemLocation, FVector::RightVector, ItemZ, curWorld, TraceObjectTypes, ignoreActors);
			}	

			It.SetScore(TestPurpose, FilterType, bHit, bWantsHit);
		}
	}
}

void UEnvQueryTest_FindCover::PostLoad()
{
	Super::PostLoad();
}

FText UEnvQueryTest_FindCover::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("Find cover")));
}

FText UEnvQueryTest_FindCover::GetDescriptionDetails() const
{
	return FText::FromString("Finds cover from all units given in -Context- bar");
}

#undef LOCTEXT_NAMESPACE
