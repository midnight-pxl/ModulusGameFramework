// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Types/Settings/MCore_GameSettingTemplates.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "PlayerMappableKeySettings.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CoreData/Logging/LogModulusUI.h"

/**=============================================================================
 * Display Settings
 *=============================================================================*/

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateDisplaySettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Display");
    Category.CategoryDescription = FText::FromString("Screen Resolution, Window Mode, and Display Options");
    Category.CategoryTag = MCore_UISettingsTags::UI_Categories_Display;

    /** Screen Resolution */
    FMCore_SettingDefinition Resolution;
    Resolution.DisplayName = FText::FromString("Resolution");
    Resolution.Description = FText::FromString("Screen Resolution");
    Resolution.SettingType = EMCore_SettingType::Dropdown;
    Resolution.SettingTag = MCore_UISettingsTags::Settings_Display_Resolution;
    Resolution.DropdownOptions = {
        FText::FromString("1280x720"),
        FText::FromString("1366x768"),
        FText::FromString("1600x900"),
        FText::FromString("1920x1080"),
        FText::FromString("2560x1080"),
        FText::FromString("2560x1440"),
        FText::FromString("2560x1600"),
        FText::FromString("3440x1440"), 
        FText::FromString("3840x2160"),
    };
    Resolution.DefaultDropdownIndex = 0;
    Category.Settings.Add(Resolution);

    /** Window Mode */
    FMCore_SettingDefinition WindowMode;
    WindowMode.DisplayName = FText::FromString("Window Mode");
    WindowMode.Description = FText::FromString("Fullscreen, Windowed, or Borderless");
    WindowMode.SettingType = EMCore_SettingType::Dropdown;
    WindowMode.SettingTag = MCore_UISettingsTags::Settings_Display_WindowMode;
    WindowMode.DropdownOptions = {
        FText::FromString("Fullscreen"),
        FText::FromString("Windowed"),
        FText::FromString("Borderless Window")
    };
    WindowMode.DefaultDropdownIndex = 0;
    Category.Settings.Add(WindowMode);

    /** VSync */
    FMCore_SettingDefinition VSync;
    VSync.DisplayName = FText::FromString("Vertical Sync");
    VSync.Description = FText::FromString("Prevents screen tearing but may reduce performance");
    VSync.SettingType = EMCore_SettingType::Toggle;
    VSync.SettingTag = MCore_UISettingsTags::Settings_Display_VSync;
    VSync.DefaultToggleValue = true;
    Category.Settings.Add(VSync);

    /** Frame Rate Limit */
    FMCore_SettingDefinition FrameRate;
    FrameRate.DisplayName = FText::FromString("Frame Rate Limit");
    FrameRate.Description = FText::FromString("Maximum frames per second (0 = unlimited)");
    FrameRate.SettingType = EMCore_SettingType::Dropdown;
    FrameRate.SettingTag = MCore_UISettingsTags::Settings_Display_FrameRateLimit;
    FrameRate.DropdownOptions = {
        FText::FromString("Unlimited"),
        FText::FromString("60 FPS"),
        FText::FromString("120 FPS"),
        FText::FromString("144 FPS"),
        FText::FromString("165 FPS"),
        FText::FromString("220 FPS")
    };
    FrameRate.DefaultDropdownIndex = 1; /** 60 FPS */
    Category.Settings.Add(FrameRate);

    return Category;
}

