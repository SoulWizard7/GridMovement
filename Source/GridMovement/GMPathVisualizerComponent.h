#pragma once

#include "CoreMinimal.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"

#include "GMPathVisualizerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGMPathVisualizerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGMPathVisualizerComponent();

	UFUNCTION(BlueprintCallable)
	void SetWaypoints(const TArray<FVector>& Waypoints);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceBetweenPoints;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent* Mesh;

protected:
	void SetupSpline(const TArray<FVector>& Waypoints);
	void SetupMesh();
};