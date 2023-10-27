#include "HttpRequestWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Kismet/KismetSystemLibrary.h"
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

	if (PostEditableText)
	{
		PostEditableText->SetText(FText::FromString("https://stanzza-api.aicrobotics.ru/api/auth/login"));
	}

	if (GetEditableText)
	{
		GetEditableText->SetText(FText::FromString("https://stanzza-api.aicrobotics.ru/api/v1/catalog/tree"));
	}

	/*if (LoginEditableText)
	{
		LoginEditableText->SetText(FText::FromString(""));
	}*/

	/*if (PasswordEditableText)
	{
		PasswordEditableText->SetText(FText::FromString(""));
	}*/
}

void UHttpRequestWidget::HandleClickedLoginButton()
{
	if (!HttpPhantomSubsystem)
	{
		HttpPhantomSubsystem = UHttpPhantomLibrary::GetHttpPhantomSubsystem(GetWorld());
	}

	if (!PostEditableText || !LoginEditableText || !PasswordEditableText)
	{
		return;
	}

	if (PostEditableText->GetText().IsEmpty())
	{
		return;
	}

	if (GetEditableText->GetText().IsEmpty())
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
	Message->SetStringField("login", LoginEditableText->GetText().ToString());
	Message->SetStringField("password", PasswordEditableText->GetText().ToString());
	Message->SetStringField("fingerprint", /*UKismetSystemLibrary::GetDeviceId()*/"HelloWorld");

	TMap<FString, FString> Temp;
	Temp.Add("Accept", "application/json");

	HttpPhantomSubsystem->OnRequestCompletePhantom().AddUObject(this, &UHttpRequestWidget::HandleRequestComplete);
	HttpPhantomSubsystem->RequestHttp(EHttpRequestType::Post, PostEditableText->GetText().ToString(), Temp , Message);
}

void UHttpRequestWidget::AddText(const FString& InString) const
{
	if (!ResultTextBlock)
	{
		return;
	}

	ResultTextBlock->SetText(FText::FromString(ResultTextBlock->GetText().ToString() + '\n' + InString));
}

void UHttpRequestWidget::HandleRequestComplete(const FString& InResponse, bool IsConnectedSuccessfully)
{
	AddText(IsConnectedSuccessfully
		? InResponse
		: "Error: Connection not successfully!");

	if (!HttpPhantomSubsystem)
	{
		return;
	}

	HttpPhantomSubsystem->OnRequestCompletePhantom().RemoveAll(this);

	HttpPhantomSubsystem->OnRequestCompletePhantom().AddLambda([&](const FString& InResponse, bool IsConnectedSuccessfully)
	{
			AddText(IsConnectedSuccessfully
				? InResponse
				: "Error: Connection not successfully!");
	});
	HttpPhantomSubsystem->RequestHttp(EHttpRequestType::Get, GetEditableText->GetText().ToString(), {}, {});
}