/**=============================================================================
 * Graphic Settings
 *=============================================================================*/

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateGraphicsSettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Graphics");
    Category.CategoryDescription = FText::FromString("Visual quality and rendering options");
    Category.CategoryTag = MCore_UISettingsTags::UI_Categories_Graphics;

    /** Overall Quality Preset */
    FMCore_SettingDefinition QualityPreset;
    QualityPreset.DisplayName = FText::FromString("Quality Preset");
    QualityPreset.Description = FText::FromString("Overall graphics quality level");
    QualityPreset.SettingType = EMCore_SettingType::Dropdown;
    QualityPreset.SettingTag = MCore_UISettingsTags::Settings_Graphics_QualityPreset;
    QualityPreset.DropdownOptions = {
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High"),
        FText::FromString("Ultra"),
        FText::FromString("Custom")
    };
    QualityPreset.DefaultDropdownIndex = 1; /** Medium */
    Category.Settings.Add(QualityPreset);

    /** Texture Quality */
    FMCore_SettingDefinition TextureQuality;
    TextureQuality.DisplayName = FText::FromString("Texture Quality");
    TextureQuality.Description = FText::FromString("Detail level of textures");
    TextureQuality.SettingType = EMCore_SettingType::Dropdown;
    TextureQuality.SettingTag = MCore_UISettingsTags::Settings_Graphics_TextureQuality;
    TextureQuality.DropdownOptions = {
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High"),
        FText::FromString("Ultra")
    };
    TextureQuality.DefaultDropdownIndex = 1; /** Medium */
    Category.Settings.Add(TextureQuality);

    /** Shadow Quality */
    FMCore_SettingDefinition ShadowQuality;
    ShadowQuality.DisplayName = FText::FromString("Shadow Quality");
    ShadowQuality.Description = FText::FromString("Quality and distance of shadows");
    ShadowQuality.SettingType = EMCore_SettingType::Dropdown;
    ShadowQuality.SettingTag = MCore_UISettingsTags::Settings_Graphics_ShadowQuality;
    ShadowQuality.DropdownOptions = {
        FText::FromString("Off"),
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High")
    };
    ShadowQuality.DefaultDropdownIndex = 2; /** Medium */
    Category.Settings.Add(ShadowQuality);

    /** Anti-Aliasing */
    FMCore_SettingDefinition AntiAliasing;
    AntiAliasing.DisplayName = FText::FromString("Anti-Aliasing");
    AntiAliasing.Description = FText::FromString("Smooths jagged edges");
    AntiAliasing.SettingType = EMCore_SettingType::Dropdown;
    AntiAliasing.SettingTag = MCore_UISettingsTags::Settings_Graphics_AntiAliasing;
    AntiAliasing.DropdownOptions = {
        FText::FromString("Off"),
        FText::FromString("FXAA"),
        FText::FromString("TAA"),
        FText::FromString("MSAA 2x"),
        FText::FromString("MSAA 4x")
    };
    AntiAliasing.DefaultDropdownIndex = 2; /** TAA */
    Category.Settings.Add(AntiAliasing);

    /** View Distance */
    FMCore_SettingDefinition ViewDistance;
    ViewDistance.DisplayName = FText::FromString("View Distance");
    ViewDistance.Description = FText::FromString("How far objects are rendered");
    ViewDistance.SettingType = EMCore_SettingType::Slider;
    ViewDistance.SettingTag = MCore_UISettingsTags::Settings_Graphics_ViewDistance;
    ViewDistance.MinValue = 0.1f;
    ViewDistance.MaxValue = 1.0f;
    ViewDistance.DefaultValue = 0.7f;
    ViewDistance.StepSize = 0.1f;
    Category.Settings.Add(ViewDistance);

    return Category;
}

