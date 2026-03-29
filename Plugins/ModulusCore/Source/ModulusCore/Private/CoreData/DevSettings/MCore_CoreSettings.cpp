// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Types/UI/MCore_ThemeTypes.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Logging/LogModulusSettings.h"

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

// ============================================================================
// SETTINGS AGGREGATION
// ============================================================================

const TArray<UMCore_DA_SettingsCollection*>& UMCore_CoreSettings::GetAllSettingsCollections() const
{
	if (!bCollectionsCacheValid)
	{
		ResolvedCollections.Reset();
		for (const TSoftObjectPtr<UMCore_DA_SettingsCollection>& SoftRef : SettingsCollections)
		{
			if (SoftRef.IsNull())
			{
				continue;
			}
			UMCore_DA_SettingsCollection* Loaded = SoftRef.LoadSynchronous();
			if (Loaded)
			{
				ResolvedCollections.Add(Loaded);
			}
			else
			{
				UE_LOG(LogModulusSettings, Warning,
					TEXT("CoreSettings::GetAllSettingsCollections -- failed to load collection '%s'"),
					*SoftRef.ToString());
			}
		}
		bCollectionsCacheValid = true;
		UE_LOG(LogModulusSettings, Log, TEXT("CoreSettings::GetAllSettingsCollections -- loaded %d collection(s)"), ResolvedCollections.Num());
	}
	return ResolvedCollections;
}

UMCore_DA_SettingDefinition* UMCore_CoreSettings::FindSettingDefinitionByTag(
	const FGameplayTag& SettingTag) const
{
	for (const UMCore_DA_SettingsCollection* Collection : GetAllSettingsCollections())
	{
		if (UMCore_DA_SettingDefinition* Found = Collection->FindSettingByTag(SettingTag))
		{
			return Found;
		}
	}
	return nullptr;
}

TArray<UMCore_DA_SettingDefinition*> UMCore_CoreSettings::GetSettingsForCategory(
	const FGameplayTag& CategoryTag) const
{
	TArray<UMCore_DA_SettingDefinition*> Combined;
	for (const UMCore_DA_SettingsCollection* Collection : GetAllSettingsCollections())
	{
		Combined.Append(Collection->GetSettingsInCategory(CategoryTag));
	}
	Combined.Sort([](const UMCore_DA_SettingDefinition& A, const UMCore_DA_SettingDefinition& B)
	{
		return A.SortOrder < B.SortOrder;
	});
	return Combined;
}

TArray<FGameplayTag> UMCore_CoreSettings::GetAllSettingsCategories() const
{
	TMap<FGameplayTag, int32> CategoryMinSort;
	for (const UMCore_DA_SettingsCollection* Collection : GetAllSettingsCollections())
	{
		for (const TObjectPtr<UMCore_DA_SettingDefinition>& Setting : Collection->GetAllSettings())
		{
			if (!Setting || !Setting->CategoryTag.IsValid())
			{
				continue;
			}
			if (int32* Existing = CategoryMinSort.Find(Setting->CategoryTag))
			{
				*Existing = FMath::Min(*Existing, Setting->SortOrder);
			}
			else
			{
				CategoryMinSort.Add(Setting->CategoryTag, Setting->SortOrder);
			}
		}
	}

	TArray<FGameplayTag> Result;
	CategoryMinSort.GetKeys(Result);
	Result.Sort([&CategoryMinSort](const FGameplayTag& A, const FGameplayTag& B)
	{
		return CategoryMinSort[A] < CategoryMinSort[B];
	});
	return Result;
}

FText UMCore_CoreSettings::GetCategoryDisplayName(const FGameplayTag& CategoryTag) const
{
	for (const UMCore_DA_SettingsCollection* Collection : GetAllSettingsCollections())
	{
		if (const FText* Name = Collection->CategoryDisplayName.Find(CategoryTag))
		{
			return *Name;
		}
	}
	// Fallback: last segment of the tag path
	const FString TagStr = CategoryTag.ToString();
	int32 LastDot;
	if (TagStr.FindLastChar(TEXT('.'), LastDot))
	{
		return FText::FromString(TagStr.Mid(LastDot + 1));
	}
	return FText::GetEmpty();
}

bool UMCore_CoreSettings::HasValidSettingsCollections() const
{
	return GetAllSettingsCollections().Num() > 0;
}

void UMCore_CoreSettings::InvalidateCollectionCache()
{
	ResolvedCollections.Reset();
	bCollectionsCacheValid = false;
	UE_LOG(LogModulusSettings, Log, TEXT("CoreSettings::InvalidateCollectionCache -- collection cache invalidated"));
}

#if WITH_EDITOR
void UMCore_CoreSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UMCore_CoreSettings, SettingsCollections))
	{
		InvalidateCollectionCache();
	}
}
#endif
