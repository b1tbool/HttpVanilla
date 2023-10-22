#pragma once

#include "Blueprint/UserWidget.h"
#include "HttpRequestWidget.generated.h"

class UEditableText;
class UTextBlock;
class UButton;

UCLASS()
class HTTPVANILLA_API UHttpRequestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	UFUNCTION()
	void HandleClickedLoginButton();

	void AddText(const FString& InString) const;

private:
	void HandleRequestComplete(const TSharedPtr<FJsonObject>& InResponse, bool IsConnectedSuccessfully);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UEditableText* UrlEditableText = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UEditableText* LoginEditableText = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UEditableText* PasswordEditableText = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UButton* StartButton = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UTextBlock* ResultTextBlock = nullptr;
};