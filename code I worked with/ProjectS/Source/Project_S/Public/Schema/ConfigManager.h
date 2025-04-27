
#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#include "ConfigManager.generated.h"

USTRUCT(BlueprintType)
struct FServerConfig
{
    GENERATED_BODY()

    UPROPERTY()
    FString DataPath;
};

UCLASS(BlueprintType)
class PROJECT_S_API UConfigManager : public UObject
{
    GENERATED_BODY()
public:
    static FServerConfig Config;
    UFUNCTION(BlueprintCallable)
    static void LoadConfig();
};