/**=============================================================================
 * Audio Settings
 *=============================================================================*/

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateAudioSettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Audio");
    Category.CategoryDescription = FText::FromString("Volume levels and audio options");
    Category.CategoryTag = MCore_UISettingsTags::UI_Categories_Audio;

    /** Master Volume */
    FMCore_SettingDefinition MasterVolume;
    MasterVolume.DisplayName = FText::FromString("Master Volume");
    MasterVolume.Description = FText::FromString("Overall game volume");
    MasterVolume.SettingType = EMCore_SettingType::Slider;
    MasterVolume.SettingTag = MCore_UISettingsTags::Settings_Audio_MasterVolume;
    MasterVolume.MinValue = 0.0f;
    MasterVolume.MaxValue = 1.0f;
    MasterVolume.DefaultValue = 0.6f;
    MasterVolume.StepSize = 0.01f;
    Category.Settings.Add(MasterVolume);

    /** Music Volume */
    FMCore_SettingDefinition MusicVolume;
    MusicVolume.DisplayName = FText::FromString("Music Volume");
    MusicVolume.Description = FText::FromString("Background music volume");
    MusicVolume.SettingType = EMCore_SettingType::Slider;
    MusicVolume.SettingTag = MCore_UISettingsTags::Settings_Audio_MusicVolume;
    MusicVolume.MinValue = 0.0f;
    MusicVolume.MaxValue = 1.0f;
    MusicVolume.DefaultValue = 0.7f;
    MusicVolume.StepSize = 0.01f;
    Category.Settings.Add(MusicVolume);

    /** SFX Volume */
    FMCore_SettingDefinition SFXVolume;
    SFXVolume.DisplayName = FText::FromString("Effects Volume");
    SFXVolume.Description = FText::FromString("Sound effects volume");
    SFXVolume.SettingType = EMCore_SettingType::Slider;
    SFXVolume.SettingTag = MCore_UISettingsTags::Settings_Audio_SFXVolume;
    SFXVolume.MinValue = 0.0f;
    SFXVolume.MaxValue = 1.0f;
    SFXVolume.DefaultValue = 0.8f;
    SFXVolume.StepSize = 0.01f;
    Category.Settings.Add(SFXVolume);

    /** Voice Volume */
    FMCore_SettingDefinition VoiceVolume;
    VoiceVolume.DisplayName = FText::FromString("Voice Volume");
    VoiceVolume.Description = FText::FromString("Character dialogue volume");
    VoiceVolume.SettingType = EMCore_SettingType::Slider;
    VoiceVolume.SettingTag = MCore_UISettingsTags::Settings_Audio_VoiceVolume;
    VoiceVolume.MinValue = 0.0f;
    VoiceVolume.MaxValue = 1.0f;
    VoiceVolume.DefaultValue = 0.9f;
    VoiceVolume.StepSize = 0.01f;
    Category.Settings.Add(VoiceVolume);

    /** Audio Quality */
    FMCore_SettingDefinition AudioQuality;
    AudioQuality.DisplayName = FText::FromString("Audio Quality");
    AudioQuality.Description = FText::FromString("Audio processing quality");
    AudioQuality.SettingType = EMCore_SettingType::Dropdown;
    AudioQuality.SettingTag = MCore_UISettingsTags::Settings_Audio_Quality;
    AudioQuality.DropdownOptions = {
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High")
    };
    AudioQuality.DefaultDropdownIndex = 1; /** Medium */
    Category.Settings.Add(AudioQuality);

    return Category;
}

/**=============================================================================
 * Controls Settings
 *=============================================================================*/

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateControlsSettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Controls");
    Category.CategoryDescription = FText::FromString("Input sensitivity and key bindings");
    Category.CategoryTag = MCore_UISettingsTags::UI_Categories_Controls;

    /** Mouse Sensitivity */
    FMCore_SettingDefinition MouseSensitivity;
    MouseSensitivity.DisplayName = FText::FromString("Mouse Sensitivity");
    MouseSensitivity.Description = FText::FromString("How fast the camera moves with mouse input");
    MouseSensitivity.SettingType = EMCore_SettingType::Slider;
    MouseSensitivity.SettingTag = MCore_UISettingsTags::Settings_Controls_MouseSensitivity;
    MouseSensitivity.MinValue = 0.1f;
    MouseSensitivity.MaxValue = 3.0f;
    MouseSensitivity.DefaultValue = 1.0f;
    MouseSensitivity.StepSize = 0.02f;
    Category.Settings.Add(MouseSensitivity);

    /** Invert Y Axis */
    FMCore_SettingDefinition InvertY;
    InvertY.DisplayName = FText::FromString("Invert Y Axis");
    InvertY.Description = FText::FromString("Invert vertical mouse movement");
    InvertY.SettingType = EMCore_SettingType::Toggle;
    InvertY.SettingTag = MCore_UISettingsTags::Settings_Controls_InvertY;
    InvertY.DefaultToggleValue = false;
    Category.Settings.Add(InvertY);

    /** Gamepad Sensitivity */
    FMCore_SettingDefinition GamepadSensitivity;
    GamepadSensitivity.DisplayName = FText::FromString("Gamepad Sensitivity");
    GamepadSensitivity.Description = FText::FromString("Controller stick sensitivity");
    GamepadSensitivity.SettingType = EMCore_SettingType::Slider;
    GamepadSensitivity.SettingTag = MCore_UISettingsTags::Settings_Controls_GamepadSensitivity;
    GamepadSensitivity.MinValue = 0.1f;
    GamepadSensitivity.MaxValue = 3.0f;
    GamepadSensitivity.DefaultValue = 1.0f;
    GamepadSensitivity.StepSize = 0.02f;
    Category.Settings.Add(GamepadSensitivity);

    /** Key Binding Examples (commonly remapped keys) */
    FMCore_SettingDefinition MoveForward;
    MoveForward.DisplayName = FText::FromString("Move Forward");
    MoveForward.Description = FText::FromString("Key to move forward");
    MoveForward.SettingType = EMCore_SettingType::KeyBinding;
    MoveForward.SettingTag = MCore_UISettingsTags::Settings_Controls_MoveForward;
    MoveForward.DefaultKey = EKeys::W;
    Category.Settings.Add(MoveForward);

    FMCore_SettingDefinition Jump;
    Jump.DisplayName = FText::FromString("Jump");
    Jump.Description = FText::FromString("Key to jump");
    Jump.SettingType = EMCore_SettingType::KeyBinding;
    Jump.SettingTag = MCore_UISettingsTags::Settings_Controls_Jump;
    Jump.DefaultKey = EKeys::SpaceBar;
    Category.Settings.Add(Jump);

    return Category;
}

