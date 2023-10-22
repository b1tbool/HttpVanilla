#pragma once

#include "CoreMinimal.h"
#include "HttpRequestData.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EHttpRequestType : uint8
{
	None	= 0 UMETA(Hidden),
	Get		= 1 UMETA(DisplayName = "GET"),
	Post	= 2 UMETA(DisplayName = "POST"),
	Put		= 3 UMETA(DisplayName = "PUT"),
	Delete	= 4 UMETA(DisplayName = "DELETE"),
	Connect	= 5 UMETA(DisplayName = "CONNECT"),
	Options	= 6 UMETA(DisplayName = "OPTIONS"),
};