// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "EnginePrivate.h"
#include "SoundDefinitions.h"
#include "Sound/SoundNodeRandom.h"

#if WITH_EDITOR
	#include "UnrealEd.h"
#endif

/*-----------------------------------------------------------------------------
    USoundNodeRandom implementation.
-----------------------------------------------------------------------------*/
USoundNodeRandom::USoundNodeRandom(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bRandomizeWithoutReplacement = true;
	NumRandomUsed = 0;
}

void USoundNodeRandom::FixWeightsArray()
{
	// If weights and children got out of sync, we fix it first.
	if( Weights.Num() < ChildNodes.Num() )
	{
		Weights.AddZeroed( ChildNodes.Num() - Weights.Num() );
	}
	else if( Weights.Num() > ChildNodes.Num() )
	{
		const int32 NumToRemove = Weights.Num() - ChildNodes.Num();
		Weights.RemoveAt( Weights.Num() - NumToRemove, NumToRemove );
	}
}

void USoundNodeRandom::FixHasBeenUsedArray()
{
	// If HasBeenUsed and children got out of sync, we fix it first.
	if( HasBeenUsed.Num() < ChildNodes.Num() )
	{
		HasBeenUsed.AddZeroed( ChildNodes.Num() - HasBeenUsed.Num() );
	}
	else if( HasBeenUsed.Num() > ChildNodes.Num() )
	{
		const int32 NumToRemove = HasBeenUsed.Num() - ChildNodes.Num();
		HasBeenUsed.RemoveAt( HasBeenUsed.Num() - NumToRemove, NumToRemove );
	}
}

void USoundNodeRandom::PostLoad()
{
	Super::PostLoad();
	if (!GIsEditor && PreselectAtLevelLoad > 0)
	{
		while (ChildNodes.Num() > PreselectAtLevelLoad)
		{
			RemoveChildNode(FMath::Rand() % ChildNodes.Num());
		}
	}
#if WITH_EDITOR
	else if (GEditor != nullptr && (GEditor->bIsSimulatingInEditor || GEditor->PlayWorld != NULL))
	{
		UpdatePIEHiddenNodes();
	}
#endif //WITH_EDITOR
}

void USoundNodeRandom::ParseNodes( FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances )
{
	RETRIEVE_SOUNDNODE_PAYLOAD( sizeof( int32 ) );
	DECLARE_SOUNDNODE_ELEMENT( int32, NodeIndex );

	FixHasBeenUsedArray();  // for now prob need this until resave packages has occurred

#if WITH_EDITOR
	bool bIsPIESound = (GEditor != nullptr) && ((GEditor->bIsSimulatingInEditor || GEditor->PlayWorld != NULL) && ActiveSound.World != NULL);
#endif //WITH_EDITOR

	// Pick a random child node and save the index.
	if( *RequiresInitialization )
	{
		NodeIndex = 0;
		float WeightSum = 0.0f;

#if WITH_EDITOR
		if (bIsPIESound)
		{
			// Find the first available index - needed if there is only one
			while (PIEHiddenNodes.Contains(NodeIndex))
			{
				NodeIndex++;
			}
		}
#endif //WITH_EDITOR

		// only calculate the weights that have not been used and use that set for the random choice
		for( int32 i = 0; i < Weights.Num(); ++i )
		{
#if WITH_EDITOR
			if (!bIsPIESound || !PIEHiddenNodes.Contains(i))
#endif //WITH_EDITOR
			{
				if( ( bRandomizeWithoutReplacement == false ) ||  ( HasBeenUsed[ i ] != true ) )
				{
					WeightSum += Weights[ i ];
				}
			}
		}

		float Choice = FMath::FRand() * WeightSum;
		WeightSum = 0.0f;
		for( int32 i = 0; i < ChildNodes.Num() && i < Weights.Num(); ++i )
		{
#if WITH_EDITOR
			if(!bIsPIESound || !PIEHiddenNodes.Contains(i) )
#endif //WITH_EDITOR
			{
				if (bRandomizeWithoutReplacement && HasBeenUsed[i])
				{
					continue;
				}
				WeightSum += Weights[i];
				if (Choice < WeightSum)
				{
					NodeIndex = i;
					HasBeenUsed[i] = true;
					++NumRandomUsed;
					break;
				}
			}
		}

		*RequiresInitialization = 0;
	}

	// check to see if we have used up our random sounds
	if( bRandomizeWithoutReplacement && ( HasBeenUsed.Num() > 0 ) && ( NumRandomUsed >= HasBeenUsed.Num()
#if WITH_EDITOR
		|| (bIsPIESound && NumRandomUsed >= (HasBeenUsed.Num() - PIEHiddenNodes.Num()))
#endif //WITH_EDITOR
		)	)
	{
		// reset all of the children nodes
		for( int32 i = 0; i < HasBeenUsed.Num(); ++i )
		{
			HasBeenUsed[ i ] = false;
		}

		// set the node that has JUST played to be true so we don't repeat it
		if (HasBeenUsed.Num() > NodeIndex)
		{
			HasBeenUsed[ NodeIndex ] = true;
		}
		NumRandomUsed = 1;
	}

	// "play" the sound node that was selected
	if( NodeIndex < ChildNodes.Num() && ChildNodes[ NodeIndex ] )
	{
		ChildNodes[ NodeIndex ]->ParseNodes( AudioDevice, GetNodeWaveInstanceHash(NodeWaveInstanceHash, ChildNodes[NodeIndex], NodeIndex), ActiveSound, ParseParams, WaveInstances );
	}
}