/**=============================================================================
 * Accessibility Settings
 *=============================================================================*/

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateAccessibilitySettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Accessibility");
    Category.CategoryDescription = FText::FromString("Options to improve game accessibility");
    Category.CategoryTag = MCore_UISettingsTags::UI_Categories_Accessibility;

    /** Subtitles */
    FMCore_SettingDefinition Subtitles;
    Subtitles.DisplayName = FText::FromString("Subtitles");
    Subtitles.Description = FText::FromString("Show text for spoken dialogue");
    Subtitles.SettingType = EMCore_SettingType::Toggle;
    Subtitles.SettingTag = MCore_UISettingsTags::Settings_Accessibility_Subtitles;
    Subtitles.DefaultToggleValue = false;
    Category.Settings.Add(Subtitles);

    /** Subtitle Size */
    FMCore_SettingDefinition SubtitleSize;
    SubtitleSize.DisplayName = FText::FromString("Subtitle Size");
    SubtitleSize.Description = FText::FromString("Size of subtitle text");
    SubtitleSize.SettingType = EMCore_SettingType::Dropdown;
    SubtitleSize.SettingTag = MCore_UISettingsTags::Settings_Accessibility_SubtitleSize;
    SubtitleSize.DropdownOptions = {
        FText::FromString("Small"),
        FText::FromString("Medium"),
        FText::FromString("Large"),
        FText::FromString("Extra Large")
    };
    SubtitleSize.DefaultDropdownIndex = 1; /** Medium */
    Category.Settings.Add(SubtitleSize);

    /** Colorblind Support */
    FMCore_SettingDefinition ColorblindMode;
    ColorblindMode.DisplayName = FText::FromString("Colorblind Support");
    ColorblindMode.Description = FText::FromString("Adjust colors for colorblind players");
    ColorblindMode.SettingType = EMCore_SettingType::Dropdown;
    ColorblindMode.SettingTag = MCore_UISettingsTags::Settings_Accessibility_ColorblindMode;
    ColorblindMode.DropdownOptions = {
        FText::FromString("None"),
        FText::FromString("Protanopia"),
        FText::FromString("Deuteranopia"),
        FText::FromString("Tritanopia")
    };
    ColorblindMode.DefaultDropdownIndex = 0; /** None */
    Category.Settings.Add(ColorblindMode);

    /** UI Scale */
    FMCore_SettingDefinition UIScale;
    UIScale.DisplayName = FText::FromString("UI Scale");
    UIScale.Description = FText::FromString("Size of user interface elements");
    UIScale.SettingType = EMCore_SettingType::Slider;
    UIScale.SettingTag = MCore_UISettingsTags::Settings_Accessibility_UIScale;
    UIScale.MinValue = 0.4f;
    UIScale.MaxValue = 2.0f;
    UIScale.DefaultValue = 1.0f;
    UIScale.StepSize = 0.2f;
    Category.Settings.Add(UIScale);

    /** Reduce Motion */
    FMCore_SettingDefinition ReduceMotion;
    ReduceMotion.DisplayName = FText::FromString("Reduce Motion");
    ReduceMotion.Description = FText::FromString("Reduce screen shake and camera effects");
    ReduceMotion.SettingType = EMCore_SettingType::Toggle;
    ReduceMotion.SettingTag = MCore_UISettingsTags::Settings_Accessibility_ReduceMotion;
    ReduceMotion.DefaultToggleValue = false;
    Category.Settings.Add(ReduceMotion);

    return Category;
}

