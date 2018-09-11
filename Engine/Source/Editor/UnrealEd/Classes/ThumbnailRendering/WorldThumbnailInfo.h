// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/**
 * Thumbnail information for world assets
 */

#pragma once
#include "Camera/CameraTypes.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"
#include "WorldThumbnailInfo.generated.h"

UENUM()
namespace EOrthoThumbnailDirection
{
	enum Type
	{
		Top,
		Bottom,
		Left,
		Right,
		Front,
		Back
	};
}

UCLASS(MinimalAPI)
class UWorldThumbnailInfo : public USceneThumbnailInfo
{
	GENERATED_UCLASS_BODY()

	/** The type of projection to use */
	UPROPERTY(EditAnywhere, Category=Thumbnail)
	TEnumAsByte<ECameraProjectionMode::Type> CameraMode;

	/** The direction of the camera when in Orthographic CameraMode */
	UPROPERTY(EditAnywhere, Category=Thumbnail)
	TEnumAsByte<EOrthoThumbnailDirection::Type> OrthoDirection;
};
