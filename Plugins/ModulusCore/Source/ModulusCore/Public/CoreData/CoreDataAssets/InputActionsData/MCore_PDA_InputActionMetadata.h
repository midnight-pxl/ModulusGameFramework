// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "MCore_PDA_InputActionMetadata.generated.h"

/**
 * Primary Data Asset for extended Input Action metadata
 * Attach this to InputAction->PlayerMappableKeySettings->MetaData for enhanced functionality
 */
UCLASS()
class MODULUSCORE_API UMCore_PDA_InputActionMetadata : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UMCore_PDA_InputActionMetadata();

    //~ Begin UPrimaryDataAsset Interface
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    virtual FString GetPrimaryAssetName() const;
    //~ End UPrimaryDataAsset Interface

    // Display Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", 
              meta = (MultiLine = "true", ToolTip = "Detailed description shown in tooltips and help"))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display",
              meta = (ClampMin = "0", ClampMax = "999", ToolTip = "Sort order for settings UI (lower = higher in list)"))
    int32 SortOrder{100};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display",
              meta = (ToolTip = "Hide this action from settings/remapping UI"))
    bool bHiddenFromUI{false};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display",
              meta = (ToolTip = "Icon to display next to action in UI"))
    TObjectPtr<UTexture2D> IconTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display",
              meta = (ToolTip = "Color tint for action display"))
    FLinearColor DisplayColor = FLinearColor::White;

    // Validation Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation",
              meta = (ToolTip = "Keys that cannot be bound to this action"))
    TArray<FKey> RestrictedKeys;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation",
              meta = (ToolTip = "Allow modifier keys (Ctrl, Alt, Shift) with this action"))
    bool bAllowModifierKeys{true};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation",
              meta = (ToolTip = "Minimum number of key bindings required for this action"))
    int32 MinRequiredBindings{0};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation",
              meta = (ToolTip = "Maximum number of key bindings allowed for this action"))
    int32 MaxAllowedBindings{5};

    // Advanced Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced",
              meta = (ToolTip = "Platform-specific visibility rules"))
    TArray<FString> SupportedPlatforms;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced",
              meta = (ToolTip = "Developer notes for this action (editor only)"))
    FString DeveloperNotes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced",
              meta = (ToolTip = "Tags for grouping and filtering actions"))
    TArray<FName> ActionTags;

    // Validation helpers
    UFUNCTION(BlueprintPure, Category = "Validation")
    bool IsKeyAllowed(FKey Key) const;

    UFUNCTION(BlueprintPure, Category = "Validation") 
    bool IsPlatformSupported(const FString& PlatformName) const;
};
