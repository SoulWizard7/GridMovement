#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GMHalfCoverComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGMHalfCoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGMHalfCoverComponent();	
};