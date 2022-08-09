#include "GMAIController.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GridMovementCharacter.h"
#include "InputState.h"

AGMAIController::AGMAIController()
{
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Default;
}

void AGMAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AGMAIController::Tick(float DeltaTime)
{	
	if(bInputPressed)
	{
		FollowTime += DeltaTime;		
	}
	else
	{
		FollowTime = 0.f;
	}
}
/*
void AGMAIController::SetupInputComponent()
{
	// set up gameplay key bindings
	//Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AGMAIController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AGMAIController::OnSetDestinationReleased);
}*/

void AGMAIController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AGMAIController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if(FollowTime <= ShortPressThreshold)
	{
		// We look for the location in the world where the player has pressed the input
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		//GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		float x = HitLocation.X / 100;
		float y = HitLocation.Y / 100;

		x = round(x);
		y = round(y);

		HitLocation.X = x * 100;
		HitLocation.Y = y * 100;

		FString msg1 = HitLocation.ToString();
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::White, msg1, true, FVector2D(1.f));

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);	

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}
/*
bool AGMAIController::GetHitResultUnderCursor(ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult) const
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(this);
	bool bHit = false;
	
	
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			bHit = GetHitResultAtScreenPosition(MousePosition, TraceChannel, bTraceComplex, HitResult);
		}
	}

	if(!bHit)	//If there was no hit we reset the results. This is redundant but helps Blueprint users
		{
		HitResult = FHitResult();
		}

	return bHit;
}
bool AGMAIController::GetHitResultAtScreenPosition(const FVector2D ScreenPosition, const ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult) const
{
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), bTraceComplex );
	return GetHitResultAtScreenPosition( ScreenPosition, TraceChannel, true, HitResult );
}*/


