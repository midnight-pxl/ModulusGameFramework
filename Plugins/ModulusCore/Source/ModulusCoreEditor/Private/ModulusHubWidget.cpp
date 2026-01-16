// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "ModulusHubWidget.h"
#include "CoreEditorLogging/LogModulusEditor.h"
#include "CoreData/CoreDevSettings/MCore_ProjectSettings.h"
#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "Misc/FileHelper.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformProcess.h"

UModulusHubWidget::UModulusHubWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UModulusHubWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshInstalledPlugins();
	UE_LOG(LogModulusEditor, Log, TEXT("Modulus Hub opened"));
}

void UModulusHubWidget::ResetAllSettings()
{
	// Reset Project Settings
	if (UMCore_ProjectSettings* Settings = GetMutableDefault<UMCore_ProjectSettings>())
	{
		Settings->bAutoInitializePlugins = true;
		Settings->EnabledFeaturePlugins.Empty();
		Settings->bEnableWidgetPooling = true;
		Settings->MaxPooledObjectsPerType = 10;
		Settings->DefaultValidationStrictness = EMCore_ValidationStrictness::Balanced;
		Settings->bEnableUIScaling = true;
		Settings->DefaultUIAnimationSpeed = 0.3f;
		Settings->MaxUIScaleRange = 2.0f;
		Settings->SaveConfig();
	}

	// Reset UI Settings
	if (UMCore_CommonUISettings* UISettings = GetMutableDefault<UMCore_CommonUISettings>())
	{
		UISettings->CurrentThemeAsset.Reset();
		UISettings->AvailableThemes.Empty();
		UISettings->DefaultThemeIndex = 0;
		UISettings->bEnableTagCaching = true;
		UISettings->WidgetClassOverrides.Empty();
		UISettings->CurrentDefaultSettings.Reset();
		UISettings->SaveConfig();
	}

	UE_LOG(LogModulusEditor, Log, TEXT("[Reset Settings] All Modulus settings reset to defaults"));
}

void UModulusHubWidget::ExportSettings()
{
	IDesktopPlatform* Desktop = FDesktopPlatformModule::Get();
	if (!Desktop)
	{
		UE_LOG(LogModulusEditor, Warning, TEXT("[Export] Desktop platform unavailable"));
		return;
	}

	TArray<FString> Files;
	const bool bOpened = Desktop->SaveFileDialog(
		FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
		TEXT("Export Modulus Settings"),
		FPaths::ProjectSavedDir(),
		TEXT("ModulusSettings.json"),
		TEXT("JSON (*.json)|*.json"),
		0,
		Files);

	if (!bOpened || Files.Num() == 0)
	{
		return; // User cancelled
	}

	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	// Serialize Project Settings
	if (const UMCore_ProjectSettings* ProjectSettings = UMCore_ProjectSettings::Get())
	{
		TSharedPtr<FJsonObject> PSJson = MakeShared<FJsonObject>();
		PSJson->SetBoolField(TEXT("bAutoInitializePlugins"), ProjectSettings->bAutoInitializePlugins);
		PSJson->SetBoolField(TEXT("bEnableWidgetPooling"), ProjectSettings->bEnableWidgetPooling);
		PSJson->SetNumberField(TEXT("MaxPooledObjectsPerType"), ProjectSettings->MaxPooledObjectsPerType);
		PSJson->SetNumberField(TEXT("DefaultValidationStrictness"), static_cast<int32>(ProjectSettings->DefaultValidationStrictness));
		PSJson->SetBoolField(TEXT("bEnableUIScaling"), ProjectSettings->bEnableUIScaling);
		PSJson->SetNumberField(TEXT("DefaultUIAnimationSpeed"), ProjectSettings->DefaultUIAnimationSpeed);
		PSJson->SetNumberField(TEXT("MaxUIScaleRange"), ProjectSettings->MaxUIScaleRange);

		// Serialize enabled plugins array
		TArray<TSharedPtr<FJsonValue>> PluginsArray;
		for (const FString& Plugin : ProjectSettings->EnabledFeaturePlugins)
		{
			PluginsArray.Add(MakeShared<FJsonValueString>(Plugin));
		}
		PSJson->SetArrayField(TEXT("EnabledFeaturePlugins"), PluginsArray);

		Root->SetObjectField(TEXT("ProjectSettings"), PSJson);
	}

	// Serialize UI Settings
	if (const UMCore_CommonUISettings* UISettings = UMCore_CommonUISettings::Get())
	{
		TSharedPtr<FJsonObject> UIJson = MakeShared<FJsonObject>();
		UIJson->SetNumberField(TEXT("DefaultThemeIndex"), UISettings->DefaultThemeIndex);
		UIJson->SetBoolField(TEXT("bEnableTagCaching"), UISettings->bEnableTagCaching);
		UIJson->SetStringField(TEXT("DefaultSaveSlotName"), UISettings->DefaultSaveSlotName);
		Root->SetObjectField(TEXT("UISettings"), UIJson);
	}

	// Add metadata
	Root->SetStringField(TEXT("ExportVersion"), TEXT("1.0"));
	Root->SetStringField(TEXT("ExportDate"), FDateTime::Now().ToString());

	// Serialize and save
	FString Output;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	if (FFileHelper::SaveStringToFile(Output, *Files[0]))
	{
		UE_LOG(LogModulusEditor, Log, TEXT("[Export] Saved to %s"), *Files[0]);
	}
	else
	{
		UE_LOG(LogModulusEditor, Error, TEXT("[Export] Failed to write file"));
	}
}

