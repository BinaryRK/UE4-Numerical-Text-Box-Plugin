// Harry Katagis - 2017

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "NumericalTextBox.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Numerical Text Box"))
class NUMERICALTEXTBOXPLUGIN_API UNumericalTextBox : public UEditableTextBox
{
	GENERATED_BODY()

public:
	UNumericalTextBox(const FObjectInitializer& ObjectInitializer);

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumericalTextBoxChangedEvent, int32, Value);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNumericalTextBoxCommitedEvent, int32, Value, ETextCommit::Type, CommitMethod);


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bUse_MinClamp : 1;

	/** Only allow values >= than this. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Content, meta = (editcondition = "bUse_MinClamp"))
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bUse_MaxClamp : 1;

	/** Only allow values <= than this. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Content, meta = (editcondition = "bUse_MaxClamp"))
	int32 MaxValue;

public:
	/** Called whenever the value is changed interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "TextBox|Event")
	FOnNumericalTextBoxChangedEvent OnValueChanged;

	/** Called whenever the value is committed.  This happens when the user presses enter or the text box loses focus. */
	UPROPERTY(BlueprintAssignable, Category = "TextBox|Event")
	FOnNumericalTextBoxCommitedEvent OnValueCommitted;


public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	int32 GetValue() const;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetValue(int32 NewValue);

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	virtual void HandleOnTextChanged(const FText& Text) override;
	virtual void HandleOnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod) override;

protected:
	int32 CurrentValue;

	int32 GetClamped(int32 InValue);

	/** Specific clamp used for nice typing behavior. eg: attempting to type 40 in a value that can go from 10-50 */
	int32 GetSoftClamped(int32 InValue);


	static TOptional<int32> GetValueFromText(const FText& Text);
	static FText GetTextFromValue(int32 Value);
};