void UMCore_GameSettingTemplates::GetMappableActionsFromContext(const UInputMappingContext* Context,
    ECommonInputType InputType, TArray<UInputAction*>& OutActions)
{
    OutActions.Empty();
    if (!Context) return;

    const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
    TSet<const UInputAction*> UniqueActions;

    for (const FEnhancedActionKeyMapping& Mapping : Mappings)
    {
        const UInputAction* Action = Mapping.Action;
        if (!Action || UniqueActions.Contains(Action)) continue;

        if (!Action->GetPlayerMappableKeySettings()) continue;

        const bool bIsGamepadKey = Mapping.Key.IsGamepadKey();
        const bool bWantGamepad = (InputType == ECommonInputType::Gamepad);
        if (bIsGamepadKey != bWantGamepad) continue;

        UniqueActions.Add(Action);
        OutActions.Add(const_cast<UInputAction*>(Action));
    }
}

TArray<FMCore_KeyBindingCategory> UMCore_GameSettingTemplates::GetAllKeyBindingCategories(UObject* WorldContext,
    ECommonInputType InputType)
{
    TArray<FMCore_KeyBindingCategory> Categories;
    
    if (!WorldContext)
    {
        UE_LOG(LogTemp, Error, TEXT("GetAllKeyBindingCategories: Invalid WorldContext"));
        return Categories;
    }

    /** Get Enhanced Input subsystem */
    const UWorld* World = WorldContext->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("GetAllKeyBindingCategories: Invalid World"));
        return Categories;
    }
    
    const UGameInstance* GameInstance = World->GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("GetAllKeyBindingCategories: No GameInstance"));
        return Categories;
    }
    
    const ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
    if (!LocalPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("GetAllKeyBindingCategories: No LocalPlayer"));
        return Categories;
    }
    
    const UEnhancedInputLocalPlayerSubsystem* Subsystem = 
        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem)
    {
        UE_LOG(LogTemp, Error, 
            TEXT("GetAllKeyBindingCategories: EnhancedInputLocalPlayerSubsystem not found. "
                 "Is the Enhanced Input plugin enabled?"));
        return Categories;
    }
    
    const UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings)
    {
        UE_LOG(LogTemp, Error, 
            TEXT("GetAllKeyBindingCategories: EnhancedInputUserSettings not initialized. "
                 "Enable User Settings in Project Settings > Enhanced Input."));
        return Categories;
    }
    
    const UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();
    if (!Profile)
    {
        UE_LOG(LogTemp, Error, 
            TEXT("GetAllKeyBindingCategories: No active key profile found."));
        return Categories;
    }

    /** Get all player mapping rows from the key profile */
    const TMap<FName, FKeyMappingRow>& MappingRows = Profile->GetPlayerMappingRows();
    
    if (MappingRows.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, 
            TEXT("GetAllKeyBindingCategories: No mappings found in key profile. "
                 "Are Input Mapping Contexts registered and do they have player-mappable actions?"));
        return Categories;
    }
    
    TMap<FName, TArray<FMCore_SettingDefinition>> CategoryMap;
    
    for (const auto& [ActionName, MappingRow] : MappingRows)
    {
        /** Each row contains potentially multiple key mappings for the same action */
        for (const FPlayerKeyMapping& Mapping : MappingRow.Mappings)
        {
            const bool bIsGamepadKey = Mapping.GetCurrentKey().IsGamepadKey();
            const bool bWantGamepad = (InputType == ECommonInputType::Gamepad);
            if (bIsGamepadKey != bWantGamepad) { continue; }
            
            const UInputAction* Action = Mapping.GetAssociatedInputAction();
            if (!Action) { continue; }

            const UPlayerMappableKeySettings* MappableSettings = Action->GetPlayerMappableKeySettings();
            if (!MappableSettings)
            {
                /** Action isn't marked as player mappable - skip it */
                continue;
            }

            /** Get category from action metadata */
            FName CategoryName = GetActionDisplayCategory(Action);

            /** Create setting definition from mapping */
            FMCore_SettingDefinition KeyBinding;
            KeyBinding.SettingType = EMCore_SettingType::KeyBinding;
            KeyBinding.DisplayName = MappableSettings->DisplayName;
            KeyBinding.Description = FText::FromString(MappableSettings->DisplayCategory.ToString()); /** Category as description */

            /** Create tag from action name */
            const FString TagString = FString::Printf(TEXT("MCore.Settings.Controls.%s"),
                *ActionName.ToString());
            KeyBinding.SettingTag = FGameplayTag::RequestGameplayTag(FName(*TagString), false);

            /** Store current key */
            KeyBinding.DefaultKey = Mapping.GetCurrentKey();

            /** Add to category map */
            if (KeyBinding.SettingTag.IsValid())
            {
                CategoryMap.FindOrAdd(CategoryName).Add(KeyBinding);
            }
        }
    }

    /** Convert map to array of structs for Blueprint compatibility */
    for (const auto& [CategoryName, KeyBindings] : CategoryMap)
    {
        FMCore_KeyBindingCategory Category;
        Category.CategoryName = FText::FromName(CategoryName);
        Category.KeyBindings = KeyBindings;
        Categories.Add(Category);
    }

    /** Sort categories alphabetically for consistent UI */
    Categories.Sort([](const FMCore_KeyBindingCategory& A, const FMCore_KeyBindingCategory& B)
    {
        return A.CategoryName.CompareTo(B.CategoryName) < 0;
    });
    
    UE_LOG(LogTemp, Log, 
        TEXT("GetAllKeyBindingCategories: Discovered %d categories with total %d actions for %s"), 
        Categories.Num(),
        MappingRows.Num(),
        InputType == ECommonInputType::Gamepad ? TEXT("Gamepad") : TEXT("Keyboard/Mouse"));
    
    return Categories;
}

