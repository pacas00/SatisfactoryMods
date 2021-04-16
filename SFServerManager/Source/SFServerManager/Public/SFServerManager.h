// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WebServer.h"
#include "GameFramework/Actor.h"
#include "Engine/ChildConnection.h"
#include "Modules/ModuleManager.h"


// Logging stuff ...
DECLARE_LOG_CATEGORY_EXTERN(LogSFServerMan, Log, All);


class FSFServerManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	static void WorldAdded(UWorld* world);
	static void CreateActor(const UWorld::FActorsInitializedParams& wap);

};




