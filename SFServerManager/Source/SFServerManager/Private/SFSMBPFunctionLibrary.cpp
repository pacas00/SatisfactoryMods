// ILikeBanas

#pragma once

#include "SFSMBPFunctionLibrary.h"
#include "FactoryGame.h"

#include "Kismet/GameplayStatics.h"
#include "FGLocalPlayer.h"



AFGAdminInterface* USFSMBPFunctionLibrary::GetAdminInterface(UObject * WorldContextObject)
 {
     UWorld * wp = WorldContextObject->GetWorld();

     if (wp)
     {
         TArray<AActor*> FoundActors;
         UGameplayStatics::GetAllActorsOfClass(wp, AFGAdminInterface::StaticClass(), FoundActors);

         
         for (AActor* Actor : FoundActors)
         {
             if ((*Actor).IsA<AFGAdminInterface>())
             {
                 return static_cast<AFGAdminInterface*>(Actor);
             }
         }
     }    
     return nullptr;
 }


DEFINE_LOG_CATEGORY(LogSFServerManUtil);

void USFSMBPFunctionLibrary::Log(FString string)
{
	UE_LOG(LogSFServerManUtil, Log, TEXT("%s"), *string);
}

void USFSMBPFunctionLibrary::Display(FString string)
{
	UE_LOG(LogSFServerManUtil, Display, TEXT("%s"), *string);
}

void USFSMBPFunctionLibrary::Warn(FString string)
{
	UE_LOG(LogSFServerManUtil, Warning, TEXT("%s"), *string);
}
void USFSMBPFunctionLibrary::Fatal(FString string)
{
	UE_LOG(LogSFServerManUtil, Fatal, TEXT("%s"), *string);
}

UBlueprintJsonObject* USFSMBPFunctionLibrary::ProcessJsonObject(UBlueprintJsonObject*& Input)
{
    //if (RealBPObject.Succeeded())
    //{
	   // UFunction* func = RealBPObject.Object->FindFunction(FName(TEXT("Processjsonobject")));
    //}

	
	return Input;
}

 TArray<FString> USFSMBPFunctionLibrary::GetPlayers(const APlayerController* PlayerController)
 {
     TArray<FString> PlayersList;
     for (AFGPlayerController* Controller : GetConnectedPlayers(PlayerController->GetWorld())) {
         PlayersList.Add(Controller->PlayerState->GetPlayerName());
     }
     return PlayersList;
 }

 ULocalPlayer* USFSMBPFunctionLibrary::GetFirstLocalPlayerFromController(UObject * WorldContextObject)
 {
     UWorld * wp = WorldContextObject->GetWorld();
     if (wp)
     {
         //If Dedicated, This needs to be implemented when Dedicated server exist
         if (UKismetSystemLibrary::IsDedicatedServer(wp))
         {
             
         }
         else if (UKismetSystemLibrary::IsServer(wp))
         {
             ULocalPlayer* LocalPlayer = wp->GetFirstLocalPlayerFromController();
             if (LocalPlayer)
             {
                 return (LocalPlayer);
             }

         }
     }
     return nullptr;
 }


 TArray<AFGPlayerController*> USFSMBPFunctionLibrary::GetAllPlayerControllers(UObject * WorldContextObject)
 {
     TArray<AFGPlayerController*> PlayersList;
     UWorld * wp = WorldContextObject->GetWorld();

     if (wp)
     {
         TArray<AActor*> FoundActors;
         UGameplayStatics::GetAllActorsOfClass(wp, AFGPlayerController::StaticClass(), FoundActors);
         for (AActor* Actor : FoundActors)
         {
             if ((*Actor).IsA<AFGPlayerController>())
             {
                 PlayersList.Add((static_cast<AFGPlayerController*>(Actor)));
             }
         }
         
         return PlayersList;
     }
     
     return PlayersList;
 }

 AFGPlayerController* USFSMBPFunctionLibrary::GetPlayerController(UObject * WorldContextObject, FString PlayerName)
 {
     UWorld * wp = WorldContextObject->GetWorld();

     if (wp)
     {
         TArray<AActor*> FoundActors;
         UGameplayStatics::GetAllActorsOfClass(wp, AFGPlayerController::StaticClass(), FoundActors);
         for (AActor* Actor : FoundActors)
         {
             if ((*Actor).IsA<AFGPlayerController>())
             {
                 AFGPlayerController* Player = (static_cast<AFGPlayerController*>(Actor));
                 if (Player->PlayerState)
                 {
                     
                     if (Player->PlayerState->GetPlayerName().ToLower() == PlayerName.ToLower())
                     {
                         return Player;
                     }
                 }
             }
         }
         

     }
     
     return nullptr;
 }

 APawn* USFSMBPFunctionLibrary::GetPlayerPawn(AFGPlayerController* PlayerController)
 {
     if (PlayerController)
     {
         return PlayerController->GetPawn();
     }
     
     return nullptr;
 }


 TArray<AFGPlayerController*> USFSMBPFunctionLibrary::GetConnectedPlayers(const UWorld* World) {
 	TArray<AFGPlayerController*> Result;
 	//iterate connected players
 	for (FConstPlayerControllerIterator iterator = World->GetPlayerControllerIterator(); iterator; ++iterator) {
 		APlayerController* controller = (*iterator).Get();
 		if (controller == nullptr) continue;
 		Result.Add(static_cast<AFGPlayerController*>(controller));
 	}
 	return Result;
    
}


