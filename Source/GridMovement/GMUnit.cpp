
#include "GMUnit.h"

#include "GMAttackCalculatorComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Combat/GMCombatManager.h"
#include "Combat/GMCombatUtils.h"
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
		
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ignoreActors.Add(this);
}

void AGMUnit::BeginPlay()
{
	Super::BeginPlay();
	UnitGroundMarkerController = FindComponentByClass<UGMUnitGroundMarkerComponent>();
	GridMovementPlayerController = Cast<AGridMovementPlayerController>(GetWorld()->GetFirstPlayerController());
	AttackCalculator = GridMovementPlayerController->AttackCalculatorComponent;

	AIController = Cast<AGMAIController>(UAIBlueprintHelperLibrary::GetAIController(this));

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
			UnitIsBusyTimer -= DeltaSeconds;
			return;
		}

		GridMovementPlayerController->EnablePlayerInput();
		
		if(IsValid(CombatManager))
		{
			CombatManager->CombatPlayerTurn->PlayerDidAction();
		}
			
		WaitForUnit = false;
	}
	else
	{
		if(IsInCombat)
		{
			RTShootTimer -= DeltaSeconds;
			if(RTShootTimer < 0.f)
			{
				
				if(RTCurrentTarget != nullptr)
				{
					//RTAttack(RTCurrentTarget, AttackCalculator->RollToHitOld(AttackCalculator->RTCalculatePercentageOld(this, RTCurrentTarget)));
					RTAttack(RTCurrentTarget, AttackCalculator->RollToHitOld(RTCalculatePercentage(this, this->GetActorLocation(), RTCurrentTarget, 25.f, GetWorld(), TraceObjectTypes, ignoreActors)));
					RTShootTimerReset();
				}
			}
		}
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

void AGMUnit::NextEnemyTurn()
{
	CombatManager->CombatEnemyTurn->NextUnit();
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

void AGMUnit::SetCurrentCoverFromCurrentPosition()
{
	//WHY NO WORK???
	
	FVector pos = GetActorLocation();
	pos.Z -= 80.f;
	CurrentCover = GridMovementPlayerController->GridPositionCoverCheckComponent->CheckGridPositionForCover(pos, ignoreActors);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, FString::Printf(TEXT("Checked cover at: %ls"), *pos.ToString()), false, FVector2D(1.f));

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
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Yellow, "Current Hovered Unit is: " + UnitToAttack->GetName(), true, FVector2D(1.f));
		
	UnitIsBusyWithAction = false;	
}

void AGMUnit::RTFindCurrentTarget()
{
	AGMUnit* Target = nullptr;
	float percentage = 0;
	
	if(IsEnemy)
	{		
		for (AGMUnit* Unit : CombatManager->PlayerUnits)
		{
			float temp = RTCalculatePercentage(this, this->GetActorLocation(), Unit, 25.f, GetWorld(), TraceObjectTypes, ignoreActors);
			if (temp >= 1 && temp >= percentage)
			{
				Target = Unit;
				percentage = temp;
			}
		}		
	}
	else //isPlayerUnit
	{
		for (AGMUnit* Unit : CombatManager->EnemyUnits)
		{
			float temp = RTCalculatePercentage(this, this->GetActorLocation(), Unit, 25.f, GetWorld(), TraceObjectTypes, ignoreActors);
			if (temp >= 1 && temp >= percentage)
			{
				Target = Unit;
				percentage = temp;
			}
		}
	}

	if(Target != nullptr)
	{
		RTCurrentTarget = Target;
	}	
}

void AGMUnit::RTShootTimerReset()
{
	RTShootTimer = RTShootTimeMax;
	RTShootTimer += FMath::RandRange(-1, 1);
}

