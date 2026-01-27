// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Types/UI/MCore_ThemeTypes.h"

UMCore_CoreSettings::UMCore_CoreSettings()
{
}

const UMCore_CoreSettings* UMCore_CoreSettings::Get()
{
	return GetDefault<UMCore_CoreSettings>();
}

#if WITH_EDITOR
FText UMCore_CoreSettings::GetSectionText() const
{
	return NSLOCTEXT("MCore_CoreSettings", "SectionText", "Modulus Core");
}

FText UMCore_CoreSettings::GetSectionDescription() const
{
	return NSLOCTEXT("MCore_CoreSettings", "SectionDesc",
		"Configure theme settings and debug options for the Modulus Game Framework.");
}
#endif

bool UMCore_CoreSettings::IsEventLoggingEnabled() const
{
#if WITH_EDITORONLY_DATA
	return bEnableEventSystemLogging;
#else
	return false;
#endif
}

bool UMCore_CoreSettings::IsUIDebugOverlayEnabled() const
{
#if WITH_EDITORONLY_DATA
	return bShowUILayerDebugOverlay;
#else
	return false;
#endif
}

UMCore_PDA_UITheme_Base* UMCore_CoreSettings::GetDefaultTheme() const
{
	if (!IsValidThemeIndex(DefaultThemeIndex))
	{
		return nullptr;
	}

	const FMCore_ThemeEntry& ThemeEntry = AvailableThemes[DefaultThemeIndex];
	if (ThemeEntry.ThemeAsset.IsNull())
	{
		return nullptr;
	}

	return ThemeEntry.ThemeAsset.LoadSynchronous();
}

UMCore_PDA_UITheme_Base* UMCore_CoreSettings::GetDesignTimeTheme()
{
	const UMCore_CoreSettings* Settings = Get();
	return Settings ? Settings->GetDefaultTheme() : nullptr;
}

bool UMCore_CoreSettings::IsValidThemeIndex(int32 Index) const
{
	return AvailableThemes.IsValidIndex(Index);
}
