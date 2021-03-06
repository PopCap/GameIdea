// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CurveFactory.generated.h"

/**
 * Factory that creates curve assets, prompting to pick the kind of curve at creation time
 */

UCLASS()
class UCurveFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	/** The type of blueprint that will be created */
	UPROPERTY(EditAnywhere, Category=CurveFactory)
	TSubclassOf<UCurveBase> CurveClass;

	// UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	// End of UFactory interface
};

/**
 * Factory that creates float curve assets
 */

UCLASS()
class UCurveFloatFactory : public UCurveFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory interface
	virtual bool ConfigureProperties() override;
	// End of UFactory interface
};
