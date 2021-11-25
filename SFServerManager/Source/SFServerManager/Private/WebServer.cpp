
#include "WebServer.h"



#include "FDirectoryLister.h"
#include "SFServerManager.h"
#include "SFSMBPFunctionLibrary.h"
#include "Modules/BuildVersion.h"


DEFINE_LOG_CATEGORY(LogSFServerHTTP);

void AWebServer::PrepareConfig()
{
	if (!GetWorld()->IsServer())
	{
		return;
	}
	//Load Default Config
	Configuration = UBlueprintJsonObject::Create();
	UBlueprintJsonObject* DefaultConfiguration = UBlueprintJsonObject::Create();
	UBlueprintJsonObject::Parse(DefaultConfig, Configuration);
	UBlueprintJsonObject::Parse(DefaultConfig, DefaultConfiguration);

	FString AWebFolderPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + TEXT("/Configs/");
	FString AWebFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + TEXT("/Configs/SFServerManager.cfg");
	FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*AWebFolderPath);
	bool makeNewKey = false;
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*AWebFilePath))
	{
		FString Result;
		if (FFileHelper::LoadFileToString(Result, *AWebFilePath))
		{			
			if (!UBlueprintJsonObject::Parse(Result, Configuration))
			{
				UE_LOG(LogSFServerHTTP, Log, TEXT("Error Loading Config File, Reverting to default"));
				//Failed, Revert to default
				Configuration = UBlueprintJsonObject::Create();
				UBlueprintJsonObject::Parse(DefaultConfig, Configuration);
				makeNewKey = true;
			}
		}
	} else
	{
		makeNewKey = true;
	}

	//Ensure all values are present, if not, defaults
	
	UBlueprintJsonObject* config_obj;
	UBlueprintJsonObject* default_obj;
	
	if (!Configuration->HasObject("WebServer"))
	{
		DefaultConfiguration->GetObject("WebServer", config_obj);
		Configuration->SetObject("WebServer", config_obj);
		makeNewKey = true;
	} else
	{
		Configuration->GetObject("WebServer", config_obj);
		DefaultConfiguration->GetObject("WebServer", default_obj);
		if (!config_obj->HasInteger("Port"))
		{
			int32 val;
			default_obj->GetInteger("Port", val);
			config_obj->SetInteger("Port", val);
		}
		if (!config_obj->HasString("AccessSecret"))
		{
			makeNewKey = true;
		}
	}
	
	if (!Configuration->HasObject("World"))
	{
		DefaultConfiguration->GetObject("World", config_obj);
		Configuration->SetObject("World", config_obj);
	} else
	{
		Configuration->GetObject("World", config_obj);
		DefaultConfiguration->GetObject("World", default_obj);
		if (!config_obj->HasBoolean("AutoLoadSave"))
		{
			bool val;
			default_obj->GetBoolean("AutoLoadSave", val);
			config_obj->SetBoolean("AutoLoadSave", val);
		}
		if (!config_obj->HasInteger("AdminPassword"))
		{
			FString val;
			default_obj->GetString("AdminPassword", val);
			config_obj->SetString("AdminPassword", val);
		}
		if (!config_obj->HasString("SaveFileName"))
		{
			FString val;
			default_obj->GetString("SaveFileName", val);
			config_obj->SetString("SaveFileName", val);
		}
		if (!config_obj->HasString("GamePassword"))
		{
			FString val;
			default_obj->GetString("GamePassword", val);
			config_obj->SetString("GamePassword", val);
		}
		
	}

	//Make key if needed
	
	if (makeNewKey)
	{
		FString Result = "";
		for (uint32 i = 0; i < 32; ++i) {
			nextNumber:
			int32 IntRes = FMath::RandRange(48, 70);;
			if ((IntRes >= 58 && IntRes <= 64))
			{
				goto nextNumber;
			}			
			Result +=  (char)IntRes;
		}
		UBlueprintJsonObject* WebServer;
		Configuration->GetObject("WebServer", WebServer);
		WebServer->SetString("AccessSecret", Result);
		Configuration->SetObject("WebServer", WebServer);
	}
	
	if (!FFileHelper::SaveStringToFile(Configuration->Stringify(true), *AWebFilePath))
	{
		//ERROR SAVING
		UE_LOG(LogSFServerHTTP, Log, TEXT("Error Saving Config File"));
	}	
}

