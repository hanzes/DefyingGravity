// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DefyingGravity : ModuleRules
{
	public DefyingGravity(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D","UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
