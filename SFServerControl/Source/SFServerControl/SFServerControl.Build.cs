// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using Tools.DotNETCommon;
using UnrealBuildTool;

public class SFServerControl : ModuleRules
{
	public SFServerControl(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage                  = PCHUsageMode.UseExplicitOrSharedPCHs;
		
        ShadowVariableWarningLevel = WarningLevel.Warning;
		
        PublicIncludePaths.AddRange(
                                    new string[] {
                                        "SFServerControl/Public"
                                        // ... add public include paths required here ...
                                    }
                                   );
				
		
        PrivateIncludePaths.AddRange(
                                     new string[] {
                                         "SFServerControl/Private",
                                         // ... add other private include paths required here ...
                                     }
                                    );
			
        //FactoryGame transitive dependencies
        PublicDependencyModuleNames.AddRange(new[] {
            "Core", "CoreUObject",
            "Engine",
            "InputCore",
            "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemEOS", "OnlineSubsystemUtils",
            "SignificanceManager",
            "APEX", "ApexDestruction",
            "AnimGraphRuntime",
            "AkAudio", 
            "PhysXVehicles",
            "AssetRegistry",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "ReplicationGraph",
            "SlateCore", "Slate", "UMG",
            "InstancedSplines"
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Sockets",
            "Networking"});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
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
		        
        PrivateDependencyModuleNames.AddRange(new string[] {"FactoryGame", "Http", "Json", "JsonUtilities" });
        PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame"});

        this.bEnableExceptions = true;

	}
}
