// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WiderBuildMenuCategories : ModuleRules
{
	public WiderBuildMenuCategories(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
                "WiderBuildMenuCategories/Public",
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
                "WiderBuildMenuCategories/Private",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"SlateCore", "Slate", "UMG",
				"SML",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"SML",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
        PrivateDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});
        PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});

        this.bEnableExceptions = true;
	}
}
