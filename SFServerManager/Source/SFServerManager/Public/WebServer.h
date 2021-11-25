// ILikeBanas

#pragma once

#include "BlueprintJsonObject.h"
#include "CoreMinimal.h"
#include "Runtime/Online/HTTPServer/Public/HttpServerModule.h"
#include "Runtime/Online/HTTPServer/Public/HttpPath.h"

#include "HttpServerResponse.h"
#include "IHttpRouter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"

#include <SFServerManager/Public/BlueprintJsonObject.h>
#include "WebServer.generated.h"

// Logging stuff ...
DECLARE_LOG_CATEGORY_EXTERN(LogSFServerHTTP, Log, All);

/**
 * 
 */


UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFSERVERMANAGER_API AWebServer : public AActor
 {
     GENERATED_BODY()

		 public:	
	// Sets default values for this component's properties
	AWebServer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

 	
 public:
	FHttpRequestHandler CreateHandler(
		const TFunction<TUniquePtr<FHttpServerResponse>(const FHttpServerRequest&)>& HttpResponder);

 	
	UFUNCTION(BlueprintCallable, Category = "SFSM|StartServer")
	void PrepareConfig();

	UFUNCTION(BlueprintCallable, Category = "SFSM|StartServer")
	void StartServer(int32 Port);
 	UFUNCTION(BlueprintCallable, Category = "SFSM|StopServer")
	void StopServer();
	

	TUniquePtr<FHttpServerResponse> SuccessResponse(FString Message);
	TUniquePtr<FHttpServerResponse> SuccessResponse(TSharedPtr<FJsonObject> Data, FString Message);
	TUniquePtr<FHttpServerResponse> ErrorResponse(FString Message);
	TUniquePtr<FHttpServerResponse> ErrorResponse(TSharedPtr<FJsonObject> Data, FString Message);
	TUniquePtr<FHttpServerResponse> JsonResponse(TSharedPtr<FJsonObject> Data, FString Message, bool Success, int32 Code);


	TSharedPtr<FJsonObject> GetRequestJsonBody(const FHttpServerRequest& Request);
	bool IsUTF8JsonRequestContent(const FHttpServerRequest& Request);



 	//Handlers
	TUniquePtr<FHttpServerResponse> HealthCheck(const FHttpServerRequest& Request);

 	TUniquePtr<FHttpServerResponse> StatusMessage(const FHttpServerRequest& Request, AWebServer* WebServer); 	
 	TUniquePtr<FHttpServerResponse> ProcessCommand(const FHttpServerRequest& Request);


    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SFSM|Event|JsonPayload") 
	UBlueprintJsonObject* ProcessJsonObject(UPARAM(ref) UBlueprintJsonObject*& Input);

 	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SFSM|Event|StatusMessage") 
	UBlueprintJsonObject* ProcessStatusMessage(UPARAM(ref) UBlueprintJsonObject*& Input);

 	FString DefaultConfig = "{\"ServerList\":{\"Register\":false,\"HasPassword\":false,\"ServerName\":\"A Satisfactory Server\",\"Hostname\":\"SERVER.DOMAIN.NET\"},\"WebServer\":{\"Port\":8033},\"World\":{\"SaveFileName\":\"DedicatedServer\",\"AutoLoadSave\":false,\"GamePassword\":\"\",\"AdminPassword\":\"PlumTestChicken\"}}";

 	UPROPERTY(BlueprintReadWrite)
 	UBlueprintJsonObject* Configuration;
};