void AWebServer::StartServer(int32 Port)
{
	if (!GetWorld()->IsServer())
	{
		return;
	}
	auto HttpServerModule = &FHttpServerModule::Get();
	TSharedPtr<IHttpRouter> HttpRouter = HttpServerModule->GetHttpRouter(Port);
	// One point to note here is that the bottom layer does not support different request methods with the same http path configuration

	WebServerInstance = this;
	WebServerWorldInstance = this->GetWorld();

	HttpRouter->BindRoute(FHttpPath(TEXT("/health")), EHttpServerRequestVerbs::VERB_GET, CreateHandler([this](const FHttpServerRequest& Request)
		{
			return HealthCheck(Request);
		}));

	HttpRouter->BindRoute(FHttpPath(TEXT("/status")), EHttpServerRequestVerbs::VERB_GET, CreateHandler([this](const FHttpServerRequest& Request)
		{
			return StatusMessage(Request, this);
		}));

	HttpRouter->BindRoute(FHttpPath(TEXT("/command")), EHttpServerRequestVerbs::VERB_POST, CreateHandler([this](const FHttpServerRequest& Request)
		{
			return ProcessCommand(Request);
		}));

	HttpServerModule->StartAllListeners();
}

void AWebServer::StopServer()
{
	if (!GetWorld()->IsServer())
	{
		return;
	}
	auto HttpServerModule = &FHttpServerModule::Get();
	HttpServerModule->StopAllListeners();

	
	FString AWebFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + TEXT("/Configs/SFServerManager.cfg");
	if (Configuration && !FFileHelper::SaveStringToFile(Configuration->Stringify(true), *AWebFilePath))
	{
		//ERROR SAVING
		UE_LOG(LogSFServerHTTP, Log, TEXT("Error Saving Config File"));
	}
}

FHttpRequestHandler AWebServer::CreateHandler(const TFunction<TUniquePtr<FHttpServerResponse>(const FHttpServerRequest&)>& HttpResponder)
{
	return [HttpResponder](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
		auto Response = HttpResponder(Request);
		if (Response == nullptr)
		{
			return false;
		}
		OnComplete(MoveTemp(Response));
		return true;
	};
}

TUniquePtr<FHttpServerResponse> AWebServer::HealthCheck(const FHttpServerRequest& Request)
{
	if (DebugLogging) UE_LOG(LogSFServerHTTP, Log, TEXT("Health Check"));
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Health Check Successfully!"));
	}
	return SuccessResponse("Health Check Successfully!");
}

TUniquePtr<FHttpServerResponse> AWebServer::SuccessResponse(FString Message)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("message"), Message);
	JsonObject->SetBoolField(TEXT("success"), true);
	JsonObject->SetNumberField(TEXT("code"), (double)200);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return FHttpServerResponse::Create(JsonString, TEXT("application/json"));
}


TUniquePtr<FHttpServerResponse> AWebServer::SuccessResponse(TSharedPtr<FJsonObject> Data, FString Message)
{
	return JsonResponse(Data, Message, true, 200);
}

TUniquePtr<FHttpServerResponse> AWebServer::ErrorResponse(FString Message)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("message"), Message);
	JsonObject->SetBoolField(TEXT("success"), false);
	JsonObject->SetNumberField(TEXT("code"), (double)400);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return FHttpServerResponse::Create(JsonString, TEXT("application/json"));
}

TUniquePtr<FHttpServerResponse> AWebServer::ErrorResponse(TSharedPtr<FJsonObject> Data, FString Message)
{
	return JsonResponse(Data, Message, false, 400);
}

