

#include "GMCoverIconDisplay.h"

AGMCoverIconDisplay::AGMCoverIconDisplay()
{
}

void AGMCoverIconDisplay::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
}

void AGMCoverIconDisplay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
