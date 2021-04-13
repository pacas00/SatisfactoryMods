// #pragma once
// #include "FGAdminInterface.h"
// #include "FindSessionsCallbackProxy.h"
// #include "Kismet/BlueprintFunctionLibrary.h"
//
// #include "SFD_C_Utils.generated.h"
//
//
// class AFGPlayerController;
// DECLARE_LOG_CATEGORY_EXTERN(LogSFDUtils, Log, Log);
//
// UCLASS(Blueprintable)
// class USFD_C_Utils : public UBlueprintFunctionLibrary
// {
//     GENERATED_BODY()
// public:
//     
//     UFUNCTION(BlueprintCallable, Category = "SFD|Utility")
//     static TArray<FString> GetPlayers(const APlayerController* PlayerController);
//     
//     UFUNCTION(BlueprintCallable, Category = "SFD|World", meta = (WorldContext = WorldContextObject))
//     static FBlueprintSessionResult GetWorldSession(UObject * WorldContextObject);
//     
//     UFUNCTION(BlueprintCallable, Category = "SFD|World", meta = (WorldContext = WorldContextObject))
//     static AFGAdminInterface* GetAdminInterface(UObject * WorldContextObject);
//     
//     UFUNCTION(BlueprintCallable, Category = "SFD|Player", meta = (WorldContext = WorldContextObject))
//     static TArray<AFGPlayerController*> GetAllPlayerControllers(UObject * WorldContextObject);
//     
//     UFUNCTION(BlueprintCallable, Category = "SFD|Player", meta = (WorldContext = WorldContextObject))
//     static AFGPlayerController* GetPlayerController(UObject * WorldContextObject, FString PlayerName);
//     
//     UFUNCTION(BlueprintCallable, Category = "SFD|Player")
//     static APawn* GetPlayerPawn(AFGPlayerController* PlayerController);
//
// 	/**
//      * Retrieves controllers of all connected players in the given world
//      */
//     static TArray<AFGPlayerController*> GetConnectedPlayers(const UWorld* World);
// };


