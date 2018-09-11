// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "ModuleInterface.h"

/**
 * Online subsystem module class (Google Play Implementation)
 * Code related to the loading and handling of the Amdroid Google Play module.
 */
class FOnlineSubsystemGooglePlayModule : public IModuleInterface
{
private:
	
	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryGooglePlay* GooglePlayFactory;

	virtual ~FOnlineSubsystemGooglePlayModule(){}

	// Begin IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
	// End IModuleInterface interface
};

typedef TSharedPtr<FOnlineSubsystemGooglePlayModule> FOnlineSubsystemGooglePlayModulePtr;

