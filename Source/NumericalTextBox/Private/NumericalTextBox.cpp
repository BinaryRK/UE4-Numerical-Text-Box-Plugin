// Harry Katagis - 2017

#include "NumericalTextBox.h"


UNumericalTextBox::UNumericalTextBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUse_MinClamp = 1;
	MinValue = 0;
	
	bUse_MaxClamp = 0;
	MaxValue = 100;

	CurrentValue = 0;
}

int32 UNumericalTextBox::GetValue() const {
	return CurrentValue;
}

void UNumericalTextBox::SetValue(int32 NewValue) {
	NewValue = GetClamped(NewValue);
	CurrentValue = NewValue;
	SetText(GetTextFromValue(NewValue));
}

void UNumericalTextBox::SynchronizeProperties() {
	Super::SynchronizeProperties();

	if (bUse_MinClamp && bUse_MaxClamp) {
		MaxValue = FMath::Max(MaxValue, MinValue);
	}
}

void UNumericalTextBox::HandleOnTextChanged(const FText& InText) {
	TOptional<int32> NewVal = GetValueFromText(InText);
	if (!NewVal.IsSet()) {
		SetText(Text);
		return;
	}

	int32 Value = NewVal.GetValue();

	const FText NewText = GetTextFromValue(GetSoftClamped(Value));

	// Allow "0" values to pass through. It means its just 0 or '-' or '+' which we dont want to edit.
	if (Value != 0) {
		SetText(NewText);
	}
	CurrentValue = GetClamped(Value);

	Super::HandleOnTextChanged(NewText);
	OnValueChanged.Broadcast(CurrentValue);
}

void UNumericalTextBox::HandleOnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod) {
	TOptional<int32> NewVal = GetValueFromText(InText);

	SetValue(NewVal.Get(0));

	Super::HandleOnTextCommitted(Text, CommitMethod);
	OnValueCommitted.Broadcast(GetValue(), CommitMethod);
}

int32 UNumericalTextBox::GetClamped(int32 InValue) {
	if (bUse_MinClamp) {
		InValue = FMath::Max(InValue, MinValue);
	}
	if (bUse_MaxClamp) {
		InValue = FMath::Min(InValue, MaxValue);
	}
	return InValue;
}

int32 UNumericalTextBox::GetSoftClamped(int32 InValue) {
	if (bUse_MinClamp && MinValue >= 0) {
		InValue = FMath::Max(InValue, 0);
	}
	else if (bUse_MinClamp) {
		InValue = FMath::Max(InValue, MinValue);
	}

	if (bUse_MaxClamp && MaxValue <= 0) {
		InValue = FMath::Min(InValue, 0);
	}
	else if (bUse_MaxClamp) {
		InValue = FMath::Min(InValue, MaxValue);
	}
	
	return InValue;
}

TOptional<int32> UNumericalTextBox::GetValueFromText(const FText& Text) {
	TOptional<int32> Result;
	if (Text.IsEmptyOrWhitespace()) {
		Result = 0;
		return Result;
	}

	FString String = Text.ToString();
	TCHAR FirstLetter = String[0];

	const bool IsFirstLetterNumber = FirstLetter >= '0' && FirstLetter <= '9';
	const bool IsSignCharacter = FirstLetter == '+' || FirstLetter == '-';

	if (!(IsFirstLetterNumber || IsSignCharacter)) {
		return Result;
	}

	for (int32 i = 1; i < String.Len(); ++i) {
		if (String[i] < '0' || String[i] > '9') {
			return Result;
		}
	}

	Result = FCString::Atoi(*String);
	return Result;
}

FText UNumericalTextBox::GetTextFromValue(int32 Value) {
	// Only update the values that need to be changed from the default FNumberFormattingOptions, 
	// as this lets us use the default formatter if possible (which is a performance win!)
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.AlwaysSign = false;;
	NumberFormatOptions.UseGrouping = false;
	NumberFormatOptions.MinimumIntegralDigits = 1;

	return FText::AsNumber(Value, &NumberFormatOptions);
}
