#pragma once

#include "Runtime/Online/HTTP/Public/Http.h"
#include "HTTPClient.generated.h"

UENUM()
enum EHttpRequestStatusEnum
{
	NotStarted,
    Processing,
    Failed,
    Failed_ConnectionError,
    Succeeded,
};

UENUM()
enum class ERequestResultState : uint8
{
	SUCCESS,
    FAILURE,
};

USTRUCT()
struct FRequestStruct
{
	GENERATED_BODY()
public:
    
	UPROPERTY(EditAnywhere)
	FString URL;
    
	UPROPERTY(EditAnywhere)
	TArray<FString> Headers;
    
	UPROPERTY(EditAnywhere)
	FString Verb;
    
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EHttpRequestStatusEnum> Status;
};

USTRUCT()
struct FResponseStruct
{
	GENERATED_BODY()
public:
    
	UPROPERTY(EditAnywhere)
	int32 ResponseCode;
    
	UPROPERTY(EditAnywhere)
	FString URL;
    
	UPROPERTY(EditAnywhere)
	TArray<FString> Headers;
    
	UPROPERTY(EditAnywhere)
	FString ContentString;

	UPROPERTY(EditAnywhere)
	FString ContentType;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHTTPResponseReceived, const FRequestStruct&, _Request, const FResponseStruct&, _Response, const bool, _bRequestCompleted, const ERequestResultState&, RequestResultState);


DECLARE_LOG_CATEGORY_EXTERN(LogHTTPClient, Log, Log);


UCLASS()
class UHTTPClient : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	FHttpModule* Http;
    
	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bRequestCompleted);


	UFUNCTION()
    void HttpGETCall(FString URL);

	UFUNCTION()
    void HttpPOSTCall(FString URL, FString Payload = FString(TEXT("{}")));

	UPROPERTY()
	FHTTPResponseReceived OnReceivedResponse;


	UFUNCTION()
    static void LogRequestToSML(FRequestStruct Request);

	UFUNCTION()
    static void LogResponseToSML(FResponseStruct Response);

	UFUNCTION()
    static bool IsJsonContentUTF8(FResponseStruct Response);
};
