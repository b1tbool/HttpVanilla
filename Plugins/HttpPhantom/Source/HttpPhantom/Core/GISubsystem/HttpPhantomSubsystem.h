#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpPhantomSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(HttpPhantom, Log, All);

enum class ERequestHttpStatus : uint8;
enum class EHttpRequestType : uint8;
class IServiceInterface;

UCLASS()
class HTTPPHANTOM_API UHttpPhantomSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestCompletePhantom, const FString& /*InResponse*/, bool /*IsConnectedSuccessfully*/);

public:
	FOnRequestCompletePhantom& OnRequestCompletePhantom() { return RequestCompletePhantom; }

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	ERequestHttpStatus RequestHttp(EHttpRequestType InRequestType, const FString& InUrl, const TMap<FString, FString>& InHeaders, const TSharedPtr<FJsonObject>& InMessage);

private:
	int32 CleanServices();

	void RemoveUsedService();

	template<typename T>
	T* CreateNewService()
	{
		T* NewService = NewObject<T>(this);
		ServicesArray.Add(NewService);
		return NewService;
	}

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<IServiceInterface*> ServicesArray = {};

private:
	FOnRequestCompletePhantom RequestCompletePhantom;
};

template<typename TSourceDelegate, typename TTargetDelegate, typename TPred = TFunction<void()>, TEMPLATE_REQUIRES(TIsInvocable<TPred>::Value)>
void RedirectDelegate(UObject* InContext, TSourceDelegate& InSourceDelegate, TTargetDelegate& InTargetDelegate, TPred InPredicate = [] {})
{
	InSourceDelegate.AddWeakLambda(
		InContext,
		[&InTargetDelegate, InPredicate](auto&&... InParams)
		{
			InPredicate();
			InTargetDelegate.Broadcast(Forward<decltype(InParams)>(InParams)...);
		});
}