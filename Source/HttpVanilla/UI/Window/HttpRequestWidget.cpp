#include "HttpRequestWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "HttpPhantom/Data/HttpRequestData.h"
#include "HttpPhantom/Library/HttpPhantomLibrary.h"
#include "HttpPhantom/Data/Status/RequestHttpStatus.h"
#include "HttpPhantom/Core/GISubsystem/HttpPhantomSubsystem.h"

void UHttpRequestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnPressed.AddDynamic(this, &UHttpRequestWidget::HandleClickedLoginButton);
	}
}

void UHttpRequestWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (StartButton)
	{
		StartButton->OnPressed.RemoveAll(this);
	}

	if (UrlEditableText)
	{
		UrlEditableText->SetText(FText::FromString("https://jsonplaceholder.typicode.com/todos/1"));
	}
}

void UHttpRequestWidget::HandleClickedLoginButton()
{
	UHttpPhantomSubsystem* HttpPhantomSubsystem = UHttpPhantomLibrary::GetHttpPhantomSubsystem(GetWorld());
	if (!HttpPhantomSubsystem)
	{
		return;
	}

	if (!UrlEditableText || !LoginEditableText || !PasswordEditableText)
	{
		return;
	}

	if (UrlEditableText->GetText().IsEmpty())
	{
		return;
	}

	if (LoginEditableText->GetText().IsEmpty())
	{
		return;
	}

	if (PasswordEditableText->GetText().IsEmpty())
	{
		return;
	}

	const TSharedPtr<FJsonObject> Message = MakeShareable(new FJsonObject);

	HttpPhantomSubsystem->OnRequestCompletePhantom().AddUObject(this, &UHttpRequestWidget::HandleRequestComplete);
	HttpPhantomSubsystem->RequestHttp(EHttpRequestType::Get,UrlEditableText->GetText().ToString(), Message);
}

void UHttpRequestWidget::AddText(const FString& InString) const
{
	if (!ResultTextBlock)
	{
		return;
	}

	ResultTextBlock->SetText(FText::FromString(ResultTextBlock->GetText().ToString() + '\n' + InString));
}

void UHttpRequestWidget::HandleRequestComplete(const TSharedPtr<FJsonObject>& InResponse, bool IsConnectedSuccessfully)
{
	AddText(IsConnectedSuccessfully
		? UHttpPhantomLibrary::JsonObjectToString(InResponse)
		: "Error: Connection not successfully!");
}