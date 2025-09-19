// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/CoreDataAssets/InputActionsData/MCore_PDA_InputActionMetadata.h"

UMCore_PDA_InputActionMetadata::UMCore_PDA_InputActionMetadata()
{
}

FPrimaryAssetId UMCore_PDA_InputActionMetadata::GetPrimaryAssetId() const
{
	return Super::GetPrimaryAssetId();
}

FString UMCore_PDA_InputActionMetadata::GetPrimaryAssetName() const
{
	return FString(TEXT(""));
}

bool UMCore_PDA_InputActionMetadata::IsKeyAllowed(FKey Key) const
{
	return true;
}

bool UMCore_PDA_InputActionMetadata::IsPlatformSupported(const FString& PlatformName) const
{
	return false;
}
