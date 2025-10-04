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
 * Networking interface for ModulusCore components
 *
 * Provides common networking contract for components that need authority validation
 * and network lifecycle hooks. Implemented by UMCore_NetworkingComponent.
 *
 * Derived Classes:
 * - UMCore_NetworkingComponent (base implementation)
 * - UMCore_EventListenerComp (uses networking base)
 */
class MODULUSCORE_API IMCore_NetworkingInterface
{
	GENERATED_BODY()

public:

	/** Check if this component/actor has network authority (pure virtual - must implement) */
	virtual bool HasNetworkAuthority() const = 0;

	/** Called when network system initializes (override for custom initialization) */
	virtual void OnNetworkInitialized() {}

	/** Called when network system shuts down (override for custom cleanup) */
	virtual void OnNetworkShutdown() {}

	/**
	 * Called when authority state changes
	 *
	 * @param bHasAuthority - True if component gained authority, false if lost
	 */
	virtual void OnAuthorityChanged(bool bHasAuthority) {}
};
