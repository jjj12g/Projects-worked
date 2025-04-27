
#include "Schema/ConfigManager.h"

FServerConfig UConfigManager::Config;
void UConfigManager::LoadConfig()
{
    FString JsonString;
    FString FilePath = FPaths::ProjectDir() / TEXT("config.json");

    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &Config, 0, 0);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Error : File Can not found, path : %s"), *FilePath);
    }
}
