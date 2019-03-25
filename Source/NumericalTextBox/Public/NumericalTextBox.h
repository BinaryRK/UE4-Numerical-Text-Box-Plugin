// Harry Katagis - 2019

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content, meta = (editcondition = "bUse_MinClamp"))
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bUse_MaxClamp : 1;

	/** Only allow values <= than this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content, meta = (editcondition = "bUse_MaxClamp"))
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

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetMinValue(int32 NewMin);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetMaxValue(int32 NewMax);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetClampMin(bool ShouldClampMin);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetClampMax(bool ShouldClampMax);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool IsClampingMin() const;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool IsClampingMax() const;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	int32 GetMinValue() const;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	int32 GetMaxValue() const;


	// Returns the width of highest length allowed number. Can be used to set a fixed width at setup.
	UFUNCTION(BlueprintCallable, Category = "Widget")
	int32 GetCharactersWidth() const;


	/** Whether to use editing improvements for numbers when using clamping. 
	* eg: typing '4' while MinValue is 10 will go through editing phase to allow typing 40. 
	* Using GetValue will always return a properly clamped value. 
	* Commiting the changes in any way will replace the value with the clamped one.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Behavior, AdvancedDisplay)
	bool UseSmartTypingClamp;
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	virtual void HandleOnTextChanged(const FText& Text) override;
	virtual void HandleOnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod) override;

protected:
	int32 CurrentValue;

	int32 GetClamped(int32 InValue) const;

	/** Specific clamp used for nice typing behavior. eg: attempting to type 40 in a value that can go from 10-50 */
	int32 GetSoftClamped(int32 InValue) const;


	static TOptional<int32> GetValueFromText(const FText& Text);
	static FText GetTextFromValue(int32 Value);
};
