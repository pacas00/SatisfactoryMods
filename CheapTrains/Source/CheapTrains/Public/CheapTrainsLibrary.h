// ILikeBanas

#pragma once

#include "FactoryGame.h"
#include "CoreMinimal.h"
#include "FGFreightWagon.h"
#include "FGLocomotive.h"
#include "FGSchematic.h"
#include "Kismet/BlueprintFunctionLibrary.h"


#include "CheapTrainsLibrary.generated.h"


// Logging stuff ...
DECLARE_LOG_CATEGORY_EXTERN(LogCheapTrainsUtil, Log, All);
/**
 * 
 */
UCLASS(Blueprintable)
class CHEAPTRAINS_API UCheapTrainsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Log")
    static void Log(FString string);
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Log")
    static void Display(FString string);
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Log")
    static void Warn(FString string);
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Log")
    static void Fatal(FString string);

	
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetRecipeIngredients(UFGRecipe * rec, TArray<FItemAmount> mIngredientsNew);

	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetPower(AFGLocomotive* train, float min, float max);
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetCurves(AFGLocomotive* train, FRuntimeFloatCurve Acceleration, FRuntimeFloatCurve Brake, FRuntimeFloatCurve Power, float AI_Max_Speed);

	
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetWagonSize(AFGFreightWagon* wagon, int32 size, int32 fluidSize);

	
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetStationPower(AFGBuildableRailroadStation* stationPlatform, float usage);

	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetCargoPower(AFGBuildableTrainPlatformCargo* cargoPlatform, float usage);
	
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Utils")
	static void SetCargoSize(AFGBuildableTrainPlatformCargo* cargoPlatform, int32 size, int32 fluidSize);

	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Player", meta = (WorldContext = WorldContextObject))
	static AFGPlayerController* GetFirstLocalPlayer(UObject * WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "CheapTrains|RCO", meta = (WorldContext = WorldContextObject))
	static UFGRemoteCallObject* GetRCO(UObject* WorldContextObject, TSubclassOf<UFGRemoteCallObject> clazz);

	UFUNCTION(BlueprintCallable, Category = "CheapTrains|Player", meta = (WorldContext = WorldContextObject))
	static AFGPlayerController* GetLocalPlayerController(UObject* WorldContextObject);
};
