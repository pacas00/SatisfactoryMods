// ILikeBanas

#pragma once

#include "FactoryGame.h"
#include "BlueprintJsonObject.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FGAdminInterface.h"
#include "FGPlayerController.h"
#include "WebServer.h"
 #include "FGPlayerController.h"

#include "SFSMBPFunctionLibrary.generated.h"


// Logging stuff ...
DECLARE_LOG_CATEGORY_EXTERN(LogSFServerManUtil, Log, All);
/**
 * 
 */
UCLASS(Blueprintable)
class SFSERVERMANAGER_API USFSMBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SFSM|World", meta = (WorldContext = WorldContextObject))
    static AFGAdminInterface* GetAdminInterface(UObject * WorldContextObject);


	UFUNCTION(BlueprintCallable, Category = "SFSM|Log")
    static void Log(FString string);
	UFUNCTION(BlueprintCallable, Category = "SFSM|Log")
    static void Display(FString string);
	UFUNCTION(BlueprintCallable, Category = "SFSM|Log")
    static void Warn(FString string);
	UFUNCTION(BlueprintCallable, Category = "SFSM|Log")
    static void Fatal(FString string);


	UFUNCTION(BlueprintCallable, Category = "SFSM|Event|JsonPayload") 
	static UBlueprintJsonObject* ProcessJsonObject(UPARAM(ref) UBlueprintJsonObject*& Input);

     UFUNCTION(BlueprintCallable, Category = "SFSM|Player", meta = (WorldContext = WorldContextObject))
	static ULocalPlayer* GetFirstLocalPlayerFromController(UObject * WorldContextObject);

     UFUNCTION(BlueprintCallable, Category = "SFSM|Utility")
     static TArray<FString> GetPlayers(const APlayerController* PlayerController);
     
     UFUNCTION(BlueprintCallable, Category = "SFSM|Player", meta = (WorldContext = WorldContextObject))
     static TArray<AFGPlayerController*> GetAllPlayerControllers(UObject * WorldContextObject);
     
     UFUNCTION(BlueprintCallable, Category = "SFSM|Player", meta = (WorldContext = WorldContextObject))
     static AFGPlayerController* GetPlayerController(UObject * WorldContextObject, FString PlayerName);
     
     UFUNCTION(BlueprintCallable, Category = "SFSM|Player")
     static APawn* GetPlayerPawn(AFGPlayerController* PlayerController);

	 	/**
      * Retrieves controllers of all connected players in the given world
      */
	static TArray<AFGPlayerController*> GetConnectedPlayers(const UWorld* World);
};
