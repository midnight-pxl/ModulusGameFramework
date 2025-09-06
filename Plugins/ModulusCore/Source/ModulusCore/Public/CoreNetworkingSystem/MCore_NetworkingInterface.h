// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCore_NetworkingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UMCore_NetworkingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODULUSCORE_API IMCore_NetworkingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool HasNetworkAuthority() const = 0;

	virtual void OnNetworkInitialized() {}
	virtual void OnNetworkShutdown() {}
	virtual void OnAuthorityChanged(bool bHasAuthority) {}
};
