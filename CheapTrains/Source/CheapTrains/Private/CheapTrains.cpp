// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheapTrains.h"


#include "CheapTrainsLibrary.h"
#include "Engine/StreamableManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#define LOCTEXT_NAMESPACE "FSFCheapTrainsModule"


void FSFCheapTrainsModule::StartupModule()
{


}

void FSFCheapTrainsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSFCheapTrainsModule, CheapTrains)

////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY(LogCheapTrains);

////////////////////////////////////////////////////////////////////////////////
