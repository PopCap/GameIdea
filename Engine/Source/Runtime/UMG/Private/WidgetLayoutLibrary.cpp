// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UMGPrivatePCH.h"
#include "Slate/SlateBrushAsset.h"
#include "WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UWidgetLayoutLibrary

UWidgetLayoutLibrary::UWidgetLayoutLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

bool UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(APlayerController* PlayerController, FVector WorldLocation, FVector2D& ScreenPosition)
{
	FVector ScreenPosition3D;
	const bool bSuccess = ProjectWorldLocationToWidgetPositionWithDistance(PlayerController, WorldLocation, ScreenPosition3D);
	ScreenPosition = FVector2D(ScreenPosition3D.X, ScreenPosition3D.Y);
	return bSuccess;
}

bool UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPositionWithDistance(APlayerController* PlayerController, FVector WorldLocation, FVector& ScreenPosition)
{	
	if ( PlayerController )
	{
		FVector PixelLocation;
		const bool bProjected = PlayerController->ProjectWorldLocationToScreenWithDistance(WorldLocation, PixelLocation);

		if ( bProjected )
		{
			ScreenPosition = PixelLocation;

			// Round the pixel projected value to reduce jittering due to layout rounding,
			// I do this before I remove scaling, because scaling is going to be applied later
			// in the opposite direction, so as long as we round, before inverse scale, scale should
			// result in more or less the same value, especially after slate does layout rounding.
			ScreenPosition.X = FMath::RoundToInt(ScreenPosition.X);
			ScreenPosition.Y = FMath::RoundToInt(ScreenPosition.Y);
			ScreenPosition.Z = PixelLocation.Z;

			// Get the application / DPI scale
			const float Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);

			// Apply inverse DPI scale so that the widget ends up in the expected position
			ScreenPosition = ScreenPosition / Scale;

			return true;
		}
	}

	ScreenPosition = FVector(0, 0, 0);

	return false;
}

float UWidgetLayoutLibrary::GetViewportScale(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if ( World && World->IsGameWorld() )
	{
		if ( UGameViewportClient* ViewportClient = World->GetGameViewport() )
		{
			FVector2D ViewportSize;
			ViewportClient->GetViewportSize(ViewportSize);
			return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
		}
	}

	return 1;
}

bool UWidgetLayoutLibrary::GetMousePositionScaledByDPI(APlayerController* Player, float& LocationX, float& LocationY)
{
	if ( Player && Player->GetMousePosition(LocationX, LocationY) )
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(Player);
		LocationX = LocationX / Scale;
		LocationY = LocationY / Scale;

		return true;
	}

	return false;
}

FVector2D UWidgetLayoutLibrary::GetViewportSize(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if ( World && World->IsGameWorld() )
	{
		if ( UGameViewportClient* ViewportClient = World->GetGameViewport() )
		{
			FVector2D ViewportSize;
			ViewportClient->GetViewportSize(ViewportSize);
			return ViewportSize;
		}
	}

	return FVector2D(1, 1);
}

UCanvasPanelSlot* UWidgetLayoutLibrary::SlotAsCanvasSlot(UWidget* ChildWidget)
{
	if ( ChildWidget )
	{
		return Cast<UCanvasPanelSlot>(ChildWidget->Slot);
	}

	return nullptr;
}

UGridSlot* UWidgetLayoutLibrary::SlotAsGridSlot(UWidget* ChildWidget)
{
	if ( ChildWidget )
	{
		return Cast<UGridSlot>(ChildWidget->Slot);
	}

	return nullptr;
}

UHorizontalBoxSlot* UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(UWidget* ChildWidget)
{
	if ( ChildWidget )
	{
		return Cast<UHorizontalBoxSlot>(ChildWidget->Slot);
	}

	return nullptr;
}

UOverlaySlot* UWidgetLayoutLibrary::SlotAsOverlaySlot(UWidget* ChildWidget)
{
	if ( ChildWidget )
	{
		return Cast<UOverlaySlot>(ChildWidget->Slot);
	}

	return nullptr;
}

UUniformGridSlot* UWidgetLayoutLibrary::SlotAsUniformGridSlot(UWidget* ChildWidget)
{
	if ( ChildWidget )
	{
		return Cast<UUniformGridSlot>(ChildWidget->Slot);
	}

	return nullptr;
}

UVerticalBoxSlot* UWidgetLayoutLibrary::SlotAsVerticalBoxSlot(UWidget* ChildWidget)
{
	if ( ChildWidget )
	{
		return Cast<UVerticalBoxSlot>(ChildWidget->Slot);
	}

	return nullptr;
}

void UWidgetLayoutLibrary::RemoveAllWidgets(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if ( World && World->IsGameWorld() )
	{
		if ( UGameViewportClient* ViewportClient = World->GetGameViewport() )
		{
			ViewportClient->RemoveAllViewportWidgets();
		}
	}
}

#undef LOCTEXT_NAMESPACE