#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GMGridPositionCoverCheckComponent.generated.h"


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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class UGMGridPositionCoverCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGMGridPositionCoverCheckComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool CheckIfMouseGridPositionHasActorsOnTop(FVector MousePosition);

	UFUNCTION(BlueprintCallable)
	void DeactivateAllCoverBools();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereHeight = 50.f;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ComponentSetIgnoreActors;
	
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> HalfCoverObject;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> FullCoverObject;

	UPROPERTY()
	FCover CurrentMousePositionCover;

	UFUNCTION()
	FCover CheckGridPositionForCover(FVector MousePosition, TArray<AActor*> ignoreActors);

	UFUNCTION()
	bool CheckPosAndDirForFullCover(FVector Position, FVector Direction);

	UFUNCTION()
	bool CheckPosAndDirForHalfCover(FVector Position, FVector Direction);

	UFUNCTION()
	bool CheckPosAndDirForCover(FVector Position, FVector Direction);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> FindPathToLocation(FVector Location, ACharacter* Character);
};