TUniquePtr<FHttpServerResponse> AWebServer::JsonResponse(TSharedPtr<FJsonObject> Data, FString Message, bool Success, int32 Code)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetObjectField(TEXT("data"), Data);
	JsonObject->SetStringField(TEXT("message"), Message);
	JsonObject->SetBoolField(TEXT("success"), Success);
	JsonObject->SetNumberField(TEXT("code"), (double)Code);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return FHttpServerResponse::Create(JsonString, TEXT("application/json"));
}

TSharedPtr<FJsonObject> AWebServer::GetRequestJsonBody(const FHttpServerRequest& Request)
{
	// check if content type is application/json
	bool IsUTF8JsonContent = IsUTF8JsonRequestContent(Request);
	if (!IsUTF8JsonContent)
	{
		if (DebugLogging) UE_LOG(LogSFServerHTTP, Warning, TEXT("caught request not in utf-8 application/json body content!"));
		return nullptr;
	}

	// body to utf8 string
	TArray<uint8> RequestBodyBytes = Request.Body;
	FString RequestBodyString = FString(UTF8_TO_TCHAR(RequestBodyBytes.GetData()));

	if (RequestBodyString.Contains("}"))
	{
		if (!RequestBodyString.EndsWith("}"))
		{
			//Ok, The decoding on this seems to be really sloppy. Trim the end.
			
			int32 Index;
			RequestBodyString.FindLastChar('}', Index);
			RequestBodyString = RequestBodyString.Left(Index + 1);
		}		
	}

	// string to json
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RequestBodyString);
	TSharedPtr<FJsonObject> RequestBody;
	if (!FJsonSerializer::Deserialize(JsonReader, RequestBody))
	{
		if (DebugLogging) UE_LOG(LogSFServerHTTP, Warning, TEXT("failed to parse request string to json: %s"), *RequestBodyString);
		return nullptr;
	}
	return RequestBody;
}

bool AWebServer::IsUTF8JsonRequestContent(const FHttpServerRequest& Request)
{
	bool bIsUTF8JsonContent = false;
	for (auto& HeaderElem : Request.Headers)
	{
		if (HeaderElem.Key.ToLower() == TEXT("content-type"))
		{
			for (auto& Value : HeaderElem.Value)
			{
				auto LowerValue = Value.ToLower();
				if (LowerValue.StartsWith(TEXT("charset=")) && LowerValue != TEXT("charset=utf-8"))
				{
					return false;
				}
				if (LowerValue == TEXT("application/json") || LowerValue == TEXT("text/json"))
				{
					bIsUTF8JsonContent = true;
				}
			}
		}
	}
	return bIsUTF8JsonContent;
}





