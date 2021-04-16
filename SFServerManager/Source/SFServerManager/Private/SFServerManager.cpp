// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFServerManager.h"


#include "SFSMBPFunctionLibrary.h"
#include "WebServer.h"
#include "Engine/StreamableManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#define LOCTEXT_NAMESPACE "FSFServerManagerModule"

	AWebServer* WebServerInstance;
	UWorld* WebServerWorldInstance;
	bool DebugLogging = true;

void FSFServerManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module


	
	//FCoreDelegates::OnPostEngineInit.AddStatic(FSFServerManagerModule::StartServer);

	FWorldDelegates::OnPostWorldCreation.AddStatic(FSFServerManagerModule::WorldAdded);

}

void FSFServerManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FSFServerManagerModule::WorldAdded(UWorld* world)
{
	if (IsValid(world) && world->IsServer())
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("World Map Name %s"), *(world->GetMapName()));
		if (world->GetMapName().Contains("MenuScene"))
		{
			//Menu
			//world->SpawnActor(AWebServer::StaticClass());
			world->OnActorsInitialized.AddStatic(FSFServerManagerModule::CreateActor);

			
		}
		if (world->GetMapName().Contains("Persistent_Level"))
		{
			//GameWorld
			//world->SpawnActor(AWebServer::StaticClass());
			world->OnActorsInitialized.AddStatic(FSFServerManagerModule::CreateActor);
		}
		
	}
}

/**
@name: SpawnBlueprintFromPath<T>(UWorld* MyWorld, const FString PathToBlueprint, const FVector SpawnLocation, FRotator SpawnRotation, FActorSpawnParameters SpawnInfo)
@param: Pointer to loaded world.
@param: Path to the Blueprint we want to spawn.
@param: Spawn Loaction for the new Blueprint actor.
@param: Spawn Rotation for the new Blueprint actor.
@param: FActorSpawnParameters for the new Blueprint actor.
@Description: Spawn a Blueprint from a refrence path in PathToBlueprint.
*/
template <typename T>
static T* SpawnBlueprintFromPath(UWorld* MyWorld)
{
	FString className("WebServerActor");
	AWebServer* classPtr = FindObject<AWebServer>(ANY_PACKAGE, *className);
	FStringAssetReference ItemToReference(TEXT("/SFServerManager/WebServerActor.WebServerActor_C"));
	UObject* ItemObject = ItemToReference.ResolveObject();
	FStringAssetReference ItemToReference2(TEXT("Blueprint'/SFServerManager/WebServerActor.WebServerActor'"));
	UObject* ItemObject2 = ItemToReference2.ResolveObject();
	FStringAssetReference ItemToReference3(TEXT("Blueprint'/SFServerManager/WebServerActor.WebServerActor_C'"));
	UObject* ItemObject3 = ItemToReference3.ResolveObject();
	if ((ItemObject) && (MyWorld))
	{
		UBlueprint* GeneratedBP = Cast<UBlueprint>(ItemObject);
		if (GeneratedBP && GeneratedBP->GeneratedClass)
		{
			if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("SPAWNING"));
			return MyWorld->SpawnActor<T>(GeneratedBP->GeneratedClass);
		}
	}
	else {
	}
	if (classPtr)
	{		
		return MyWorld->SpawnActor<T>((UClass*)classPtr);
	}

	
	return NULL;
}

void FSFServerManagerModule::CreateActor(const UWorld::FActorsInitializedParams& wap)
{
	if (!wap.World->IsServer())
	{
		return;
	}
	
	UClass* clz = LoadClass<AWebServer>(nullptr, TEXT("'/SFServerManager/WebServerActor.WebServerActor_C'"));
	AActor* actor = SpawnBlueprintFromPath<AActor>(wap.World);

	
	UBlueprint* clzzz = LoadObject<UBlueprint>(nullptr, TEXT("'/SFServerManager/BPFUNCLIB.BPFUNCLIB'"));
	UBlueprint* clzzzs = LoadObject<UBlueprint>(nullptr, TEXT("Blueprint'/SFServerManager/BPFUNCLIB.BPFUNCLIB'"));
	UBlueprint* clzzzzz = LoadObject<UBlueprint>(nullptr, TEXT("'/SFServerManager/BPFUNCLIB.BPFUNCLIB_C'"));
	UBlueprint* clzzzzzz = LoadObject<UBlueprint>(nullptr, TEXT("Blueprint'/SFServerManager/BPFUNCLIB.BPFUNCLIB_C'"));

	FStringAssetReference ItemToReference2(TEXT("Blueprint'/SFServerManager/BPFUNCLIB.BPFUNCLIB'"));
	UObject* ItemObject2 = ItemToReference2.ResolveObject();
	FStringAssetReference ItemToReference3(TEXT("Blueprint'/SFServerManager/BPFUNCLIB.BPFUNCLIB_C'"));
	UObject* ItemObject3 = ItemToReference3.ResolveObject();

	if (clzzz)
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("We have the library"));
	} else
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("NOPE NO library"));
	}
	if (clzzzs)
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("We have the clzzzs library"));
	} else
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("NOPE NO clzzzs library"));
	}
	if (clzzzzz)
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("We have the clzzzzz library"));
	} else
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("NOPE NO clzzzzz library"));
	}
	if (clzzzzzz)
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("We have the clzzzzzz library"));
	} else
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("NOPE NO clzzzzzz library"));
	}
	if (ItemObject2)
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("We have the ItemObject2 library"));
	} else
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("NOPE NO ItemObject2 library"));
	}
	if (ItemObject3)
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("We have the ItemObject3 library"));
	} else
	{
		if (DebugLogging) UE_LOG(LogSFServerMan, Display, TEXT("NOPE NO ItemObject3 library"));
	}
	
	if (clz)
	{
		actor = wap.World->SpawnActor(clz);
	}
	
	//AActor* actor = wap.World->SpawnActor(AWebServer::StaticClass());
	if (actor)
	{
		//actor->SetAutoDestroyWhenFinished(false);
		//actor->SetLifeSpan(0);
	}

	WebServerWorldInstance = wap.World;
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSFServerManagerModule, SFServerManager)

////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY(LogSFServerMan);

////////////////////////////////////////////////////////////////////////////////
