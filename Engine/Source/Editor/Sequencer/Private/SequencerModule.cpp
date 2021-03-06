// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SequencerPrivatePCH.h"
#include "ModuleManager.h"
#include "Sequencer.h"
#include "Toolkits/ToolkitManager.h"
#include "SequencerCommands.h"
#include "SequencerAssetEditor.h"
#include "SequencerEdMode.h"
#include "SequencerObjectChangeListener.h"


// We disable the deprecation warnings here because otherwise it'll complain about us
// implementing RegisterTrackEditor and UnRegisterTrackEditor.  We know
// that, but we only want it to complain if *others* implement or call these functions.
//
// These macros should be removed when those functions are removed.
PRAGMA_DISABLE_DEPRECATION_WARNINGS


/**
 * SequencerModule implementation (private)
 */
class FSequencerModule
	: public ISequencerModule
{
public:

	// ISequencerModule interface

	virtual TSharedPtr<ISequencer> CreateSequencer( UMovieScene* RootMovieScene, const FSequencerViewParams& InViewParams, TSharedRef<ISequencerObjectBindingManager> ObjectBindingManager ) override
	{
		TSharedRef< FSequencer > Sequencer = MakeShareable(new FSequencer);
		
		FSequencerInitParams SequencerInitParams;
		{
			SequencerInitParams.ViewParams = InViewParams;
			SequencerInitParams.ObjectChangeListener = MakeShareable( new FSequencerObjectChangeListener( Sequencer, false ) );
			SequencerInitParams.ObjectBindingManager = ObjectBindingManager;
			SequencerInitParams.RootMovieScene = RootMovieScene;
			SequencerInitParams.bEditWithinLevelEditor = false;
			SequencerInitParams.ToolkitHost = nullptr;
		}
		
		Sequencer->InitSequencer(SequencerInitParams, TrackEditorDelegates);
		
		return Sequencer;
	}
	
	virtual TSharedPtr<ISequencer> CreateSequencerAssetEditor( const EToolkitMode::Type Mode, const FSequencerViewParams& InViewParams, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UMovieScene* InRootMovieScene, bool bEditWithinLevelEditor ) override
	{
		TSharedRef<FSequencerAssetEditor> SequencerAssetEditor = MakeShareable(new FSequencerAssetEditor);

		SequencerAssetEditor->InitSequencerAssetEditor( Mode, InViewParams, InitToolkitHost, InRootMovieScene, TrackEditorDelegates, bEditWithinLevelEditor );
		return SequencerAssetEditor->GetSequencerInterface();
	}

	virtual FDelegateHandle RegisterTrackEditor_Handle( FOnCreateTrackEditor InOnCreateTrackEditor ) override
	{
		TrackEditorDelegates.Add( InOnCreateTrackEditor );
		return TrackEditorDelegates.Last().GetHandle();
	}

	virtual void UnRegisterTrackEditor_Handle( FDelegateHandle InHandle ) override
	{
		TrackEditorDelegates.RemoveAll( [=](const FOnCreateTrackEditor& Delegate){ return Delegate.GetHandle() == InHandle; } );
	}

	virtual void StartupModule() override
	{
		if (GIsEditor)
		{
			FSequencerCommands::Register();

			FEditorModeRegistry::Get().RegisterMode<FSequencerEdMode>(
				FSequencerEdMode::EM_SequencerMode,
				NSLOCTEXT("Sequencer", "SequencerEditMode", "Sequencer Mode"),
				FSlateIcon(),
				false);
		}

		MenuExtensibilityManager = MakeShareable( new FExtensibilityManager );
		ToolBarExtensibilityManager = MakeShareable( new FExtensibilityManager );
	}

	virtual void ShutdownModule() override
	{
		if (GIsEditor)
		{
			FSequencerCommands::Unregister();

			FEditorModeRegistry::Get().UnregisterMode(FSequencerEdMode::EM_SequencerMode);
		}
	}

	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() const override { return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() const override { return ToolBarExtensibilityManager; }

private:

	/** List of auto-key handler delegates sequencers will execute when they are created */
	TArray< FOnCreateTrackEditor > TrackEditorDelegates;

	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};


PRAGMA_ENABLE_DEPRECATION_WARNINGS

IMPLEMENT_MODULE(FSequencerModule, Sequencer);
