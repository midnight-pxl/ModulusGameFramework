// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/CoreStructEnums/UIStructsEnums/UIExtensionStructEnums/MCore_UIExtensionTypes.h"

bool FMCore_UIExtensionPoint::DoesExtensionPassContract(const FMCore_UIExtension* Extension) const
{
	if (!Extension) { return false; }

	/** Context matching rules:
	 * 1. Both nullptr (global) -> MATCH
	 * 2. Both valid and same object -> MATCH
	 * 3. Otherwise -> NO MATCH */

	const bool bExtensionPointHasContext = ContextObject.IsValid();
	const bool bExtensionHasContext = Extension->ContextObject.IsValid();

	/** Case 1: Both are global (no context) */
	if (!bExtensionPointHasContext && !bExtensionHasContext) { return true; }

	/** Case 2: Both have context - check if same object */
	if (bExtensionPointHasContext && bExtensionHasContext)
	{
		return ContextObject.Get() == Extension->ContextObject.Get();
	}
	/** Case 3: One has context, other doesn't - no match */
	return false;
}