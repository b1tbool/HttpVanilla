#include "StartupGM.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "HttpVanilla/UI/Window/HttpRequestWidget.h"

void AStartupGM::StartPlay()
{
	Super::StartPlay();

	if (!HttpRequestWidgetClass.Get())
	{
		return;
	}

	UHttpRequestWidget* HttpRequestWidget = CreateWidget<UHttpRequestWidget>(GetWorld(), HttpRequestWidgetClass);
	if (!HttpRequestWidget)
	{
		return;
	}

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(GetWorld(), 0), HttpRequestWidget);
	HttpRequestWidget->AddToViewport();
}