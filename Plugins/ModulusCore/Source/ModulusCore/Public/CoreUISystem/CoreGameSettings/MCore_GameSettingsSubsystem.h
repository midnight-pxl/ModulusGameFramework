// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_GameSettingsSubsystem.generated.h"

class FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameSettingChanged, const FGameplayTag&, SaveKey, const FString&, NewSettingValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameSettingApplied, const FGameplayTag&, SaveKey, const FString&, NewSettingValue);

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_GameSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    	virtual void Deinitialize() override;
    
    	UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        FString GetSettingValue(const FGameplayTag& SaveKey, const FString& SettingValue = "");
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void SetSettingValue(const FGameplayTag& SaveKey, const FString& NewSettingValue, bool bSaveImmediately = true);
    
        /**
         * Typed convenience functions
         */
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        float GetFloatSetting(const FGameplayTag& SaveKey, float SettingValue = 0.0f);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void SetFloatSetting(const FGameplayTag& SaveKey, float NewSettingValue);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        bool GetBoolSetting(const FGameplayTag& SaveKey, bool SettingValue = false);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void SetBoolSetting(const FGameplayTag& SaveKey, bool NewSettingValue);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        int32 GetIntSetting(const FGameplayTag& SaveKey, int32 DefaultValue = 0);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void SetIntSetting(const FGameplayTag& SaveKey, int32 NewSettingValue);
    	
        /** Bulk Ops */
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        TMap<FGameplayTag, FString> GetMultipleSettings(const TArray<FGameplayTag>& SaveKeys);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void SetMultipleSettings(const TMap<FGameplayTag, FString>& Settings, bool bSaveImmediately = true);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
        bool HasSetting(const FGameplayTag& SaveKey) const;
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
        TMap<FGameplayTag, FString> GetAllSettings() const { return CurrentSettings; }
    	
        /** Save/Load Integration */
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void SaveAllSettings();
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void LoadAllSettings();
    
    	UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    	void ApplySettingImmediately(const FGameplayTag& SaveKey, const FString& NewSettingValue);
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void ResetAllSettings();
    
        UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
        void ResetCategorySettings(const FGameplayTag& CategoryTag);
    	
        /** Events */
    
        UPROPERTY(BlueprintAssignable, Category = "Modulus Settings")
        FOnGameSettingChanged OnGameSettingChangedEvent;
    	UPROPERTY(BlueprintAssignable, Category = "Modulus Settings")
    	FOnGameSettingApplied OnGameSettingAppliedEvent;
    
    protected:
        void OnSettingValueChanged(const FGameplayTag& SaveKey, const FString& NewSettingValue);
    
    private:
        // Current settings values (tag-based for performance)
        UPROPERTY()
        TMap<FGameplayTag, FString> CurrentSettings;
    	
    	// Simple UI settings saver
        void SaveToPlayerPrefs();
        void LoadFromPlayerPrefs();
};
