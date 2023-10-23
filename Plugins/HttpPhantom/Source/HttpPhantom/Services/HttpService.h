#pragma once

#include "HttpPhantom/Services/Interfaces/ServiceInterface.h"
#include "Interfaces/IHttpRequest.h"
#include "HttpService.generated.h"

enum class ERequestHttpStatus : uint8;
enum class EHttpRequestType : uint8;

DECLARE_LOG_CATEGORY_EXTERN(HttpService, Log, All);

UCLASS()
class HTTPPHANTOM_API UHttpService : public UObject, public IServiceInterface
{
	GENERATED_BODY()

protected:
	DECLARE_EVENT_TwoParams(UHttpService, FOnRequestComplete, const FString& /*InResponse*/, bool /*IsConnectedSuccessfully*/);

public:
	void Deinit() override;

	bool IsServiceCompleteRequests() const override;

	void UnsubscribeFromAll(UObject* InObject) override;

	ERequestHttpStatus RequestHttp(EHttpRequestType InRequestType, const FString& InUrl, const TMap<FString, FString>& InHeaders, const TSharedPtr<FJsonObject>& InMessage);

	FOnRequestComplete& OnRequestComplete() { return RequestComplete; }

private:
	FOnRequestComplete RequestComplete;

	FHttpRequestPtr HttpRequestPtr;
};