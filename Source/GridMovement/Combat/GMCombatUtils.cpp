
#include "GMCombatUtils.h"

#include "GridMovement/GMFullCoverComponent.h"
#include "GridMovement/GMHalfCoverComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UGMCombatUtils::test123()
{
	return true;
}

FVector RoundVectorXY(FVector toRound)
{
	float x = toRound.X / 100;
	float y = toRound.Y / 100;
	x = round(x);
	y = round(y);
	toRound.X = x * 100;
	toRound.Y = y * 100;
	return toRound;
}

bool CanAttackUnit(FCardinalPosition CardinalPosition, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{	
	FVector toAttackPosition = RoundVectorXY(toAttack->GetActorLocation());

	//if(selfPosition.X == toAttackPosition.X || selfPosition.Y == toAttackPosition.Y) return false; //Should player be able to shoot from direct line with fullcover?

	// NOTE: the value of 110 ensures that unit can peak around corner
	
	if(attackerPosition.X - 110 > toAttackPosition.X) //unit is North of its enemy
	{
		if(toAttack->CurrentCover.NorthFullCover)
		{
			if (CardinalPosition.East) //unit is East of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C1")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::RightVector * 100, FVector::ForwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			if (CardinalPosition.West) //unit is West of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C2")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::LeftVector * 100, FVector::ForwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}
	}
	
	if (attackerPosition.X + 110 < toAttackPosition.X) //unit is South of its enemy
	{		
		if(toAttack->CurrentCover.SouthFullCover)
		{
			if (CardinalPosition.East) //unit is East of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C3")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::RightVector * 100, FVector::BackwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			
			if (CardinalPosition.West) //unit is West of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C4")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::LeftVector * 100, FVector::BackwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}
	}
	
	if (attackerPosition.Y - 110 > toAttackPosition.Y) //unit is East of its enemy
	{		
		if(toAttack->CurrentCover.EastFullCover) 
		{
			if(CardinalPosition.North) //unit is North of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C5")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::ForwardVector * 100, FVector::RightVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			if (CardinalPosition.South) //unit is South of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C6")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::BackwardVector * 100, FVector::RightVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}
	}
	else if (attackerPosition.Y + 110 < toAttackPosition.Y) //unit is West of its enemy
	{
		if(toAttack->CurrentCover.WestFullCover)
		{
			if(CardinalPosition.North) //unit is North of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C7")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::ForwardVector * 100, FVector::LeftVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			if (CardinalPosition.South) //unit is South of its enemy
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("C8")), true, FVector2D(1.f));
				return !CheckPosAndDirForFullCover(toAttackPosition + FVector::BackwardVector * 100, FVector::LeftVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}		
	}

	return true;
}

float CalculatePercentage(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes,	TArray<AActor*> ignoreActors)
{
	FCardinalPosition CardinalPosition = {};
	FVector selfPosition = RoundVectorXY(attackerPosition);
	FVector toAttackPosition = RoundVectorXY(toAttack->GetActorLocation());

	if (selfPosition.X - 10 > toAttackPosition.X) //unit is North of its enemy
	{
		CardinalPosition.North = true;
	}
	else if (selfPosition.X + 10 < toAttackPosition.X) //unit is South of its enemy
	{
		CardinalPosition.South = true;
	}

	if (selfPosition.Y - 10 > toAttackPosition.Y) //unit is East of its enemy
	{
		CardinalPosition.East = true;
	}
	else if (selfPosition.Y + 10 < toAttackPosition.Y) //unit is West of its enemy
	{
		CardinalPosition.West = true;
	}
	
	if(!CanAttackUnit(CardinalPosition, selfPosition, toAttack, zOffset, World, TraceObjectTypes, ignoreActors))
	{		
		return 0.f;		
	}

	//Find if UnitToAttack has cover
	bool EnemyHasHalfCover = false;
	bool EnemyHasFullCover = false;
	bool AttackerHasLOS = false;
	bool AttackerHasFlank = false;

	if(CardinalPosition.North) //unit is North of its enemy
	{		
		if(attacker->CurrentCover.SouthFullCover || attacker->CurrentCover.SouthHalfCover)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("Unit is standing in cover to south")), true, FVector2D(1.f));
						
			if(!CheckPosAndDirForFullCover(attackerPosition + FVector::LeftVector * 100, FVector::BackwardVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::LeftVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("B1")), true, FVector2D(1.f));
			
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.NorthHalfCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.NorthFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;				
			}
			if(!AttackerHasFlank && !CheckPosAndDirForFullCover(attackerPosition + FVector::RightVector * 100, FVector::BackwardVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::RightVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("B2")), true, FVector2D(1.f));
			
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.NorthHalfCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.NorthFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;	
			}			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("A1")), true, FVector2D(1.f));

			if(toAttack->CurrentCover.NorthHalfCover) EnemyHasHalfCover = true;
			if(toAttack->CurrentCover.NorthFullCover) EnemyHasFullCover = true;
		}		
	}
	else if (CardinalPosition.South) //unit is South of its enemy
	{
		if(attacker->CurrentCover.NorthFullCover || attacker->CurrentCover.NorthHalfCover)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("Unit is standing in cover to north")), true, FVector2D(1.f));
						
			if(!CheckPosAndDirForFullCover(attackerPosition + FVector::LeftVector * 100, FVector::ForwardVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::LeftVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.SouthHalfCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.SouthFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;
			}
			if(!AttackerHasFlank && !CheckPosAndDirForFullCover(attackerPosition + FVector::RightVector * 100, FVector::ForwardVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::RightVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.SouthHalfCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.SouthFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("A2")), true, FVector2D(1.f));
			if(toAttack->CurrentCover.SouthHalfCover) EnemyHasHalfCover = true;
			if(toAttack->CurrentCover.SouthFullCover) EnemyHasFullCover = true;
		}		
	}
	
	if (!AttackerHasFlank && CardinalPosition.East) //unit is East of its enemy
	{
		if(attacker->CurrentCover.WestFullCover || attacker->CurrentCover.WestHalfCover)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("Unit is standing in cover to west")), true, FVector2D(1.f));
						
			if(!CheckPosAndDirForFullCover(attackerPosition + FVector::ForwardVector * 100, FVector::LeftVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::ForwardVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.EastHalfCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.EastFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;
			}
			if(!AttackerHasFlank && !CheckPosAndDirForFullCover(attackerPosition + FVector::BackwardVector * 100, FVector::LeftVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::BackwardVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.EastHalfCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.EastFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;
			}			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("A3")), true, FVector2D(1.f));
			if(toAttack->CurrentCover.EastHalfCover) EnemyHasHalfCover = true;
			if(toAttack->CurrentCover.EastFullCover) EnemyHasFullCover = true;
		}
	}
	else if (!AttackerHasFlank && CardinalPosition.West) //unit is West of its enemy
	{
		if(attacker->CurrentCover.EastFullCover || attacker->CurrentCover.EastHalfCover)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("Unit is standing in cover to east")), true, FVector2D(1.f));
						
			if(!CheckPosAndDirForFullCover(attackerPosition + FVector::ForwardVector * 100, FVector::RightVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::ForwardVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.WestFullCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.WestFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;
			}
			if(!AttackerHasFlank && !CheckPosAndDirForFullCover(attackerPosition + FVector::BackwardVector * 100, FVector::RightVector, zOffset, World, TraceObjectTypes, ignoreActors) &&
				HasLineOfSight(attackerPosition + FVector::BackwardVector * 100, toAttack->GetActorLocation(), zOffset, World, TraceObjectTypes, ignoreActors))
			{
				AttackerHasLOS = true;
				if(toAttack->CurrentCover.WestFullCover) EnemyHasHalfCover = true;
				if(toAttack->CurrentCover.WestFullCover) EnemyHasFullCover = true;
				if(!EnemyHasFullCover || !EnemyHasHalfCover) AttackerHasFlank = true;
			}			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, FString::Printf(TEXT("A4")), true, FVector2D(1.f));
			if(toAttack->CurrentCover.WestFullCover) EnemyHasHalfCover = true;
			if(toAttack->CurrentCover.WestFullCover) EnemyHasFullCover = true;
		}
	}
	
	if(!AttackerHasFlank && !AttackerHasLOS)
	{
		if(CardinalPosition.North && HasLineOfSight(attackerPosition, toAttackPosition + FVector::ForwardVector * 51, zOffset, World, TraceObjectTypes, ignoreActors))
		{
			AttackerHasLOS = true;
		}
		else if(CardinalPosition.South && HasLineOfSight(attackerPosition, toAttackPosition + FVector::BackwardVector * 51, zOffset, World, TraceObjectTypes, ignoreActors))
		{
			AttackerHasLOS = true;
		}

		if(!AttackerHasLOS && CardinalPosition.East && HasLineOfSight(attackerPosition, toAttackPosition + FVector::RightVector * 51, zOffset, World, TraceObjectTypes, ignoreActors))
		{
			AttackerHasLOS = true;
		}
		else if(!AttackerHasLOS && CardinalPosition.West && HasLineOfSight(attackerPosition, toAttackPosition + FVector::LeftVector * 51, zOffset, World, TraceObjectTypes, ignoreActors))
		{
			AttackerHasLOS = true;
		}		
	}

	//Start calculating bonuses & penalties
	
	float precentage = attacker->StartPercentage;

	if(AttackerHasLOS)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Red, FString::Printf(TEXT("UnitToAttack has LOS")), true, FVector2D(1.f));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Red, FString::Printf(TEXT("UnitToAttack has NO LOS")), true, FVector2D(1.f));
		return 0;
	}

	if(AttackerHasFlank)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Red, FString::Printf(TEXT("UnitToAttack has FLANK")), true, FVector2D(1.f));
	}
	else
	{
		if (EnemyHasFullCover)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Red, FString::Printf(TEXT("UnitToAttack has FULL cover")), true, FVector2D(1.f));
			precentage -= toAttack->FullCoverBonus;
		}
		else if (EnemyHasHalfCover)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Red, FString::Printf(TEXT("UnitToAttack has HALF cover")), true, FVector2D(1.f));
			precentage -= toAttack->HalfCoverBonus;		
		}
	}

	//Calculate distance penalty

	float distance = FVector::Dist2D(selfPosition, toAttackPosition) / 100;
	if(distance > attacker->attackDistanceInMeter)
	{
		precentage -= attacker->OutOfWeaponRangePenalty;
	}
	else
	{		
		precentage -= attacker->HitChanceCurve->GetFloatValue(distance/attacker->attackDistanceInMeter);
	}

	// Calculate unit bonuses
	precentage += attacker->AimBonus;

	// Calculate toAttack unit bonuses
	precentage -= toAttack->DodgeBonus;

	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("dist penalty: %f"), attacker->HitChanceCurve->GetFloatValue(distance/attacker->attackDistanceInMeter)) , true, FVector2D(1.f));
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("HitChance: %f"), precentage) , true, FVector2D(1.f));
	
	return precentage;
}