TUniquePtr<FHttpServerResponse> AWebServer::StatusMessage(const FHttpServerRequest& Request, AWebServer* WebServer)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	TSharedPtr<FJsonObject> GameObject = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> PluginsArray;
	TSharedPtr<FJsonObject> WorldObject = MakeShareable(new FJsonObject());


	GameObject->SetStringField(TEXT("Branch"), FEngineVersion::Current().GetBranch());
	GameObject->SetStringField(TEXT("BranchDescriptor"), FEngineVersion::Current().GetBranchDescriptor());

	GameObject->SetStringField(TEXT("Version"), FEngineVersion::Current().ToString());
	GameObject->SetNumberField(TEXT("Changelist"), FEngineVersion::Current().GetChangelist());

	//GameObject->SetBoolField(TEXT("success"), true);
	//GameObject->SetNumberField(TEXT("code"), (double)200);

	
	//Populate Plugins
	
	FString PluginFolderPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectModsDir());
	TArray<FString> Results;
	FPlatformFileManager* myFileManager = new FPlatformFileManager;

	// Reference to the FFileMatch class that has the visit override for the Visitor needed for iterate directory
	FDirectoryLister FileMatch( Results, FString(""), false, false );
			
	// Check if the directory supplied exists
	if ( FPlatformFileManager::Get().GetPlatformFile().DirectoryExists( *PluginFolderPath ) )
	{
		// Directory exists so loop through the files
		myFileManager->Get().GetPlatformFile().IterateDirectory(*PluginFolderPath, FileMatch);

	}


	UE_LOG(LogSFServerHTTP, Log, TEXT("HERE WE GO"));
	UBlueprintJsonObject* upluginFile = UBlueprintJsonObject::Create();
	for (FString FileFolder : Results)
	{
		TSharedPtr<FJsonObject> Plugin = MakeShareable(new FJsonObject());

		FString UPluginFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectModsDir()) + TEXT("/" + FileFolder + "/" + FileFolder + ".uplugin");
	
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*UPluginFilePath))
		{
			FString Result;
			if (FFileHelper::LoadFileToString(Result, *UPluginFilePath))
			{			
				if (UBlueprintJsonObject::Parse(Result, upluginFile))
				{
					FString Strings[] = {"FriendlyName", "Description", "VersionName", "SemVersion", "CreatedBy", "CreatedByURL"};

					FString val;
					for (FString String : Strings)
					{
						if (upluginFile->HasString(String))
						{
							upluginFile->GetString(String, val);
							Plugin->SetStringField(String,val);
						}
					}
					
					TSharedPtr<FJsonValueObject> PluginValue = MakeShareable(new FJsonValueObject(Plugin));
					PluginsArray.Add(PluginValue);
				}
			}
		}
	}
	
	try
	{
		if (WebServerInstance && !WebServerInstance->HasActorBegunPlay())
		{
			if (WebServerInstance && !WebServerInstance->HasActorBegunPlay())
			{
				JsonObject->SetStringField("INSTANCE", "INVALID INSTANCE OBJECT");				
			}
			if (!WebServerWorldInstance || (WebServerWorldInstance && !IsValid(WebServerWorldInstance)))
			{
				JsonObject->SetStringField("WorldInstance", "INVALID WorldInstance OBJECT");				
			}
			
			JsonObject->SetObjectField(TEXT("Game"), GameObject);
			JsonObject->SetArrayField(TEXT("Plugins"), PluginsArray);
			JsonObject->SetObjectField(TEXT("World"), WorldObject);
			JsonObject->SetStringField("ERROR", "INVALID WEBSERVER OBJECT");
			return SuccessResponse(JsonObject, "StatusResponse");
		} else
		{
			if (!WebServerWorldInstance || (WebServerWorldInstance && !IsValid(WebServerWorldInstance)))
			{
				JsonObject->SetStringField("WorldInstance", "INVALID WorldInstance OBJECT");				
			}
		}
		if (WebServerWorldInstance)
		{
			if (IsValid(WebServerWorldInstance))
			{
				WorldObject->SetStringField("MapName", WebServerWorldInstance->GetMapName());
				if (WorldObject->GetStringField("MapName").Contains("Persistent_Level"))
				{
					WorldObject->SetStringField("MapType", "GameWorld");
				}
				else
				{
					WorldObject->SetStringField("MapType", "MenuWorld");
				}
				WorldObject->SetNumberField("PlayerCount", WebServerWorldInstance->GetNumPlayerControllers());
			}
			else
			{
				WorldObject->SetStringField("ERROR", "CANNOT GET WORLD");
			}
		}
		else
		{
			WorldObject->SetStringField("ERROR", "BAD REFERENCE TO WebServerWorldInstance");
		}
	}
	catch (...)
	{
		WorldObject->SetStringField("ERROR", "Exception Thrown");
	}

	
	JsonObject->SetObjectField(TEXT("Game"), GameObject);
	JsonObject->SetArrayField(TEXT("Plugins"), PluginsArray);
	JsonObject->SetObjectField(TEXT("World"), WorldObject);

	try
	{
		if (WebServerInstance)
		{
			UBlueprintJsonObject* BPJson = UBlueprintJsonObject::FromSharedPointer(JsonObject);

			UBlueprintJsonObject* BPJsonOut = WebServerInstance->ProcessStatusMessage(BPJson);

			//UFunction* func = WebServerInstance->FindFunction(FName(TEXT("Processjsonobject")));
			////uint8* Buffer = (uint8*)FMemory_Alloca(func->ParmsSize);
			////FMemory::Memzero(Buffer, func->ParmsSize);

			//if (func != nullptr)
			//{

			//	//FStructOnScope FuncParam(func);
			//	//UProperty* ReturnProp = nullptr;

			//	//for (TFieldIterator<UProperty> It(func); It; ++It)
			//	//{
			//	//	UProperty* Prop = *It;
			//	//	if (Prop->HasAnyPropertyFlags(CPF_ReturnParm))
			//	//	{
			//	//		ReturnProp = Prop;
			//	//	}
			//	//	else
			//	//	{
			//	//		*It->ContainerPtrToValuePtr<UBlueprintJsonObject*>(Buffer) = BPJson;
			//	//		//FillParam here            
			//	//	}
			//	//}


			//	struct Params
			//	{
			//		// Ordered parameters here
			//		UBlueprintJsonObject* Arg1;

			//		// Return value last
			//		UBlueprintJsonObject* RetVal;
			//	};

			//	Params Parameters;
			//	Parameters.Arg1 = BPJson;

			//	//WebServer->ProcessEvent(func, FuncParam.GetStructMemory());
			//	WebServerInstance->ProcessEvent(func, &Parameters);


			//	if (Parameters.RetVal)
			//	{
			//		UBlueprintJsonObject* BPJsonOut = Parameters.RetVal;
			//		//UBlueprintJsonObject* BPJsonOut = WebServer->ProcessJsonObject(BPJson);

			//		if (BPJsonOut && IsValid(BPJsonOut))
			//		{
			//			JsonObject = BPJsonOut->ToSharedPointer();
			//		} //else
			//		{
			//			JsonObject->SetStringField("ERROR", "CANNOT GET JSON OUTPUT");
			//		}
			//		
			//	}
			//}
		}
		else
		{
			JsonObject->SetStringField("ERROR", "Invalid WebServer Reference");
		}
	}
	catch (...)
	{
		JsonObject->SetStringField("ERROR", "EXCEPTION: CANNOT GET JSON OUTPUT");
	}


	return SuccessResponse(JsonObject, "StatusResponse");

}


