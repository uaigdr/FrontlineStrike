// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FrontlineStrike : ModuleRules
{
	public FrontlineStrike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FrontlineStrike",
			"FrontlineStrike/Variant_Platforming",
			"FrontlineStrike/Variant_Platforming/Animation",
			"FrontlineStrike/Variant_Combat",
			"FrontlineStrike/Variant_Combat/AI",
			"FrontlineStrike/Variant_Combat/Animation",
			"FrontlineStrike/Variant_Combat/Gameplay",
			"FrontlineStrike/Variant_Combat/Interfaces",
			"FrontlineStrike/Variant_Combat/UI",
			"FrontlineStrike/Variant_SideScrolling",
			"FrontlineStrike/Variant_SideScrolling/AI",
			"FrontlineStrike/Variant_SideScrolling/Gameplay",
			"FrontlineStrike/Variant_SideScrolling/Interfaces",
			"FrontlineStrike/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
