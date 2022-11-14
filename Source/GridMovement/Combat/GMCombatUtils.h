#pragma once

#include "CoreMinimal.h"
#include "GridMovement/GMUnit.h"
#include "GMCombatUtils.generated.h"

/*
USTRUCT(BlueprintType)
struct FCover
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool NorthHalfCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool EastHalfCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool SouthHalfCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool WestHalfCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool NorthFullCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool EastFullCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool SouthFullCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool WestFullCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover Struct")
	bool CanMoveToPosition;
};*/

UCLASS()
class UGMCombatUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
};


FVector RoundVectorXY(FVector toRound);


float CalculatePercentage(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes,	TArray<AActor*> ignoreActors);

bool CanAttackUnit(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors);

bool RollToHit(float HitChance);

float RTCalculatePercentage(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors);

bool CheckPosAndDirForCover(FVector Position, FVector Direction, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors);

bool CheckPosAndDirForFullCover(FVector Position, FVector Direction, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors);