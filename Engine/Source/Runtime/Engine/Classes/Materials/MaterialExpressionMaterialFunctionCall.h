// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/**
 * MaterialExpressionMaterialFunctionCall - an expression which allows a material to use a material function
 */

#pragma once
#include "Materials/MaterialExpression.h"
#include "MaterialExpressionMaterialFunctionCall.generated.h"

/** Struct that stores information about a function input which is needed to maintain connections and implement the function call. */
USTRUCT()
struct FFunctionExpressionInput
{
	GENERATED_USTRUCT_BODY()

	/** 
	 * Reference to the FunctionInput in the material function.  
	 * This is a reference to a private object so it can't be saved, and must be generated by UpdateFromFunctionResource or SetMaterialFunction. 
	 */
	UPROPERTY(transient)
	class UMaterialExpressionFunctionInput* ExpressionInput;

	/** Id of the FunctionInput, used to link ExpressionInput. */
	UPROPERTY()
	FGuid ExpressionInputId;

	/** Actual input struct which stores information about how this input is connected in the material. */
	UPROPERTY()
	FExpressionInput Input;


	FFunctionExpressionInput()
		: ExpressionInput(NULL)
	{
	}

};

/** Struct that stores information about a function output which is needed to maintain connections and implement the function call. */
USTRUCT()
struct FFunctionExpressionOutput
{
	GENERATED_USTRUCT_BODY()

	/** 
	 * Reference to the FunctionOutput in the material function.  
	 * This is a reference to a private object so it can't be saved, and must be generated by UpdateFromFunctionResource or SetMaterialFunction. 
	 */
	UPROPERTY(transient)
	class UMaterialExpressionFunctionOutput* ExpressionOutput;

	/** Id of the FunctionOutput, used to link ExpressionOutput. */
	UPROPERTY()
	FGuid ExpressionOutputId;

	/** Actual output struct which stores information about how this output is connected in the material. */
	UPROPERTY()
	FExpressionOutput Output;


	FFunctionExpressionOutput()
		: ExpressionOutput(NULL)
	{
	}

};

UCLASS(hidecategories=object, MinimalAPI)
class UMaterialExpressionMaterialFunctionCall : public UMaterialExpression
{
	GENERATED_UCLASS_BODY()

	/** The function to call. */
	UPROPERTY(EditAnywhere, Category=MaterialExpressionMaterialFunctionCall)
	class UMaterialFunction* MaterialFunction;

	/** Array of all the function inputs that this function exposes. */
	UPROPERTY()
	TArray<struct FFunctionExpressionInput> FunctionInputs;

	/** Array of all the function outputs that this function exposes. */
	UPROPERTY()
	TArray<struct FFunctionExpressionOutput> FunctionOutputs;


	// Begin UObject interface.
#if WITH_EDITOR
	virtual void PreEditChange(UProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	virtual void PostLoad() override;
	// End UObject interface.

	// Begin UMaterialExpression Interface
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex, int32 MultiplexIndex) override;
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual const TArray<FExpressionInput*> GetInputs() override;
	virtual FExpressionInput* GetInput(int32 InputIndex) override;
	virtual FString GetInputName(int32 InputIndex) const override;
	virtual bool IsInputConnectionRequired(int32 InputIndex) const override;
#if WITH_EDITOR
	virtual FString GetDescription() const override;
	virtual void GetConnectorToolTip(int32 InputIndex, int32 OutputIndex, TArray<FString>& OutToolTip) override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
#endif
	virtual bool MatchesSearchQuery( const TCHAR* SearchQuery ) override;
	virtual bool IsResultMaterialAttributes(int32 OutputIndex) override;
#if WITH_EDITOR
	virtual uint32 GetInputType(int32 InputIndex) override;
#endif // WITH_EDITOR
	// End UMaterialExpression Interface


	/** 
	 * Set a new material function, given an old function so that links can be passed over if the name matches. 
	 *
	 *	@param ThisFunctionResource				The function the resource is a part of.
	 *	@param OldFunctionResource				The function it was set to.
	 *	@param NewResource						The function to be set to.
	 *
	 *	@return									true if setting the function was a success, false if it failed.
	 */
	ENGINE_API bool SetMaterialFunction(UMaterialFunction* ThisFunctionResource, UMaterialFunction* OldFunctionResource, UMaterialFunction* NewResource);

	/** 
	 * Update FunctionInputs and FunctionOutputs from the MaterialFunction.  
	 * This must be called to keep the inputs and outputs up to date with the function being used. 
	 */
	ENGINE_API void UpdateFromFunctionResource(bool bRecreateAndLinkNode = true);

private:
	
	/** Helper that fixes up expression links where possible. */
	void FixupReferencingExpressions(
		const TArray<FFunctionExpressionOutput>& NewOutputs,
		const TArray<FFunctionExpressionOutput>& OriginalOutputs,
		TArray<UMaterialExpression*>& Expressions, 
		TArray<FExpressionInput*>& MaterialInputs,
		bool bMatchByName);
};



