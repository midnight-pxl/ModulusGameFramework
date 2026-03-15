// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_NetworkingInterface.h
 *
 * Networking interface providing authority validation and network lifecycle hooks.
 * Implemented by UMCore_NetworkingComponent for actor components.
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCore_NetworkingInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UMCore_NetworkingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Networking interface for ModulusCore components.
 *
 * Provides a common networking contract for components that need authority validation
 * and network lifecycle hooks.
 *
 * Key Implementors:
 * - UMCore_NetworkingComponent (base implementation)
 * - UMCore_EventListenerComp (uses networking base)
 */
class MODULUSCORE_API IMCore_NetworkingInterface
{
	GENERATED_BODY()

public:

	/* Pure virtual - must be implemented by derived classes */
	virtual bool HasNetworkAuthority() const = 0;

	virtual void OnNetworkInitialized() {}

	virtual void OnNetworkShutdown() {}

	/**
	 * Called when authority state changes.
	 *
	 * @param bHasAuthority True if component gained authority, false if lost
	 */
	virtual void OnAuthorityChanged(bool bHasAuthority) {}
};
