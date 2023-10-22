#include "HttpPhantomLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HttpPhantom/Data/HttpRequestData.h"

UHttpPhantomSubsystem* UHttpPhantomLibrary::GetHttpPhantomSubsystem(const UObject* InWCO)
{
	const UGameInstance* GI = UGameplayStatics::GetGameInstance(InWCO);
	if (!GI)
	{
		return nullptr;
	}

	return GI->GetSubsystem<UHttpPhantomSubsystem>();
}

FString UHttpPhantomLibrary::GetStringOfHttpRequestType(EHttpRequestType InRequestType)
{
	return StaticEnum<EHttpRequestType>()->GetNameStringByIndex((int32)InRequestType);
}

FString UHttpPhantomLibrary::JsonObjectToString(const TSharedPtr<FJsonObject>& InJsonObject)
{
	FString Result = "";

	if (!InJsonObject.IsValid())
	{
		return Result;
	}

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Result);
	FJsonSerializer::Serialize(InJsonObject.ToSharedRef(), Writer);

	return Result;
}