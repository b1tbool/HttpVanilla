#pragma once

#include "CoreMinimal.h"
#include "RequestHttpStatus.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class ERequestHttpStatus : uint8
{
	None		= 0,
	Success		= 1,
	NoneRequest = 2,
	EmptyUrl	= 3,
};