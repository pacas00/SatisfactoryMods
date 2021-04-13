// #include "SFD_C_Utils.h"
//
// #include "FGNetworkLibrary.h"
// #include "FGPlayerController.h"
//
//
// TArray<FString> USFD_C_Utils::GetPlayers(const APlayerController* PlayerController)
// {
//     TArray<FString> PlayersList;
//     for (AFGPlayerController* Controller : GetConnectedPlayers(PlayerController->GetWorld())) {
//         PlayersList.Add(Controller->PlayerState->GetPlayerName());
//     }
//     return PlayersList;
// }
//
// FBlueprintSessionResult USFD_C_Utils::GetWorldSession(UObject * WorldContextObject)
// {
//     UWorld * wp = WorldContextObject->GetWorld();
//     if (wp)
//     {
//         //If Dedicated, This needs to be implemented when Dedicated server exist
//         if (UKismetSystemLibrary::IsDedicatedServer(wp))
//         {
//             
//         }
//         else if (UKismetSystemLibrary::IsServer(wp))
//         {
//             ULocalPlayer* LocalPlayer = wp->GetFirstLocalPlayerFromController();
//             if (LocalPlayer)
//             {
//                 return UFGSessionLibrary::GetMySession(LocalPlayer);
//             }
//
//         }
//     }
//     return FBlueprintSessionResult();
// }
//
// AFGAdminInterface* USFD_C_Utils::GetAdminInterface(UObject * WorldContextObject)
// {
//     UWorld * wp = WorldContextObject->GetWorld();
//
//     if (wp)
//     {
//         TArray<AActor*> FoundActors;
//         UGameplayStatics::GetAllActorsOfClass(wp, AFGAdminInterface::StaticClass(), FoundActors);
//
//         UE_LOG(LogSFDUtils, Display, TEXT("Number of AFGAdminInterface %d"), FoundActors.Num());
//
//         for (AActor* Actor : FoundActors)
//         {
//             if ((*Actor).IsA<AFGAdminInterface>())
//             {
//                 return static_cast<AFGAdminInterface*>(Actor);
//             }
//         }
//     }    
//     return nullptr;
// }
//
// TArray<AFGPlayerController*> USFD_C_Utils::GetAllPlayerControllers(UObject * WorldContextObject)
// {
//     TArray<AFGPlayerController*> PlayersList;
//     UWorld * wp = WorldContextObject->GetWorld();
//
//     if (wp)
//     {
//         TArray<AActor*> FoundActors;
//         UGameplayStatics::GetAllActorsOfClass(wp, AFGPlayerController::StaticClass(), FoundActors);
//         for (AActor* Actor : FoundActors)
//         {
//             if ((*Actor).IsA<AFGPlayerController>())
//             {
//                 PlayersList.Add((static_cast<AFGPlayerController*>(Actor)));
//             }
//         }
//         
//         return PlayersList;
//     }
//     
//     return PlayersList;
// }
//
// AFGPlayerController* USFD_C_Utils::GetPlayerController(UObject * WorldContextObject, FString PlayerName)
// {
//     UWorld * wp = WorldContextObject->GetWorld();
//
//     if (wp)
//     {
//         TArray<AActor*> FoundActors;
//         UGameplayStatics::GetAllActorsOfClass(wp, AFGPlayerController::StaticClass(), FoundActors);
//         for (AActor* Actor : FoundActors)
//         {
//             if ((*Actor).IsA<AFGPlayerController>())
//             {
//                 AFGPlayerController* Player = (static_cast<AFGPlayerController*>(Actor));
//                 if (Player->PlayerState)
//                 {
//                     
//                     if (Player->PlayerState->GetPlayerName().ToLower() == PlayerName.ToLower())
//                     {
//                         return Player;
//                     }
//                 }
//             }
//         }
//         
//
//     }
//     
//     return nullptr;
// }
//
// APawn* USFD_C_Utils::GetPlayerPawn(AFGPlayerController* PlayerController)
// {
//     if (PlayerController)
//     {
//         return PlayerController->GetPawn();
//     }
//     
//     return nullptr;
// }
//
//
// TArray<AFGPlayerController*> USFD_C_Utils::GetConnectedPlayers(const UWorld* World) {
// 	TArray<AFGPlayerController*> Result;
// 	//iterate connected players
// 	for (FConstPlayerControllerIterator iterator = World->GetPlayerControllerIterator(); iterator; ++iterator) {
// 		APlayerController* controller = (*iterator).Get();
// 		if (controller == nullptr) continue;
// 		Result.Add(static_cast<AFGPlayerController*>(controller));
// 	}
// 	return Result;
//    }