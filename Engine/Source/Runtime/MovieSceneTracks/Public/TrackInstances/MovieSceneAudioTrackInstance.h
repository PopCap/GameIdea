// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMovieSceneTrackInstance.h"


class UAudioComponent;
class UMovieSceneAudioTrack;


/**
 * Instance of a UMovieSceneAudioTrack
 */
class FMovieSceneAudioTrackInstance
	: public IMovieSceneTrackInstance
{
public:

	FMovieSceneAudioTrackInstance( UMovieSceneAudioTrack& InAudioTrack );

	/** IMovieSceneTrackInstance interface */
	virtual void SaveState(const TArray<UObject*>& RuntimeObjects) override {}
	virtual void RestoreState(const TArray<UObject*>& RuntimeObjects) override {}
	virtual void Update( float Position, float LastPosition, const TArray<UObject*>& RuntimeObjects, IMovieScenePlayer& Player ) override;
	virtual void RefreshInstance( const TArray<UObject*>& RuntimeObjects, IMovieScenePlayer& Player ) override {}
	virtual void ClearInstance( IMovieScenePlayer& Player ) override {}

private:

	/** Plays the sound of the given section at the given time */
	void PlaySound(class UMovieSceneAudioSection* AudioSection, UAudioComponent* Component, float Time);
	
	/** Stops all components for the given row index */
	void StopSound(int32 RowIndex);

	/** Stops all components from playing */
	void StopAllSounds();

	/** Gets the audio component component for the actor and row index, creating it if necessary */
	UAudioComponent* GetAudioComponent(AActor* Actor, int32 RowIndex);

	/** Utility function for getting actors from objects array */
	TArray<AActor*> GetRuntimeActors(const TArray<UObject*>& RuntimeObjects) const;

private:

	/** Track that is being instanced */
	UMovieSceneAudioTrack* AudioTrack;

	/** Audio components to play our audio tracks with, one per row per actor */
	TArray< TMap<AActor*, UAudioComponent*> > PlaybackAudioComponents;
};