void UModulusHubWidget::ImportSettings()
{
	IDesktopPlatform* Desktop = FDesktopPlatformModule::Get();
	if (!Desktop)
	{
		UE_LOG(LogModulusEditor, Warning, TEXT("[Import] Desktop platform unavailable"));
		return;
	}

	TArray<FString> Files;
	const bool bOpened = Desktop->OpenFileDialog(
		FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
		TEXT("Import Modulus Settings"),
		FPaths::ProjectSavedDir(),
		TEXT(""),
		TEXT("JSON (*.json)|*.json"),
		0,
		Files);

	if (!bOpened || Files.Num() == 0)
	{
		return; // User cancelled
	}

	FString Content;
	if (!FFileHelper::LoadFileToString(Content, *Files[0]))
	{
		UE_LOG(LogModulusEditor, Error, TEXT("[Import] Failed to read file"));
		return;
	}

	TSharedPtr<FJsonObject> Root;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
	{
		UE_LOG(LogModulusEditor, Error, TEXT("[Import] Invalid JSON format"));
		return;
	}

	// Apply Project Settings
	const TSharedPtr<FJsonObject>* ProjectJson = nullptr;
	if (Root->TryGetObjectField(TEXT("ProjectSettings"), ProjectJson) && ProjectJson)
	{
		if (UMCore_ProjectSettings* PS = GetMutableDefault<UMCore_ProjectSettings>())
		{
			PS->bAutoInitializePlugins = (*ProjectJson)->GetBoolField(TEXT("bAutoInitializePlugins"));
			PS->bEnableWidgetPooling = (*ProjectJson)->GetBoolField(TEXT("bEnableWidgetPooling"));
			PS->MaxPooledObjectsPerType = (*ProjectJson)->GetIntegerField(TEXT("MaxPooledObjectsPerType"));
			PS->DefaultValidationStrictness = static_cast<EMCore_ValidationStrictness>((*ProjectJson)->GetIntegerField(TEXT("DefaultValidationStrictness")));
			PS->bEnableUIScaling = (*ProjectJson)->GetBoolField(TEXT("bEnableUIScaling"));
			PS->DefaultUIAnimationSpeed = (*ProjectJson)->GetNumberField(TEXT("DefaultUIAnimationSpeed"));
			PS->MaxUIScaleRange = (*ProjectJson)->GetNumberField(TEXT("MaxUIScaleRange"));

			// Import enabled plugins array
			const TArray<TSharedPtr<FJsonValue>>* PluginsArray = nullptr;
			if ((*ProjectJson)->TryGetArrayField(TEXT("EnabledFeaturePlugins"), PluginsArray) && PluginsArray)
			{
				PS->EnabledFeaturePlugins.Empty();
				for (const TSharedPtr<FJsonValue>& Value : *PluginsArray)
				{
					PS->EnabledFeaturePlugins.Add(Value->AsString());
				}
			}

			PS->SaveConfig();
		}
	}

	// Apply UI Settings
	const TSharedPtr<FJsonObject>* UIJson = nullptr;
	if (Root->TryGetObjectField(TEXT("UISettings"), UIJson) && UIJson)
	{
		if (UMCore_CommonUISettings* UISettings = GetMutableDefault<UMCore_CommonUISettings>())
		{
			UISettings->DefaultThemeIndex = (*UIJson)->GetIntegerField(TEXT("DefaultThemeIndex"));
			UISettings->bEnableTagCaching = (*UIJson)->GetBoolField(TEXT("bEnableTagCaching"));

			FString SaveSlotName;
			if ((*UIJson)->TryGetStringField(TEXT("DefaultSaveSlotName"), SaveSlotName))
			{
				UISettings->DefaultSaveSlotName = SaveSlotName;
			}

			UISettings->SaveConfig();
		}
	}

	UE_LOG(LogModulusEditor, Log, TEXT("[Import] Loaded from %s"), *Files[0]);
}

