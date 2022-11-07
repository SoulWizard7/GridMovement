
#include "GMCombatUtils.h"

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
