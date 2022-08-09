#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "GMAIController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class AAIController;

UCLASS()
class AGMAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGMAIController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void SetupInputComponent() override;

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	/*
	bool AGMAIController::GetHitResultUnderCursor(ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult) const;
	bool AGMAIController::GetHitResultAtScreenPosition(const FVector2D ScreenPosition, const ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult) const;
*/
private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


