// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

using UnrealBuildTool;

public class ModulusCore : ModuleRules
{
	public ModulusCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"InputCore",
				"Engine",
				"Slate",
				"SlateCore",
				"NetCore",
				"CommonUI",
				"CommonInput",
				"UMG",
				"EnhancedInput",
				"GameplayTags",
				"DeveloperSettings"
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"PropertyPath",
				"GameplayDebugger"
				// ... add private dependencies that you statically link with here ...	
			}
			);

		bUseUnity = true;
		OptimizeCode = CodeOptimization.InShippingBuildsOnly;
	}
}
