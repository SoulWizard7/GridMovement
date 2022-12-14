

#include "GMGridPositionCoverCheckComponent.h"

#include "GMHalfCoverComponent.h"
#include "GMFullCoverComponent.h"
#include "GMWalkableSurfaceComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UGMGridPositionCoverCheckComponent::UGMGridPositionCoverCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	HalfCoverObject.Add(EObjectTypeQuery::ObjectTypeQuery7);
	FullCoverObject.Add(EObjectTypeQuery::ObjectTypeQuery8);
}

void UGMGridPositionCoverCheckComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UGMGridPositionCoverCheckComponent::CheckIfMouseGridPositionHasActorsOnTop(FVector MousePosition)
{
	// Finds if GridPosition has actors on position (pawns mainly)
	
	FVector CurPos = FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight * 2);	
	TArray<AActor*> outActors;
	bool hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CurPos, SphereRadius, TraceObjectTypes, nullptr, ComponentSetIgnoreActors, outActors);

	
	for (AActor* overlappedActor : outActors)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::White, overlappedActor->GetName(), true, FVector2D(1.f));
	}
	
	//DrawDebugSphere(GetWorld(), CurPos, SphereRadius, 5, hit ? FColor::Red : FColor::Green, false, 3.f);

	if (hit) return false;

	

	//TODO PATH PARTIAL CHECK
	
	return true;
}

void UGMGridPositionCoverCheckComponent::DeactivateAllCoverBools()
{
	CurrentMousePositionCover.NorthHalfCover = false;
	CurrentMousePositionCover.EastHalfCover = false;
	CurrentMousePositionCover.SouthHalfCover = false;
	CurrentMousePositionCover.WestHalfCover = false;
	CurrentMousePositionCover.NorthFullCover = false;
	CurrentMousePositionCover.EastFullCover = false;
	CurrentMousePositionCover.SouthFullCover = false;
	CurrentMousePositionCover.WestFullCover = false;
	CurrentMousePositionCover.CanMoveToPosition = false;
}

FCover UGMGridPositionCoverCheckComponent::CheckGridPositionForCover(FVector MousePosition, TArray<AActor*> ignoreActors)
{
	DeactivateAllCoverBools();
	FHitResult Hit;
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::White, "Checked for Cover", true, FVector2D(1.f));
	
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::ForwardVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>()) CurrentMousePositionCover.NorthHalfCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::RightVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>()) CurrentMousePositionCover.EastHalfCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::BackwardVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>()) CurrentMousePositionCover.SouthHalfCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::LeftVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>()) CurrentMousePositionCover.WestHalfCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::ForwardVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>()) CurrentMousePositionCover.NorthFullCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::RightVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);
	
	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>()) CurrentMousePositionCover.EastFullCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::BackwardVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>()) CurrentMousePositionCover.SouthFullCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight) + FVector::LeftVector * 100.f,
		TraceObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>()) CurrentMousePositionCover.WestFullCover = true;		
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z + SphereHeight * 2),
		FVector(MousePosition.X, MousePosition.Y, MousePosition.Z - SphereHeight * 2),
		TraceObjectTypes,
		true,
		ignoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{		
		if(Hit.GetActor()->FindComponentByClass<UGMWalkableSurfaceComponent>()) CurrentMousePositionCover.CanMoveToPosition = true;		
	}	
		
	return CurrentMousePositionCover;	
}

bool UGMGridPositionCoverCheckComponent::CheckPosAndDirForFullCover(FVector Position, FVector Direction)
{
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(Position.X, Position.Y, Position.Z + SphereHeight),
		FVector(Position.X, Position.Y, Position.Z + SphereHeight) + Direction * 100.f,
		TraceObjectTypes,
		false,
		ComponentSetIgnoreActors,
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

bool UGMGridPositionCoverCheckComponent::CheckPosAndDirForHalfCover(FVector Position, FVector Direction)
{
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(Position.X, Position.Y, Position.Z + SphereHeight),
		FVector(Position.X, Position.Y, Position.Z + SphereHeight) + Direction * 100.f,
		TraceObjectTypes,
		false,
		ComponentSetIgnoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>())
		{
			return false;
		}		
	}
	return true;
}

bool UGMGridPositionCoverCheckComponent::CheckPosAndDirForCover(FVector Position, FVector Direction)
{
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FVector(Position.X, Position.Y, Position.Z + SphereHeight),
		FVector(Position.X, Position.Y, Position.Z + SphereHeight) + Direction * 100.f,
		TraceObjectTypes,
		false,
		ComponentSetIgnoreActors,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,FLinearColor::Green,2);

	if(Hit.bBlockingHit)
	{
		if(Hit.GetActor()->FindComponentByClass<UGMHalfCoverComponent>())
		{
			return false;
		}
		
		if(Hit.GetActor()->FindComponentByClass<UGMFullCoverComponent>())
		{
			return false;
		}	
	}
	return true;
}

TArray<FVector> UGMGridPositionCoverCheckComponent::FindPathToLocation(FVector Location, ACharacter* Character)
{
	TArray<FVector> Result;
	FNavPathSharedPtr Path;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if (NavSys && Character)
	{
		const ANavigationData* NavData = NavSys->GetNavDataForProps(Character->GetNavAgentPropertiesRef(), Character->GetNavAgentLocation());
		if (NavData)
		{
			const FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, nullptr);
			FPathFindingQuery Query = FPathFindingQuery(*Character, *NavData, Character->GetNavAgentLocation(), Location, NavFilter);
			Query.SetAllowPartialPaths(true);

			FPathFindingResult PathResult = NavSys->FindPathSync(Query);
			if (PathResult.Result != ENavigationQueryResult::Error)
			{
				if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
				{
					if (!PathResult.IsPartial())
					{						
						PathResult.Path->EnableRecalculationOnInvalidation(true);
						Path = PathResult.Path;
					}
				}
			}
		}

		if (Path)
		{
			const TArray<FNavPathPoint>& Points = Path->GetPathPoints();
			for (int PointIndex = 0; PointIndex < Points.Num(); ++PointIndex)
			{
				Result.Add(Points[PointIndex].Location);
			}
		}
	}
	return Result;
}
