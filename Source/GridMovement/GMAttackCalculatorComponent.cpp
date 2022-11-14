
#include "GMAttackCalculatorComponent.h"

#include "Combat/GMCombatUtils.h"

UGMAttackCalculatorComponent::UGMAttackCalculatorComponent()
{
}

bool UGMAttackCalculatorComponent::CanAttackUnitOld(AGMUnit* attacker, AGMUnit* toAttack)
{
	FVector selfPosition = RoundVectorXY(attacker->GetActorLocation());
	FVector toAttackPosition = RoundVectorXY(toAttack->GetActorLocation());

	//if(selfPosition.X == toAttackPosition.X || selfPosition.Y == toAttackPosition.Y) return false; //Should player be able to shoot from direct line with fullcover?
	
	if(selfPosition.X - 10 > toAttackPosition.X)
	{
		if(toAttack->CurrentCover.NorthFullCover)
		{
			if (selfPosition.Y - 10 > toAttackPosition.Y)
			{
				return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::RightVector * 100, FVector::ForwardVector);
			}
			
			if (selfPosition.Y + 10 < toAttackPosition.Y)
			{
				return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::LeftVector * 100, FVector::ForwardVector);
			}
		}
	}
	
	if (selfPosition.X + 10 < toAttackPosition.X)
	{		
		if(toAttack->CurrentCover.SouthFullCover)
		{
			if (selfPosition.Y - 10 > toAttackPosition.Y)
			{
				return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::RightVector * 100, FVector::BackwardVector);
			}
			
			if (selfPosition.Y + 10 < toAttackPosition.Y)
			{
				return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::LeftVector * 100, FVector::BackwardVector);
			}
		}
	}
	
	if (selfPosition.Y - 10 > toAttackPosition.Y)
	{		
		if(toAttack->CurrentCover.EastFullCover)
		{
			if(selfPosition.X - 10 > toAttackPosition.X)
			{
				return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::ForwardVector * 100, FVector::RightVector);
			}
			if (selfPosition.X + 10 < toAttackPosition.X)
			{
				return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::BackwardVector * 100, FVector::RightVector);
			}
		}
	}
	else if (selfPosition.Y + 10 < toAttackPosition.Y)
	{		
		if(selfPosition.X - 10 > toAttackPosition.X)
		{
			return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::ForwardVector * 100, FVector::LeftVector);
		}
		if (selfPosition.X + 10 < toAttackPosition.X)
		{
			return GridPositionCoverCheckComponent->CheckPosAndDirForFullCover(toAttackPosition + FVector::BackwardVector * 100, FVector::LeftVector);
		}
	}

	return true;
}

float UGMAttackCalculatorComponent::CalculatePercentageOld(AGMUnit* attacker, AGMUnit* toAttack)
{
	bool canAttack = CanAttackUnitOld(attacker, toAttack);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("CanAttack: %hhd"), canAttack) , true, FVector2D(1.f));
	
	float precentage = StartPercentage;

	if(!CanAttackUnitOld(attacker, toAttack))
	{
		return 0.f;		
	}
	
	FVector selfPosition = RoundVectorXY(attacker->GetActorLocation());
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

	if(hasFullCover) precentage -= FullCoverBonus ;
	else if(hasHalfCover) precentage -= HalfCoverBonus;

	//Calculate distance penalty

	float distance = FVector::Dist2D(selfPosition, toAttackPosition) / 100;
	if(distance > attacker->attackDistanceInMeter)
	{
		precentage -= OutOfWeaponRangePenalty;
	}
	else
	{		
		precentage -= HitChanceCurve->GetFloatValue(distance/attacker->attackDistanceInMeter);
	}

	// Calculate unit bonuses
	precentage += attacker->AimBonus;

	// Calculate toAttack unit bonuses
	precentage -= toAttack->DodgeBonus;

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("dist penalty: %f"), HitChanceCurve->GetFloatValue(distance/attacker->attackDistanceInMeter)) , true, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("HitChance: %f"), precentage) , true, FVector2D(1.f));
	
	return precentage;
}

bool UGMAttackCalculatorComponent::RollToHitOld(float HitChance)
{
	float roll = FMath::RandRange(0, 100);
	if(roll < HitChance)
	{
		return true;
	}
	return false;
}

float UGMAttackCalculatorComponent::RTCalculatePercentageOld(AGMUnit* attacker, AGMUnit* toAttack)
{
	return CalculatePercentageOld(attacker,toAttack);
}
