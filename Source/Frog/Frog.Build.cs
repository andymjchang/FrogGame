// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Frog : ModuleRules
{
	public Frog(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Frog"
			}
		);
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "CableComponent", "GameplayTags", "GameplayAbilities", "GameplayTasks"});
	}
}
