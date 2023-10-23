#pragma once

#include "UObject/Interface.h"
#include "ServiceInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UServiceInterface : public UInterface
{
	GENERATED_BODY()
};

class HTTPPHANTOM_API IServiceInterface
{
	GENERATED_BODY()

public:
	virtual void Init() {}
	virtual void Deinit() {}

	virtual void UnsubscribeFromAll(UObject* InObject) { }
	virtual bool IsServiceCompleteRequests() const { return false; }
};