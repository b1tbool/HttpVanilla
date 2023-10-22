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

ERequestHttpStatus UHttpService::RequestHttp(EHttpRequestType InRequestType, const FString& InUrl, const TSharedPtr<FJsonObject>& InMessage)
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

	//HttpRequestPtr->SetHeader()

	HttpRequestPtr->OnProcessRequestComplete().BindLambda(
		[&](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool IsConnectedSuccessfully)
		{
			if (HttpRequestPtr.IsValid())
			{
				HttpRequestPtr->OnProcessRequestComplete().Unbind();
			}

			HttpRequestPtr.Reset();

			TSharedPtr<FJsonObject> ResponseObject;

			if (IsConnectedSuccessfully)
			{
				UE_LOG(HttpService, Log, TEXT("Response: %s"), *InResponse->GetContentAsString());
				ResponseObject = PackageResponseToJsonObject(InResponse);
			}
			else
			{
				UE_LOG(HttpService, Error, TEXT("Connection not successfull!"));
			}

			OnRequestComplete().Broadcast(ResponseObject, IsConnectedSuccessfully);
		});
	HttpRequestPtr->ProcessRequest();

	UE_LOG(HttpService, Log, TEXT("Process request..."));

	return ERequestHttpStatus::Success;
}

TSharedPtr<FJsonObject> UHttpService::PackageResponseToJsonObject(FHttpResponsePtr InResponse)
{
	TSharedPtr<FJsonObject> Result;

	if (!InResponse.IsValid())
	{
		return Result;
	}

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InResponse->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, Result);
	return Result;
}