FName UMCore_GameSettingTemplates::GetActionDisplayCategory(const UInputAction* Action)
{
    if (!Action) return NAME_None;
    
    const UPlayerMappableKeySettings* Settings = Action->GetPlayerMappableKeySettings();
    if (!Settings || Settings->DisplayCategory.IsEmpty())
    {
        return FName(TEXT("General"));
    }
    return FName(*Settings->DisplayCategory.ToString());
}

TArray<FName> UMCore_GameSettingTemplates::GetActionsUsingKey(FKey Key, FName ExcludeAction)
{
    TArray<FName> ConflictingActions;

    const UGameInstance* GameInstance = GEngine->GameViewport ? 
        GEngine->GameViewport->GetGameInstance() : nullptr;
    if (!GameInstance) return ConflictingActions;

    const ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
    if (!LocalPlayer) return ConflictingActions;

    const UEnhancedInputLocalPlayerSubsystem* Subsystem = 
        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return ConflictingActions;

    const UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return ConflictingActions;

    const UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();
    if (!Profile) return ConflictingActions;

    const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();
    for (const auto& [ActionName, Row] : Rows)
    {
        if (ActionName == ExcludeAction) continue;

        for (const FPlayerKeyMapping& Mapping : Row.Mappings)
        {
            if (Mapping.GetCurrentKey() == Key)
            {
                ConflictingActions.Add(ActionName);
                break;
            }
        }
    }
    return ConflictingActions;
}

FKey UMCore_GameSettingTemplates::GetCurrentKeyForAction(const UInputAction* Action, EPlayerMappableKeySlot Slot)
{
    if (!Action) return EKeys::Invalid;

    const UGameInstance* GameInstance = GEngine->GameViewport ? 
        GEngine->GameViewport->GetGameInstance() : nullptr;
    if (!GameInstance) return EKeys::Invalid;

    const ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
    if (!LocalPlayer) return EKeys::Invalid;

    const UEnhancedInputLocalPlayerSubsystem* Subsystem = 
        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return EKeys::Invalid;

    const UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return EKeys::Invalid;

    const UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();
    if (!Profile) return EKeys::Invalid;

    const FKeyMappingRow* Row = Profile->FindKeyMappingRow(Action->GetFName());
    if (!Row) return EKeys::Invalid;

    /** TSet - iterate to find the mapping with matching Slot */
    for (const FPlayerKeyMapping& Mapping : Row->Mappings)
    {
        if (Mapping.GetSlot() == Slot)
        {
            return Mapping.GetCurrentKey();
        }
    }

    return EKeys::Invalid;
}

