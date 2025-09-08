// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreGameSettings/MCore_SettingsWidgetFactory.h"

/**
 * UMCore_SettingsWidgetFactory
 */
UCommonActivatableWidget* UMCore_SettingsWidgetFactory::CreateSettingsMenu(
	const FMCore_SettingsConfiguration& Configuration, UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateCategoryPanel(const FMCore_SettingCategory& Category, UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateSettingWidget(const FMCore_SettingDefinition& Setting, UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::GetOrCreateSettingWidget(const FMCore_SettingDefinition& Setting,
	UObject* Outer)
{
	return nullptr;
}

bool UMCore_SettingsWidgetFactory::ReturnWidgetToPool(UUserWidget* Widget, const FGuid& WidgetID)
{
	return false;
}

bool UMCore_SettingsWidgetFactory::ReturnWidgetToPoolByReference(UUserWidget* Widget)
{
	return false;
}

void UMCore_SettingsWidgetFactory::ClearWidgetPools()
{
}

TMap<FGameplayTag, int32> UMCore_SettingsWidgetFactory::GetPoolStatistics()
{
	return {};
}

TSubclassOf<UUserWidget> UMCore_SettingsWidgetFactory::GetWidgetClassForSettingType(EMCore_SettingType SettingType)
{
	return nullptr;
}

TSubclassOf<UUserWidget> UMCore_SettingsWidgetFactory::GetWidgetClassForTag(const FGameplayTag& WidgetTypeTag)
{
	return nullptr;
}

bool UMCore_SettingsWidgetFactory::CanCreateWidgetForSetting(const FMCore_SettingDefinition& Setting)
{
	return false;
}

FString UMCore_SettingsWidgetFactory::GetSettingValue(const FGameplayTag& SaveKey, const FString& DefaultValue)
{
	return DefaultValue;
}

void UMCore_SettingsWidgetFactory::SetSettingValue(const FGameplayTag& SaveKey, const FString& Value,
	bool bSaveImmediately)
{
}

FString UMCore_SettingsWidgetFactory::GetSettingValueString(const FString& SaveKey, const FString& DefaultValue)
{
	return DefaultValue;
}

void UMCore_SettingsWidgetFactory::SetSettingValueString(const FString& SaveKey, const FString& Value,
	bool bSaveImmediately)
{
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateToggleWidget(const FMCore_SettingDefinition& Setting, UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateSliderWidget(const FMCore_SettingDefinition& Setting, UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateDropdownWidget(const FMCore_SettingDefinition& Setting, UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateTextInputWidget(const FMCore_SettingDefinition& Setting,
	UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateKeyBindingWidget(const FMCore_SettingDefinition& Setting,
	UObject* Outer)
{
	return nullptr;
}

UUserWidget* UMCore_SettingsWidgetFactory::CreateActionWidget(const FMCore_SettingDefinition& Setting, UObject* Outer)
{
	return nullptr;
}

void UMCore_SettingsWidgetFactory::ConfigureWidget(UUserWidget* Widget, const FMCore_SettingDefinition& Setting)
{
}

void UMCore_SettingsWidgetFactory::SetupWidgetEvents(UUserWidget* Widget, const FMCore_SettingDefinition& Setting)
{
}

UUserWidget* UMCore_SettingsWidgetFactory::GetPooledWidget(const FGameplayTag& WidgetTypeTag,
                                                           const FMCore_SettingDefinition& Setting, UObject* Outer)
{
	return nullptr;
}

FGuid UMCore_SettingsWidgetFactory::AddToPool(UUserWidget* Widget, const FGameplayTag& WidgetTypeTag,
	const FGameplayTag& SettingTag)
{
	return {};
}

void UMCore_SettingsWidgetFactory::CleanupPools()
{
}

bool UMCore_SettingsWidgetFactory::CanReuseWidget(const FMCore_WidgetPoolEntry& PoolEntry,
	const FMCore_SettingDefinition& Setting)
{
	return false;
}

void UMCore_SettingsWidgetFactory::ResetWidgetForReuse(UUserWidget* Widget, const FMCore_SettingDefinition& Setting)
{
}

/**
 * UMCore_SettingWidgetBase
 */
UMCore_SettingWidgetBase::UMCore_SettingWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_SettingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMCore_SettingWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMCore_SettingWidgetBase::NotifyValueChanged(const FString& NewValue)
{
}

UMCore_SettingsMenuWidget::UMCore_SettingsMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_SettingsMenuWidget::SaveAllSettings()
{
}

void UMCore_SettingsMenuWidget::ResetAllSettings()
{
}

void UMCore_SettingsMenuWidget::ApplySettings()
{
}

FString UMCore_SettingsMenuWidget::GetSettingByTag(const FGameplayTag& SaveKey, const FString& DefaultValue) const
{
	return "";
}

void UMCore_SettingsMenuWidget::SetSettingByTag(const FGameplayTag& SaveKey, const FString& Value)
{
}

void UMCore_SettingsMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UMCore_SettingsMenuWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

void UMCore_SettingsMenuWidget::OnSettingChanged(const FGameplayTag& SaveKey, const FString& NewValue)
{
}

UCommonActivatableWidget* UMCore_SettingsBlueprintLibrary::ShowDefaultSettingsMenu(UObject* WorldContext)
{
	return nullptr;
}

UCommonActivatableWidget* UMCore_SettingsBlueprintLibrary::ShowCustomSettingsMenu(UObject* WorldContext,
	const FMCore_SettingsConfiguration& Configuration)
{
	return nullptr;
}

float UMCore_SettingsBlueprintLibrary::GetFloatSettingByTag(const FGameplayTag& SaveKey, float DefaultValue)
{
	return 0;
}

void UMCore_SettingsBlueprintLibrary::SetFloatSettingByTag(const FGameplayTag& SaveKey, float Value)
{
}

bool UMCore_SettingsBlueprintLibrary::GetBoolSettingByTag(const FGameplayTag& SaveKey, bool DefaultValue)
{
	return false;
}

void UMCore_SettingsBlueprintLibrary::SetBoolSettingByTag(const FGameplayTag& SaveKey, bool Value)
{
}

int32 UMCore_SettingsBlueprintLibrary::GetIntSettingByTag(const FGameplayTag& SaveKey, int32 DefaultValue)
{
	return 0;
}

void UMCore_SettingsBlueprintLibrary::SetIntSettingByTag(const FGameplayTag& SaveKey, int32 Value)
{
}

float UMCore_SettingsBlueprintLibrary::GetFloatSetting(const FString& SaveKey, float DefaultValue)
{
	return 0;
}

void UMCore_SettingsBlueprintLibrary::SetFloatSetting(const FString& SaveKey, float Value)
{
}

bool UMCore_SettingsBlueprintLibrary::GetBoolSetting(const FString& SaveKey, bool DefaultValue)
{
	return false;
}

void UMCore_SettingsBlueprintLibrary::SetBoolSetting(const FString& SaveKey, bool Value)
{
}

int32 UMCore_SettingsBlueprintLibrary::GetIntSetting(const FString& SaveKey, int32 DefaultValue)
{
	return 0;
}

void UMCore_SettingsBlueprintLibrary::SetIntSetting(const FString& SaveKey, int32 Value)
{
}

FGameplayTag UMCore_SettingsBlueprintLibrary::ConvertStringToSettingTag(const FString& SaveKey)
{
	return FGameplayTag();
}

FString UMCore_SettingsBlueprintLibrary::ConvertSettingTagToString(const FGameplayTag& SaveKey)
{
	return "";
}
