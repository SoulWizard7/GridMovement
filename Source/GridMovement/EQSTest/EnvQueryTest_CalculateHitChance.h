#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_CalculateHitChance.generated.h"

UENUM()
namespace EEnvTestCalculateHitChance
{
	enum Type
	{
		Distance3D,
		Distance2D,
		DistanceZ,
		DistanceAbsoluteZ UMETA(DisplayName = "Distance Z (Absolute)")
	};
}

UCLASS()
class UEnvQueryTest_CalculateHitChance : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	/** testing mode */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TEnumAsByte<EEnvTestCalculateHitChance::Type> TestMode;

	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TSubclassOf<UEnvQueryContext> Context;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ignoreActors;
	
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
