#include "HttpService.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpPhantom/Data/HttpRequestData.h"
#include "HttpPhantom/Library/HttpPhantomLibrary.h"
#include "HttpPhantom/Data/Status/RequestHttpStatus.h"

DEFINE_LOG_CATEGORY(HttpService);

void UHttpService::Deinit()
{
	IServiceInterface::Deinit();

	if (HttpRequestPtr.IsValid())
	{
		HttpRequestPtr->CancelRequest();
	}

	OnRequestComplete().Clear();
}

bool UHttpService::IsServiceCompleteRequests() const
{
	return !HttpRequestPtr.IsValid();
}

void UHttpService::UnsubscribeFromAll(UObject* InObject)
{
	IServiceInterface::UnsubscribeFromAll(InObject);

	if (!InObject)
	{
		return;
	}

	OnRequestComplete().RemoveAll(InObject);
}

ERequestHttpStatus UHttpService::RequestHttp(EHttpRequestType InRequestType, const FString& InUrl, const TMap<FString, FString>& InHeaders, const TSharedPtr<FJsonObject>& InMessage)
{
	if (InRequestType == EHttpRequestType::None)
	{
		UE_LOG(HttpService, Error, TEXT("Request type is none!"));
		return ERequestHttpStatus::NoneRequest;
	}

	if (InUrl.IsEmpty())
	{
		UE_LOG(HttpService, Error, TEXT("URL is empty!"));
		return ERequestHttpStatus::EmptyUrl;
	}

	HttpRequestPtr = FHttpModule::Get().CreateRequest().ToSharedPtr();
	HttpRequestPtr->SetURL(InUrl);
	HttpRequestPtr->SetVerb(UHttpPhantomLibrary::GetStringOfHttpRequestType(InRequestType).ToUpper());

	if (InMessage.IsValid())
	{
		FString RequestBody;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
		FJsonSerializer::Serialize(InMessage.ToSharedRef(), Writer);
		HttpRequestPtr->SetContentAsString(RequestBody);
	}

	if (InRequestType == EHttpRequestType::Post)
	{
		HttpRequestPtr->SetHeader("Content-Type", "application/json");
	}

	for (auto Header : InHeaders)
	{
		if (Header.Key.IsEmpty() || Header.Key.IsEmpty())
		{
			continue;
		}

		HttpRequestPtr->SetHeader(Header.Key, Header.Value);
	}

	HttpRequestPtr->OnProcessRequestComplete().BindLambda(
		[&](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool IsConnectedSuccessfully)
		{
			if (HttpRequestPtr.IsValid())
			{
				HttpRequestPtr->OnProcessRequestComplete().Unbind();
			}

			HttpRequestPtr.Reset();

			FString Response = InResponse->GetContentAsString();;

			if (IsConnectedSuccessfully)
			{
				UE_LOG(HttpService, Log, TEXT("Response: %s"), *Response);
			}
			else
			{
				UE_LOG(HttpService, Error, TEXT("Connection not successfull!"));
				UE_LOG(HttpService, Error, TEXT("Response: %s"), *Response);
			}

			OnRequestComplete().Broadcast(Response, IsConnectedSuccessfully);
		});
	HttpRequestPtr->ProcessRequest();

	UE_LOG(HttpService, Log, TEXT("Process request..."));

	return ERequestHttpStatus::Success;
}