void USoundNodeRandom::CreateStartingConnectors()
{
	// Random Sound Nodes default with two connectors.
	InsertChildNode( ChildNodes.Num() );
	InsertChildNode( ChildNodes.Num() );
}

void USoundNodeRandom::InsertChildNode( int32 Index )
{
	FixWeightsArray();
	FixHasBeenUsedArray();

	check( Index >= 0 && Index <= Weights.Num() );
	check( ChildNodes.Num() == Weights.Num() );

	Weights.InsertUninitialized( Index );
	Weights[Index] = 1.0f;

	HasBeenUsed.InsertUninitialized( Index );
	HasBeenUsed[ Index ] = false;

	Super::InsertChildNode( Index );
}

void USoundNodeRandom::RemoveChildNode( int32 Index )
{
	FixWeightsArray();
	FixHasBeenUsedArray();

	check( Index >= 0 && Index < Weights.Num() );
	check( ChildNodes.Num() == Weights.Num() );

	Weights.RemoveAt( Index );
	HasBeenUsed.RemoveAt( Index );

	Super::RemoveChildNode( Index );
}

#if WITH_EDITOR
void USoundNodeRandom::SetChildNodes(TArray<USoundNode*>& InChildNodes)
{
	Super::SetChildNodes(InChildNodes);

	if (Weights.Num() < ChildNodes.Num())
	{
		while (Weights.Num() < ChildNodes.Num())
		{
			int32 NewIndex = Weights.Num();
			Weights.InsertUninitialized(NewIndex);
			Weights[ NewIndex ] = 1.0f;
		}
	}
	else if (Weights.Num() > ChildNodes.Num())
	{
		const int32 NumToRemove = Weights.Num() - ChildNodes.Num();
		Weights.RemoveAt(Weights.Num() - NumToRemove, NumToRemove);
	}

	if (HasBeenUsed.Num() < ChildNodes.Num())
	{
		while (HasBeenUsed.Num() < ChildNodes.Num())
		{
			int32 NewIndex = HasBeenUsed.Num();
			HasBeenUsed.InsertUninitialized(NewIndex);
			HasBeenUsed[ NewIndex ] = false;
		}
	}
	else if (HasBeenUsed.Num() > ChildNodes.Num())
	{
		const int32 NumToRemove = HasBeenUsed.Num() - ChildNodes.Num();
		HasBeenUsed.RemoveAt(HasBeenUsed.Num() - NumToRemove, NumToRemove);
	}

}

void USoundNodeRandom::OnBeginPIE(const bool bIsSimulating)
{
	UpdatePIEHiddenNodes();
}
#endif //WITH_EDITOR

#if WITH_EDITOR
void USoundNodeRandom::UpdatePIEHiddenNodes()
{
	// should we hide some nodes?
	int32 NodesToHide = ChildNodes.Num() - PreselectAtLevelLoad;
	if (PreselectAtLevelLoad > 0 && NodesToHide > 0)
	{
		// Choose the right amount of nodes to hide
		PIEHiddenNodes.Empty();
		while (PIEHiddenNodes.Num() < NodesToHide)
		{
			PIEHiddenNodes.AddUnique(FMath::Rand() % ChildNodes.Num());
		}
		// reset all of the child nodes and the use count
		for( int32 i = 0; i < HasBeenUsed.Num(); ++i )
		{
			HasBeenUsed[ i ] = false;
		}
		NumRandomUsed = 0;
	}
	// don't hide zero/negative amounts of nodes
	else if ((PreselectAtLevelLoad <= 0 || NodesToHide <= 0))
	{
		PIEHiddenNodes.Empty();
	}
}
#endif //WITH_EDITOR
