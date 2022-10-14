#include "GMUnit.h"

#include "Combat/GMCombatManager.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AGMUnit::AGMUnit()
{
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Configure Unit Ground Marker
	UnitGroundMarker = CreateDefaultSubobject<UDecalComponent>(TEXT("UnitGroundMarker"));
	UnitGroundMarker->SetupAttachment(RootComponent);
	UnitGroundMarker->DecalSize = FVector(10,50,50);
	UnitGroundMarker->AddLocalOffset(FVector(0,0, -90));
	UnitGroundMarker->AddLocalRotation(FRotator(90,0,0));
}

void AGMUnit::BeginPlay()
{
	Super::BeginPlay();
	UnitGroundMarkerController = FindComponentByClass<UGMUnitGroundMarkerComponent>();
	GridMovementPlayerController = Cast<AGridMovementPlayerController>(GetWorld()->GetFirstPlayerController());
}

void AGMUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	

	if(WaitForUnit)
	{
		while(GetVelocity().Length() > .1f)
		{
			return;
		}
		
		while(UnitIsBusyWithAction)
		{
			return;
		}
	
		while(UnitIsBusyTimer>0)
		{
			UnitIsBusyTimer -= GetWorld()->DeltaTimeSeconds;
			return;
		}

		if(IsValid(CombatManager))
		{
			CombatManager->CombatPlayerTurn->PlayerDidAction();
		}
			
		GridMovementPlayerController->EnablePlayerInput();
		WaitForUnit = false;
	}
}


void AGMUnit::DebugCover()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current NorthHalfCover: %hs"), CurrentCover.NorthHalfCover ? ("true") : ("false")), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current EastHalfCover: %hs"), CurrentCover.EastHalfCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current SouthHalfCover: %hs"), CurrentCover.SouthHalfCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Blue, FString::Printf(TEXT("Current WestHalfCover: %hs"), CurrentCover.WestHalfCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current NorthFullCover: %hs"), CurrentCover.NorthFullCover ? ("true") : ("false")), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current EastFullCover: %hs"), CurrentCover.EastFullCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current SouthFullCover: %hs"), CurrentCover.SouthFullCover ? "true" : "false"), false, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Current WestFullCover: %hs"), CurrentCover.WestFullCover ? "true" : "false"), false, FVector2D(1.f));
}

void AGMUnit::RemoveUnitFromCombat()
{	
	if(CombatManager)
	{
		if(IsEnemy)
		{
			CombatManager->EnemyUnits.Remove(this);
		}
		else
		{
			CombatManager->PlayerUnits.Remove(this);
		}
		CombatManager = nullptr;
	}
}

void AGMUnit::UnitIsBusy(bool Action)
{
	UnitIsBusyWithAction = Action;
	WaitForUnit = true;
	UnitIsBusyTimer = 0.2f;	
}

void AGMUnit::RegainActions()
{
	// check if unit has conditions that disables actions

	CurrentMovementUnits = MaxMovementUnits;
	hasAction = true;
}

void AGMUnit::AttackUnit(AGMUnit* UnitToAttack)
{
	hasAction = false;

	
	UnitIsBusyWithAction = false;	
}