FMCore_SettingDefinition UMCore_GameSettingTemplates::CreateSliderSetting(
    const FGameplayTag& SettingTag,
    const FText& DisplayName,
    const FText& Description,
    float MinValue,
    float MaxValue,
    float DefaultValue,
    float StepValue)
{
    FMCore_SettingDefinition Setting;
    Setting.SettingType = EMCore_SettingType::Slider;
    Setting.SettingTag = SettingTag;
    Setting.DisplayName = DisplayName;
    Setting.Description = Description;

    /** Ensure valid range */
    Setting.MinValue = FMath::Min(MinValue, MaxValue);
    Setting.MaxValue = FMath::Max(MinValue, MaxValue);

    /** Clamp default to valid range */
    Setting.DefaultValue = FMath::Clamp(DefaultValue, Setting.MinValue, Setting.MaxValue);

    /** Ensure positive step size */
    Setting.StepSize = FMath::Max(0.001f, StepValue);
    
    return Setting;
}

FMCore_SettingDefinition UMCore_GameSettingTemplates::CreateDropdownSetting(
    const FGameplayTag& SettingTag,
    const FText& DisplayName,
    const FText& Description,
    const TArray<FText>& Options,
    int32 DefaultIndex)
{
    FMCore_SettingDefinition Setting;
    Setting.SettingType = EMCore_SettingType::Dropdown;
    Setting.SettingTag = SettingTag;
    Setting.DisplayName = DisplayName;
    Setting.Description = Description;
    Setting.DropdownOptions = Options;

    /** Ensure valid default index */
    if (Options.Num() > 0)
    {
        Setting.DefaultDropdownIndex = FMath::Clamp(DefaultIndex, 0, Options.Num() - 1);
    }
    else
    {
        Setting.DefaultDropdownIndex = 0;
        UE_LOG(LogModulusUI, Warning, TEXT("CreateDropdownSetting: '%s' has no options"), 
            *DisplayName.ToString());
    }
    
    return Setting;
}

FMCore_SettingDefinition UMCore_GameSettingTemplates::CreateToggleSetting(
    const FGameplayTag& SettingTag,
    const FText& DisplayName,
    const FText& Description,
    bool DefaultValue)
{
    FMCore_SettingDefinition Setting;
    Setting.SettingType = EMCore_SettingType::Toggle;
    Setting.SettingTag = SettingTag;
    Setting.DisplayName = DisplayName;
    Setting.Description = Description;
    Setting.DefaultToggleValue = DefaultValue;
    
    return Setting;
}

FMCore_SettingDefinition UMCore_GameSettingTemplates::CreateKeyBindingSetting(
    const UInputAction* InputAction,
    const UInputMappingContext* MappingContext)
{
    FMCore_SettingDefinition KeyBinding;
    
    if (!InputAction)
    {
        UE_LOG(LogModulusUI, Error, TEXT("CreateKeyBindingSetting: InputAction is null"));
        return KeyBinding;
    }

    /** Check if action is player-mappable */
    const UPlayerMappableKeySettings* MappableSettings = InputAction->GetPlayerMappableKeySettings();
    if (!MappableSettings)
    {
        UE_LOG(LogModulusUI, Warning, 
            TEXT("CreateKeyBindingSetting: InputAction '%s' is not marked as Player Mappable. "
                 "Enable 'Player Mappable Key Settings' in the Input Action asset."), 
            *InputAction->GetName());
        return KeyBinding;
    }

    /** Configure setting definition from player mappable settings */
    KeyBinding.SettingType = EMCore_SettingType::KeyBinding;
    KeyBinding.DisplayName = MappableSettings->DisplayName;
    KeyBinding.Description = MappableSettings->DisplayCategory;

    /** Create tag from action name for identification */
    const FString TagString = FString::Printf(TEXT("MCore.Settings.Controls.%s"),
        *InputAction->GetFName().ToString());
    KeyBinding.SettingTag = FGameplayTag::RequestGameplayTag(FName(*TagString), false);

    /** Get current key mapping (defaults to first slot) */
    KeyBinding.DefaultKey = GetCurrentKeyForAction(InputAction, EPlayerMappableKeySlot::First);
    
    return KeyBinding;
}

/**=============================================================================
 * Template Functions
 *=============================================================================*/

