// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using Tools.DotNETCommon;
using UnrealBuildTool;

public class CheapTrains : ModuleRules
{
	public CheapTrains(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
                "CheapTrains/Public",
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
                "CheapTrains/Private",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Sockets",
                "Json",
                "JsonUtilities",
                "HTTP",
                "ReplicationGraph",
                "SlateCore", "Slate", "UMG",
                "Slate",
                "SlateCore",
                "Sockets",
                "Networking"
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
                "Core",
                "Engine",
                "Sockets",
                "Json",
                "JsonUtilities",
                "HTTP",
                "HttpServer",
                "Slate",
                "SlateCore",
                "Sockets",
                "Networking"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        PrivateDependencyModuleNames.AddRange(new string[] {"FactoryGame"});
        PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame"});

        this.bEnableExceptions = true;
	}
}
