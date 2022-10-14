// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GridMovement : ModuleRules
{
	public GridMovement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core",
	        "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "Paper2D", "UMG" });
    }
}
