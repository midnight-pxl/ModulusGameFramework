// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "Engine/Blueprint.h"
#include "K2Node_CallFunction.h"
#include "GameplayTagContainer.h"
#include "MCore_K2_SimpleBroadcast.generated.h"

struct FMCore_EventData;
enum class EMCore_EventScope : uint8;

/**
 * Custom Blueprint node to replace: Get Subsystem > Create Event > Set Parameters > Broadcast > Handle Result
 * Creates single "Broadcast Event" node with Success/Failed outputs
 */
UCLASS()
class MODULUSCORE_API UMCore_K2_SimpleBroadcast : public UK2Node
{
	GENERATED_BODY()

public:
	UMCore_K2_SimpleBroadcast();

	virtual void AllocateDefaultPins() override;
    virtual FText GetTooltipText() const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
    virtual FText GetMenuCategory() const override;
    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    virtual bool IsNodePure() const override { return false; }

protected:
    // Pin names
    static const FName ExecPinName;
    static const FName WorldContextPinName;
    static const FName EventTagPinName;
    static const FName EventScopePinName;
    static const FName ParameterPinName;
    static const FName SuccessPinName;
    static const FName FailedPinName;

    // Pin accessors
    UEdGraphPin* GetInputExecPin() const;
    UEdGraphPin* GetInputWorldContextPin() const;
    UEdGraphPin* GetInputEventTagPin() const;
    UEdGraphPin* GetInputEventScopePin() const;
    UEdGraphPin* GetInputParameterPin() const;
    UEdGraphPin* GetInputSuccessPin() const;
    UEdGraphPin* GetInputFailedPin() const;

private:
    void CreateBroadcastImplementation(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph);
    
    // Helper functions
    UEdGraphPin* CreateInputPin(const FName& PinName, const FEdGraphPinType& PinType, const FString& DefaultValue = TEXT(""));
    UEdGraphPin* CreateOutputPin(const FName& PinName, const FEdGraphPinType& PinType);
    
    static FEdGraphPinType GetExecPinType();
    static FEdGraphPinType GetObjectPinType(UClass* Class = nullptr);
    static FEdGraphPinType GetStructPinType(UScriptStruct* Struct);
    static FEdGraphPinType GetEnumPinType(UEnum* Enum);
    static FEdGraphPinType GetStringPinType();
};

/**
 * Backend function for Broadcast Event
 */

UCLASS()
class MODULUSCORE_API UK2Node_MCoreSimpleBroadcast_Library : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Optimized broadcast function used by custom node.
     * Combines validation + broadcast in single C++ call.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Events|Internal", 
        meta = (BlueprintInternalUseOnly = "true"))
    static bool OptimizedBroadcastEvent(
        UObject* WorldContext,
        const FGameplayTag& EventTag,
        EMCore_EventScope EventScope,
        const FString& Parameter
    );

    /**
     * Quick validation function.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Events|Internal",
        meta = (BlueprintInternalUseOnly = "true"))
    static bool ValidateEventBroadcast(UObject* WorldContext, const FGameplayTag& EventTag);

private:
    static class UMCore_SimpleEventSubsystem* GetValidatedSubsystem(UObject* WorldContext);
};
