#pragma once

#include "CoreMinimal.h"
#include "GMCombatUtils.generated.h"

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
};

FVector RoundVectorXY(FVector toRound);