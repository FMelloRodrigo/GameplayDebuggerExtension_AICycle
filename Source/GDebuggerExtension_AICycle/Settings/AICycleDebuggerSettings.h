//Rodrigo Mello 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameFramework/Pawn.h"
#include "AICycleDebuggerSettings.generated.h"

UCLASS(config = GDEBUGGEREXTENSION_AICYCLE, defaultconfig, meta = (DisplayName = "Gameplay Debugger AI Cycle"))
class GDEBUGGEREXTENSION_AICYCLE_API UAICycleDebuggerSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UAICycleDebuggerSettings();

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", meta = (UIMin = 100.0, ClampMin = 100.0))
    float SearchRadius;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    TArray<TSubclassOf<APawn>> AIFilterClasses;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    bool IncludePlayer;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    bool OrganizeByDistance;
};