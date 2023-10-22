#pragma once

#include "GameFramework/GameMode.h"
#include "StartupGM.generated.h"

class UHttpRequestWidget;

UCLASS()
class HTTPVANILLA_API AStartupGM : public AGameMode
{
	GENERATED_BODY()

public:
	void StartPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UHttpRequestWidget> HttpRequestWidgetClass = nullptr;
};