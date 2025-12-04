using UnrealBuildTool;

public class FrogShop : ModuleRules
{
    public FrogShop(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "CableComponent", "GameplayTags",
                "GameplayAbilities", "GameplayTasks", "Niagara", "OnlineSubsystem", "OnlineSubsystemSteam", "UI",
                "UMG", "AIModule"
            });
    }
}