TArray<FMCore_SettingDefinition> UMCore_GameSettingTemplates::CreateBasicControlDefinition()
{
    TArray<FMCore_SettingDefinition> BasicControls;

    /** Mouse Sensitivity */
    BasicControls.Add(CreateSliderSetting(
        MCore_UISettingsTags::Settings_Controls_MouseSensitivity,
        FText::FromString("Mouse Sensitivity"),
        FText::FromString("How fast the camera moves with mouse input"),
        0.1f,  /** Min */
        3.0f,  /** Max */
        1.0f,  /** Default */
        0.05f  /** Step */
    ));

    /** Invert Y Axis */
    BasicControls.Add(CreateToggleSetting(
        MCore_UISettingsTags::Settings_Controls_InvertY,
        FText::FromString("Invert Y Axis"),
        FText::FromString("Invert vertical mouse movement"),
        false /** Default off */
    ));

    /** Gamepad Sensitivity */
    BasicControls.Add(CreateSliderSetting(
        MCore_UISettingsTags::Settings_Controls_GamepadSensitivity,
        FText::FromString("Gamepad Sensitivity"),
        FText::FromString("Controller stick sensitivity"),
        0.1f,  /** Min */
        3.0f,  /** Max */
        1.0f,  /** Default */
        0.05f  /** Step */
    ));
    
    return BasicControls;
}

/**=============================================================================
 * Validation utilities
 *=============================================================================*/

bool UMCore_GameSettingTemplates::ValidateEnhancedInputSetup(
    UObject* WorldContext, 
    TArray<FString>& ValidationErrors)
{
    ValidationErrors.Empty();
    
    if (!WorldContext)
    {
        ValidationErrors.Add("Invalid WorldContext");
        return false;
    }
    
    const UWorld* World = WorldContext->GetWorld();
    if (!World)
    {
        ValidationErrors.Add("No valid World found");
        return false;
    }
    
    const UGameInstance* GameInstance = World->GetGameInstance();
    if (!GameInstance)
    {
        ValidationErrors.Add("No GameInstance found");
        return false;
    }
    
    const ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
    if (!LocalPlayer)
    {
        ValidationErrors.Add("No LocalPlayer found - is this being called before player initialization?");
        return false;
    }

    /** Validate Enhanced Input subsystem exists */
    const UEnhancedInputLocalPlayerSubsystem* Subsystem =
        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem)
    {
        ValidationErrors.Add("EnhancedInputLocalPlayerSubsystem not found");
        ValidationErrors.Add("Solution: Enable the Enhanced Input plugin in Project Settings");
        return false;
    }

    /** Validate user settings initialized */
    const UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings)
    {
        ValidationErrors.Add("EnhancedInputUserSettings not initialized");
        ValidationErrors.Add("Solution: Enable 'User Settings' in Project Settings > Enhanced Input");
        return false;
    }

    /** Validate active key profile exists */
    const UEnhancedPlayerMappableKeyProfile* Profile = UserSettings->GetActiveKeyProfile();
    if (!Profile)
    {
        ValidationErrors.Add("No active key profile found");
        ValidationErrors.Add("Solution: Ensure at least one key profile is created in User Settings");
        return false;
    }

    /** Validate mappings exist in profile */
    const TMap<FName, FKeyMappingRow>& MappingRows = Profile->GetPlayerMappingRows();
    if (MappingRows.Num() == 0)
    {
        ValidationErrors.Add("No input mappings found in key profile");
        ValidationErrors.Add("Solution: Register Input Mapping Contexts with UserSettings->RegisterInputMappingContext()");
        return false;
    }

    /** Check if contexts have player-mappable actions */
    int32 TotalMappableActions = 0;
    for (const auto& [ActionName, MappingRow] : MappingRows)
    {
        TotalMappableActions += MappingRow.Mappings.Num();
    }
    
    if (TotalMappableActions == 0)
    {
        ValidationErrors.Add("No player-mappable actions found in registered contexts");
        ValidationErrors.Add("Solution: Mark Input Actions as 'Player Mappable' in their asset settings");
        return false;
    }

    /** Validation passed */
    UE_LOG(LogTemp, Log, 
        TEXT("Enhanced Input validation passed: %d mapping rows, %d mappable actions"), 
        MappingRows.Num(), TotalMappableActions);
    
    return true;
}
