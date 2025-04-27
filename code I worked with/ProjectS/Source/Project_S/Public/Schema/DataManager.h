
#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "ConfigManager.h"

#include "DataManager.generated.h"

UINTERFACE(Blueprintable)
class PROJECT_S_API ULoader : public UInterface
{
    GENERATED_BODY()
};

class PROJECT_S_API ILoader
{
    GENERATED_BODY()
public:
    virtual TMap<int32, UObject*> MakeDict() = 0;
};

UCLASS(BlueprintType)
class PROJECT_S_API UDataManager : public UObject
{
    GENERATED_BODY()
    
private:
        static TMap<int32, class UEnemy*> _EnemyDict;

    
public:

    UFUNCTION(BlueprintCallable)
    static void LoadData();

    template <typename Loader>
    static TSharedRef<Loader, ESPMode::ThreadSafe> LoadJson(const FString& Path)
    {
        FString FilePath = FPaths::Combine(UConfigManager::Config.DataPath, Path + TEXT(".json"));
        FString FileContent;

        if (FFileHelper::LoadFileToString(FileContent, *FilePath))
        {
            auto loader = Loader();
            if (FJsonObjectConverter::JsonObjectStringToUStruct(FileContent, &loader, 0, 0))
            {
            auto LoadedData = MakeShared<Loader>(loader);
                return LoadedData;
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("Error: Could not load file at path: %s"), *FilePath);
        return MakeShared<Loader>();
    }
};