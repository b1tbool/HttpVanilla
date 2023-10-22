#include "HttpPhantomSubsystem.h"
#include "HttpPhantom/Services/HttpService.h"
#include "HttpPhantom/Services/Interfaces/ServiceInterface.h"

DEFINE_LOG_CATEGORY(HttpPhantom);

void UHttpPhantomSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const int32 Cleaned = CleanServices();

	UE_LOG(HttpPhantom, Log, TEXT("Cleaned services on Initialize: %d"), Cleaned);
}

void UHttpPhantomSubsystem::Deinitialize()
{
	Super::Deinitialize();

	const int32 Cleaned = CleanServices();

	UE_LOG(HttpPhantom, Log, TEXT("Cleaned services on Deinitialize: %d"), Cleaned);
}

ERequestHttpStatus UHttpPhantomSubsystem::RequestHttp(EHttpRequestType InRequestType, const FString& InUrl, const TSharedPtr<FJsonObject>& InMessage)
{
	const auto RemoveUsedSerivce = [&]()
		{
			RemoveUsedService();
			OnRequestCompletePhantom().RemoveAll(this);
		};

	UHttpService* NewService = CreateNewService<UHttpService>();
	RedirectDelegate(this, NewService->OnRequestComplete(), OnRequestCompletePhantom(), RemoveUsedSerivce);
	return NewService->RequestHttp(InRequestType, InUrl, InMessage);
}

int32 UHttpPhantomSubsystem::CleanServices()
{
	int32 Result = 0;

	for (IServiceInterface* Service : ServicesArray)
	{
		if (!Service)
		{
			continue;
		}

		Service->Deinit();
		++Result;
	}

	ServicesArray.Empty();

	OnRequestCompletePhantom().Clear();

	return Result;
}

void UHttpPhantomSubsystem::RemoveUsedService()
{
	int32 IndexService = -1;

	for (int32 Index = 0; Index < ServicesArray.Num(); ++Index)
	{
		if (!ServicesArray[Index])
		{
			continue;
		}

		if (ServicesArray[Index]->IsServiceCompleteRequests())
		{
			IndexService = Index;
			break;
		}
	}

	if (ServicesArray.IsValidIndex(IndexService))
	{
		ServicesArray.RemoveAt(IndexService);
	}
}