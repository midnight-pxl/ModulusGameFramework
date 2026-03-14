// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_SettingsTypes.generated.h"

class UMCore_DA_SettingDefinition;

/** Setting widget types for game settings menus */
UENUM(BlueprintType)
enum class EMCore_SettingType : uint8
{
    Toggle       UMETA(DisplayName = "Toggle (Boolean)"),
    Slider       UMETA(DisplayName = "Slider (Float)"),
    Dropdown     UMETA(DisplayName = "Dropdown (Selection)")
};

/** A single float setting + value pair for batch setter operations */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_FloatSettingChange
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
        meta = (DisplayName = "Setting"))
    TObjectPtr<UMCore_DA_SettingDefinition> Setting = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
        meta = (DisplayName = "Value"))
    float Value = 0.f;
};

/** A single int setting + value pair for batch setter operations */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_IntSettingChange
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
        meta = (DisplayName = "Setting"))
    TObjectPtr<UMCore_DA_SettingDefinition> Setting = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
        meta = (DisplayName = "Value"))
    int32 Value = 0;
};

/** A single bool setting + value pair for batch setter operations */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_BoolSettingChange
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
        meta = (DisplayName = "Setting"))
    TObjectPtr<UMCore_DA_SettingDefinition> Setting = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
        meta = (DisplayName = "Value"))
    bool Value = false;
};