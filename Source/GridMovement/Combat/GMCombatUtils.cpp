
#include "GMCombatUtils.h"

#include "GridMovement/GMFullCoverComponent.h"
#include "GridMovement/GMHalfCoverComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

bool CanAttackUnit(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, TArray<AActor*> ignoreActors)
{
	FVector selfPosition = RoundVectorXY(attackerPosition);
	FVector toAttackPosition = RoundVectorXY(toAttack->GetActorLocation());

	//if(selfPosition.X == toAttackPosition.X || selfPosition.Y == toAttackPosition.Y) return false; //Should player be able to shoot from direct line with fullcover?
	
	if(selfPosition.X - 10 > toAttackPosition.X)
	{
		if(toAttack->CurrentCover.NorthFullCover)
		{
			if (selfPosition.Y - 10 > toAttackPosition.Y)
			{
				return CheckPosAndDirForFullCover(toAttackPosition + FVector::RightVector * 100, FVector::ForwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			
			if (selfPosition.Y + 10 < toAttackPosition.Y)
			{
				return CheckPosAndDirForFullCover(toAttackPosition + FVector::LeftVector * 100, FVector::ForwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}
	}
	
	if (selfPosition.X + 10 < toAttackPosition.X)
	{		
		if(toAttack->CurrentCover.SouthFullCover)
		{
			if (selfPosition.Y - 10 > toAttackPosition.Y)
			{
				return CheckPosAndDirForFullCover(toAttackPosition + FVector::RightVector * 100, FVector::BackwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			
			if (selfPosition.Y + 10 < toAttackPosition.Y)
			{
				return CheckPosAndDirForFullCover(toAttackPosition + FVector::LeftVector * 100, FVector::BackwardVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}
	}
	
	if (selfPosition.Y - 10 > toAttackPosition.Y)
	{		
		if(toAttack->CurrentCover.EastFullCover)
		{
			if(selfPosition.X - 10 > toAttackPosition.X)
			{
				return CheckPosAndDirForFullCover(toAttackPosition + FVector::ForwardVector * 100, FVector::RightVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
			if (selfPosition.X + 10 < toAttackPosition.X)
			{
				return CheckPosAndDirForFullCover(toAttackPosition + FVector::BackwardVector * 100, FVector::RightVector, zOffset, World, TraceObjectTypes, ignoreActors);
			}
		}
	}
	else if (selfPosition.Y + 10 < toAttackPosition.Y)
	{		
		if(selfPosition.X - 10 > toAttackPosition.X)
		{
			return CheckPosAndDirForFullCover(toAttackPosition + FVector::ForwardVector * 100, FVector::LeftVector, zOffset, World, TraceObjectTypes, ignoreActors);
		}
		if (selfPosition.X + 10 < toAttackPosition.X)
		{
			return CheckPosAndDirForFullCover(toAttackPosition + FVector::BackwardVector * 100, FVector::LeftVector, zOffset, World, TraceObjectTypes, ignoreActors);
		}
	}

	return true;
}

float CalculatePercentage(AGMUnit* attacker, FVector attackerPosition, AGMUnit* toAttack, float zOffset, UWorld* World, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes,	TArray<AActor*> ignoreActors)
{	
	float precentage = attacker->StartPercentage;

	if(!CanAttackUnit(attacker, attackerPosition, toAttack, zOffset, World, TraceObjectTypes, ignoreActors))
	{
		return 0.f;		
	}
	
	FVector selfPosition = RoundVectorXY(attackerPosition);
	FVector toAttackPosition = RoundVectorXY(toAttack->GetActorLocation());	

	//Find if UnitToAttack has cover
	bool hasHalfCover = false;
	bool hasFullCover = false;

	if(selfPosition.X - 10 > toAttackPosition.X)
	{
		if(toAttack->CurrentCover.NorthHalfCover) hasHalfCover = true;
		if(toAttack->CurrentCover.NorthFullCover) hasFullCover = true;
	}
	else if (selfPosition.X - 10 < toAttackPosition.X)
	{
		if(toAttack->CurrentCover.SouthHalfCover) hasHalfCover = true;
		if(toAttack->CurrentCover.SouthFullCover) hasFullCover = true;
	}
	
	if (selfPosition.Y - 10 > toAttackPosition.Y)
	{
		if(toAttack->CurrentCover.EastHalfCover) hasHalfCover = true;
		if(toAttack->CurrentCover.EastFullCover) hasFullCover = true;
	}
	else if (selfPosition.Y - 10 < toAttackPosition.Y)
	{
		if(toAttack->CurrentCover.WestFullCover) hasHalfCover = true;
		if(toAttack->CurrentCover.WestFullCover) hasFullCover = true;
	}

	if(hasFullCover) precentage -= toAttack->FullCoverBonus ;
	else if(hasHalfCover) precentage -= toAttack->HalfCoverBonus;

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
			return false;
		}		
	}
	return true;
}