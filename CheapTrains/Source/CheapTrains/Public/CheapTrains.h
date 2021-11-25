// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/ChildConnection.h"
#include "Modules/ModuleManager.h"


// Logging stuff ...
DECLARE_LOG_CATEGORY_EXTERN(LogCheapTrains, Log, All);


class FSFCheapTrainsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};




