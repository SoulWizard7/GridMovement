
#include "GMAttackCalculatorComponent.h"

UGMAttackCalculatorComponent::UGMAttackCalculatorComponent()
{
}

bool UGMAttackCalculatorComponent::CanAttackUnit(AGMUnit* attacker, AGMUnit* toAttack)
{
	FVector selfPosition = RoundVectorXY(attacker->GetActorLocation());
	FVector toAttackPosition = RoundVectorXY(toAttack->GetActorLocation());

	if(selfPosition.X == toAttackPosition.X || selfPosition.Y == toAttackPosition.Y) return false;
	
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

float UGMAttackCalculatorComponent::CalculatePercentage(AGMUnit* attacker, AGMUnit* toAttack)
{
	//bool canAttack = CanAttackUnit(attacker, toAttack;
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("CanAttack: %hhd"), canAttack) , true, FVector2D(1.f));
	
	float precentage = StartPercentage;

	if(!CanAttackUnit(attacker, toAttack))
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

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("dist penalty: %f"), HitChanceCurve->GetFloatValue(distance/attacker->attackDistanceInMeter)) , true, FVector2D(1.f));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, FString::Printf(TEXT("HitChance: %f"), precentage) , true, FVector2D(1.f));
	
	return precentage;
}

