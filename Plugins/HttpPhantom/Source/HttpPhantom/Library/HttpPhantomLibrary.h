#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpPhantomLibrary.generated.h"

enum class EHttpRequestType : uint8;
class UHttpPhantomSubsystem;

UCLASS()
class HTTPPHANTOM_API UHttpPhantomLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UHttpPhantomSubsystem* GetHttpPhantomSubsystem(const UObject* InWCO);
	static FString GetStringOfHttpRequestType(EHttpRequestType InRequestType);
	static FString JsonObjectToString(const TSharedPtr<FJsonObject>& InJsonObject);
};