void UModulusHubWidget::ValidateSettingsConfiguration()
{
	ValidationMessages.Empty();
	bLastValidationPassed = true;

	const UMCore_CommonUISettings* UISettings = UMCore_CommonUISettings::Get();
	const UMCore_ProjectSettings* ProjectSettings = UMCore_ProjectSettings::Get();

	// Check UI Settings exist
	if (!UISettings)
	{
		ValidationMessages.Add(TEXT("Error: UMCore_CommonUISettings not found"));
		bLastValidationPassed = false;
	}
	else
	{
		// Check theme configuration
		if (UISettings->CurrentThemeAsset.IsNull())
		{
			ValidationMessages.Add(TEXT("Warning: No CurrentThemeAsset configured"));
		}

		if (UISettings->AvailableThemes.Num() == 0)
		{
			ValidationMessages.Add(TEXT("Warning: AvailableThemes array is empty"));
		}
		else if (UISettings->DefaultThemeIndex < 0 || UISettings->DefaultThemeIndex >= UISettings->AvailableThemes.Num())
		{
			ValidationMessages.Add(FString::Printf(TEXT("Error: DefaultThemeIndex (%d) out of range [0, %d)"),
				UISettings->DefaultThemeIndex, UISettings->AvailableThemes.Num()));
			bLastValidationPassed = false;
		}

		if (UISettings->CurrentDefaultSettings.IsNull())
		{
			ValidationMessages.Add(TEXT("Warning: No CurrentDefaultSettings configured"));
		}
	}

	// Check Project Settings exist
	if (!ProjectSettings)
	{
		ValidationMessages.Add(TEXT("Error: UMCore_ProjectSettings not found"));
		bLastValidationPassed = false;
	}

	// Success message if no issues
	if (ValidationMessages.Num() == 0)
	{
		ValidationMessages.Add(TEXT("All checks passed"));
	}

	UE_LOG(LogModulusEditor, Log, TEXT("[Validate] %s"), bLastValidationPassed ? TEXT("PASSED") : TEXT("FAILED"));
}

TArray<FString> UModulusHubWidget::GetInstalledModulusPlugins()
{
	TArray<FString> Result;

	TArray<TSharedRef<IPlugin>> AllPlugins = IPluginManager::Get().GetDiscoveredPlugins();
	for (const TSharedRef<IPlugin>& Plugin : AllPlugins)
	{
		if (Plugin->GetName().Contains(TEXT("Modulus")))
		{
			Result.Add(Plugin->GetName());
		}
	}

	return Result;
}

void UModulusHubWidget::RefreshInstalledPlugins()
{
	InstalledPluginNames.Empty();
	InstalledPluginEnabled.Empty();

	TArray<TSharedRef<IPlugin>> AllPlugins = IPluginManager::Get().GetDiscoveredPlugins();
	for (const TSharedRef<IPlugin>& Plugin : AllPlugins)
	{
		if (Plugin->GetName().Contains(TEXT("Modulus")))
		{
			InstalledPluginNames.Add(Plugin->GetName());
			InstalledPluginEnabled.Add(Plugin->IsEnabled());
		}
	}

	UpdatePluginStatus();

	UE_LOG(LogModulusEditor, Log, TEXT("[Refresh Plugins] Found %d Modulus plugin(s)"), InstalledPluginNames.Num());
}

void UModulusHubWidget::UpdatePluginStatus()
{
	// Called on construct and after refresh
	// Blueprint can read InstalledPluginNames and InstalledPluginEnabled arrays
}

void UModulusHubWidget::OpenQuickStartGuide()
{
	//OpenURL(TEXT("[Add URL When Available]"));
	UE_LOG(LogModulusEditor, Log, TEXT("[Docs] Opened Quick Start Guide"));
}

void UModulusHubWidget::OpenAPIReference()
{
	//OpenURL(TEXT("[Add URL When Available]"));
	UE_LOG(LogModulusEditor, Log, TEXT("[Docs] Opened API Reference"));
}

void UModulusHubWidget::OpenURL(const FString& URL)
{
	if (!URL.IsEmpty())
	{
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	}
}
