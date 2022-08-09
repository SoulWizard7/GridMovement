// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GMPathPointCheckComponent.h"
#include "GameFramework/Character.h"
#include "GridMovementCharacter.generated.h"

UCLASS(Blueprintable)
class AGridMovementCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGridMovementCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToLocation(FVector NewLocation);

	UFUNCTION()
	void DebugCover();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCover CurrentCover;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayCoverIconsBlueprintEvent(FCover Cover, FVector GridPosition);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

