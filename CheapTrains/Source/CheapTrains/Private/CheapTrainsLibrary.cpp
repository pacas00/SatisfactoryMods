// ILikeBanas

#pragma once

#include "CheapTrainsLibrary.h"
#include "FactoryGame.h"
#include "FGLocomotive.h"
#include "FGPipeConnectionComponent.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableTrainPlatformCargo.h"


DEFINE_LOG_CATEGORY(LogCheapTrainsUtil);

void UCheapTrainsLibrary::Log(FString string)
{
	UE_LOG(LogCheapTrainsUtil, Log, TEXT("%s"), *string);
}

void UCheapTrainsLibrary::Display(FString string)
{
	UE_LOG(LogCheapTrainsUtil, Display, TEXT("%s"), *string);
}

void UCheapTrainsLibrary::Warn(FString string)
{
	UE_LOG(LogCheapTrainsUtil, Warning, TEXT("%s"), *string);
}
void UCheapTrainsLibrary::Fatal(FString string)
{
	UE_LOG(LogCheapTrainsUtil, Fatal, TEXT("%s"), *string);
}


void UCheapTrainsLibrary::SetRecipeIngredients(UFGRecipe* rec,  TArray< FItemAmount > mIngredientsNew)
{
	rec->mIngredients = mIngredientsNew;
}


void UCheapTrainsLibrary::SetPower(AFGLocomotive* train, float min, float max)
{
	train->mPowerConsumption = FFloatInterval(min, max);
}

void UCheapTrainsLibrary::SetCurves(AFGLocomotive* train, FRuntimeFloatCurve Acceleration, FRuntimeFloatCurve Brake)
{
	train->GetLocomotiveMovementComponent()->mTractiveEffortCurve = Acceleration;
	train->GetLocomotiveMovementComponent()->mDynamicBrakingEffortCurve = Brake;
}


void UCheapTrainsLibrary::SetWagonSize(AFGFreightWagon* wagon, int32 size, int32 fluidSize)
{
	wagon->mFluidStackSizeMultiplier = fluidSize;
	wagon->mInventorySize = size;

	if (wagon->GetFreightCargoType() == EFreightCargoType::FCT_Liquid) {
		wagon->mInventorySize = fluidSize;
		if (wagon->mStorageInventory != nullptr) {
			wagon->mStorageInventory->Resize(fluidSize);
		}
	}
	if (wagon->GetFreightCargoType() == EFreightCargoType::FCT_Standard) {
		if (wagon->mStorageInventory != nullptr) {
			wagon->mStorageInventory->Resize(size);
		}
	}
}

void UCheapTrainsLibrary::SetStationPower(AFGBuildableRailroadStation* stationPlatform, float usage)
{
	stationPlatform->mPowerConsumption = usage;
}

void UCheapTrainsLibrary::SetCargoPower(AFGBuildableTrainPlatformCargo* cargoPlatform, float usage)
{
	cargoPlatform->mPowerConsumption = usage;
}

void UCheapTrainsLibrary::SetCargoSize(AFGBuildableTrainPlatformCargo* cargoPlatform, int32 size, int32 fluidSize)
{
	cargoPlatform->mStorageSizeX = 8;
	cargoPlatform->mStorageSizeY = size / 8;
	//Warn(FString::FromInt(cargoPlatform->mFluidStackSizeMultiplier));
	//Warn(FString::FromInt(fluidSize));
	//cargoPlatform->mFluidStackSizeMultiplier = fluidSize;

	if (cargoPlatform->mCargoInventoryHandler != nullptr)
	{
		if (cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent() != nullptr)
		{
			if (cargoPlatform->mFreightCargoType == EFreightCargoType::FCT_Liquid)
			{
				//cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->Resize(fluidSize);
				//Warn(FString::FromInt(cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->GetSizeLinear()));
				//Warn(FString::FromInt(cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->GetCanBeRearranged()));
				//Warn(FString::FromInt(cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->IsLocked()));
				//Warn(FString::FromInt(cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->mDefaultInventorySize));
				//Warn(FString::FromInt(cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->mInventoryStacks.Num()));
			}
			else
			{
				cargoPlatform->mCargoInventoryHandler->GetActiveInventoryComponent()->Resize(size);
			}
		}
	}
}