bool HasLineOfSight(FVector StartPos, FVector EndPos, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{
	TArray<FVector> PositionsToCheck;
	FVector Direction = EndPos - StartPos;
	Direction.Normalize();
	
	float RemainingDistance = FVector::Dist2D(StartPos, EndPos);
	RemainingDistance -= 200;
		
	while (RemainingDistance > 200)
	{
		FVector NewPos = StartPos + Direction * RemainingDistance;
		NewPos = RoundVectorXY(NewPos);
		if(!PositionsToCheck.Contains(NewPos))
		{
			PositionsToCheck.Add(NewPos);
		}
		RemainingDistance -= 100;
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("LOS checked %d positions"), PositionsToCheck.Num()) , true, FVector2D(1.f));

	for (FVector ToCheck : PositionsToCheck)
	{
		if(CheckPosForFullCover(ToCheck, zOffset, World, TraceObjectTypes, ignoreActors))
		{
			return false;
		}
	}	
	return true;	
}

bool RollToHit(float HitChance)
{
	float roll = FMath::RandRange(0, 100);
	if(roll < HitChance)
	{
		return true;
	}
	return false;
}

float RTCalculatePercentage(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{
	return CalculatePercentage(attacker, attackerPosition, toAttack, zOffset, World, TraceObjectTypes, ignoreActors);
}

bool CheckPosAndDirForCover(FVector Position, FVector Direction, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(World,
		FVector(Position.X, Position.Y, Position.Z + zOffset),
		FVector(Position.X, Position.Y, Position.Z + zOffset) + Direction * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>())
		{
			return true;
		}
		
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>())
		{
			return true;
		}	
	}
	return false;
}

bool CheckPosAndDirForFullCover(FVector Position, FVector Direction, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(World,
		FVector(Position.X, Position.Y, Position.Z + zOffset),
		FVector(Position.X, Position.Y, Position.Z + zOffset) + Direction * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>())
		{
			return true;
		}		
	}
	return false;
}

bool CheckPosForFullCover(FVector Position, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(World,
		FVector(Position.X, Position.Y, Position.Z + zOffset + 250.f),
		FVector(Position.X, Position.Y, Position.Z + zOffset),
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>())
		{
			return true;
		}	
	}
	return false;
}