#include "HTTPClient.h"

DEFINE_LOG_CATEGORY(LogHTTPClient);

const FString EnumToString(const TCHAR* Enum, int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
	if (!EnumPtr)
		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
	return EnumPtr->GetDisplayNameTextByIndex(EnumValue).ToString();
#else
	return EnumPtr->GetEnumName(EnumValue);
#endif
}


// Sets default values
UHTTPClient::UHTTPClient(const FObjectInitializer &init) : UActorComponent(init)
{
	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();
}


void UHTTPClient::HttpGETCall(FString URL)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&, this, URL]()
    {
        TSharedRef<IHttpRequest> Request = Http->CreateRequest();
        Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseReceived);
        //This is the url on which to process the request
        Request->SetURL(URL);
        Request->SetVerb("GET");
        Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
        Request->ProcessRequest();
        UE_LOG(LogHTTPClient, Display, TEXT("SFD GET Request Started %s"), *URL);
    });
}

void UHTTPClient::HttpPOSTCall(FString URL, FString Payload)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&, this, URL, Payload]()
	{
	    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseReceived);
		//This is the url on which to process the request
		Request->SetURL(URL);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString(Payload);
		Request->ProcessRequest();
		UE_LOG(LogHTTPClient, Display, TEXT("SFD POST Request Started %s"), *URL);
		
	});
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bRequestCompleted)
{
	if (!Response.IsValid())
	{
		FRequestStruct ReqStruct;
		ReqStruct.URL = Request->GetURL();
		ReqStruct.Headers = Request->GetAllHeaders();
		ReqStruct.Verb = Request->GetVerb();
		AsyncTask(ENamedThreads::GameThread, [&, ReqStruct, bRequestCompleted]()
	    {
			FRequestStruct ReqStruct_ = ReqStruct;
			FResponseStruct RespStruct;
			OnReceivedResponse.Broadcast(ReqStruct_, RespStruct, false, ERequestResultState::FAILURE);
	    });
		return;
	}
	
	FString ResponseString = Response->GetContentAsString();

	FRequestStruct ReqStruct;
	FResponseStruct RespStruct;

	ReqStruct.URL = Request->GetURL();
	ReqStruct.Headers = Request->GetAllHeaders();
	ReqStruct.Verb = Request->GetVerb();
	
	switch (Request->GetStatus())
	{
		case EHttpRequestStatus::NotStarted : {
			ReqStruct.Status = EHttpRequestStatusEnum::NotStarted;
			break;
		}
		case EHttpRequestStatus::Processing : {
				ReqStruct.Status = EHttpRequestStatusEnum::Processing;
				break;
		}
		case EHttpRequestStatus::Failed : {
			ReqStruct.Status = EHttpRequestStatusEnum::Failed;
			break;
		}
		case EHttpRequestStatus::Failed_ConnectionError : {
			ReqStruct.Status = EHttpRequestStatusEnum::Failed_ConnectionError;
			break;
		}
		case EHttpRequestStatus::Succeeded : {
			ReqStruct.Status = EHttpRequestStatusEnum::Succeeded;
			break;
		}
		default: ;
	}

	RespStruct.ResponseCode = Response->GetResponseCode();
	RespStruct.Headers = Response->GetAllHeaders();
	RespStruct.URL = Response->GetURL();
	
	RespStruct.ContentType = Response->GetContentType();

	FString Content = Response->GetContentAsString();

	if (RespStruct.ContentType.ToLower().Contains(TEXT("application/json")))
	{
		//Json Fix
		Content = Content.ReplaceEscapedCharWithChar();		
		Content = Content.TrimQuotes();
	}

	RespStruct.ContentString = *(Content);

	UE_LOG(LogHTTPClient, Display, TEXT("SFD Request Complete %s %d"), *ReqStruct.URL, bRequestCompleted);
	
	AsyncTask(ENamedThreads::GameThread, [&, ReqStruct, RespStruct, bRequestCompleted]()
    {
		FRequestStruct ReqStruct_ = ReqStruct;
		FResponseStruct RespStruct_ = RespStruct;
		bool bRequestCompleted_ = bRequestCompleted;
		
		if (!bRequestCompleted_)
		{
			OnReceivedResponse.Broadcast(ReqStruct_, RespStruct_, bRequestCompleted_, ERequestResultState::FAILURE);
			return;
		}
		
        if (RespStruct.ResponseCode >= 200 && RespStruct.ResponseCode <= 208)
        {
        	OnReceivedResponse.Broadcast(ReqStruct_, RespStruct_, bRequestCompleted_, ERequestResultState::SUCCESS);
        } else
        {
        	OnReceivedResponse.Broadcast(ReqStruct_, RespStruct_, bRequestCompleted_, ERequestResultState::FAILURE);
        }
    });

}

//TODO print headers on own lines
void UHTTPClient::LogRequestToSML(FRequestStruct Request)
{
	ERequestResultState num = static_cast<ERequestResultState>(Request.Status.GetValue());
	FString enumVal = EnumToString(TEXT("EMyEnum"), static_cast<uint8>(num));
	
	UE_LOG(LogHTTPClient, Display, TEXT("SFD Request - %s %s %s"), *Request.Verb, *Request.URL, *enumVal);

	for (FString Element : Request.Headers)
	{
		UE_LOG(LogHTTPClient, Display, TEXT("SFD Request Header - %s"), *Element);
	}	
}

void UHTTPClient::LogResponseToSML(FResponseStruct Response)
{
	const FString Logline = FString::Printf(TEXT("SFD Response - %i %s %s %s"), Response.ResponseCode, *Response.URL, *Response.ContentType, *Response.ContentString);
	//SML::Logging::info(*Logline);
	
	for (FString Element : Response.Headers)
	{
		//SML::Logging::info(*FString::Printf(TEXT("SFD Response Header - %s"), *Element));
		UE_LOG(LogHTTPClient, Display, TEXT("SFD Response Header - %s"), *Element);
	}
}

bool UHTTPClient::IsJsonContentUTF8(FResponseStruct Response)
{
	for (FString Element : Response.Headers)
	{
		if (Element.ToLower().StartsWith(TEXT("content-type:")))
		{
			if (Element.ToLower().Contains(TEXT("application/json")) && Element.Contains(TEXT("charset=utf-8")))
			{
				return true;
			}
		}
	}

	return false;
}