#pragma once

#include "HTTPDataStructs.generated.h"

struct HTTPDataStructs
{
    
};

USTRUCT(BlueprintType)
struct FHTTPClientInfo {
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Uuid = "00000000-0000-0000-0000-000000000000";
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Apikey = "67744158a7d8421e935c501586b5c1452c908c90380b1ae6";
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StatsJson;
};

UENUM(BlueprintType)
enum EHTTPMessageType
{
    //These are sent to the queue for the UE Mod to process
    //Messages Read by Client on Get
    
    ChatMessage			= 0		UMETA(DisplayName = "Chat Message"),
    LoadWorld			= 1		UMETA(DisplayName = "Load World"),
    SaveWorld			= 2		UMETA(DisplayName = "Save World"),
    QuitGame			= 3		UMETA(DisplayName = "Quit Game"),
    KickPlayer			= 4		UMETA(DisplayName = "Kick Player"),
    BanPlayer			= 5		UMETA(DisplayName = "Ban Player"),
    ListPlayer			= 6		UMETA(DisplayName = "List Player"),



    //These are returned to the Server, or Queued by the Server, Expecting a Message of the same type to return.
    //Messages Sent to Server as a response to a Get Message, Or Replies from the server
    //Sometimes these are used as Get Messages, aka WorldList put in queue, to get a world list from game.

    NoResponse		    = 80		UMETA(DisplayName = "No Response"),
    CommandResponse		= 81		UMETA(DisplayName = "Command Response"),
    WorldList    		= 82		UMETA(DisplayName = "World List Response"),
    PlayerList    		= 83		UMETA(DisplayName = "Player List Response"),



    RegistrationResponse= 110		UMETA(DisplayName = "Registration Response"),

    MAXITEM			= 127	UMETA(DisplayName = "MAXITEM"),
};

USTRUCT(BlueprintType)
struct FHTTPMessage {
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EHTTPMessageType> MessageType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MessagePayload;
};

USTRUCT(BlueprintType)
struct FHTTPGetMessageResponse {
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHTTPClientInfo Client;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHTTPMessage Message;
};
