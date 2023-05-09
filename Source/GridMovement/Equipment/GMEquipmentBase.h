
#pragma once

#include "CoreMinimal.h"
#include "GMEquipmentBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol,
	Rifle,
	Shotgun,
	None
};

UCLASS()
class GRIDMOVEMENT_API AGMEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:
	AGMEquipmentBase();
	
	UPROPERTY(BlueprintReadWrite)
	UMeshComponent* WorldMesh;

	UPROPERTY(BlueprintReadWrite)
	UMeshComponent* InventoryMesh;
	
	//Stats

	UPROPERTY(BlueprintReadWrite)
	EWeaponType WeaponType; 
	
	UPROPERTY(BlueprintReadWrite)
	float BaseAccuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (DisplayName = "HitChanceDistancePenaltyCurve"))
	UCurveFloat* HitChanceDistancePenaltyCurve;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartHitPercentage = 80.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OutOfWeaponRangeHitPenalty = 40.f;
};