TUniquePtr<FHttpServerResponse> AWebServer::ProcessCommand(const FHttpServerRequest& Request)
{
	if (!IsUTF8JsonRequestContent(Request))
	{
		return ErrorResponse("Request Body is not a Json Payload");
	}

	TSharedPtr<FJsonObject> JsonObject = GetRequestJsonBody(Request);

	if (!JsonObject)
	{
		return ErrorResponse("Missing Request Payload");
	}

	UBlueprintJsonObject* BPJson = UBlueprintJsonObject::FromSharedPointer(JsonObject);

	if (!BPJson)
	{
		return ErrorResponse("Internal Conversion Error. Payload likely corrupt. Please Resend.");
	}

	UBlueprintJsonObject* BPJsonOut = WebServerInstance->ProcessJsonObject(BPJson);

	JsonObject = BPJsonOut->ToSharedPointer();

	return SuccessResponse(JsonObject, "StatusResponse");
}

// Sets default values for this component's properties
AWebServer::AWebServer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;

	//AWebServer::Instance = this;
	// ...
}


// Called when the game starts
void AWebServer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogSFServerHTTP, Log, TEXT("Starting up in world %s"), *(this->GetWorld()->GetMapName()));

	//Moved to BP for easier editing
	//StartServer(8033);

	//AWebServer::Instance = this;
	this->GetWorld()->OnBeginTearingDown().AddLambda([this]()
		{
			UE_LOG(LogSFServerHTTP, Log, TEXT("Shutting down in world %s"), *(this->GetWorld()->GetMapName()));
			StopServer();
		});

}


// Called every frame
void AWebServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ...
}