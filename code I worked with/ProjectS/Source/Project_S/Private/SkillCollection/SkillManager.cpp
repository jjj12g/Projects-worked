// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollection/SkillManager.h"
#include "SkillCollection/Skill2DSphereCollisionActor.h"
#include "SkillCollection/Skill2DBoxCollisionActor.h"
#include "SkillCollection/Skill2DConeCollisionActor.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"

void USkillManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // �ʿ� �� JSON �����͸� �ҷ����ų�, DataTable���� �ʱ� ��ų �����͸� �ε�
      // DataTable ���� ��� (������Ʈ ��ο� �°� ����)
    FString DataTablePath = TEXT("DataTable'/Game/Project/SkillData/DT_SkillData.DT_SkillData'");
    UDataTable* SkillDataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);

    if (SkillDataTable)
    {
        static const FString ContextString(TEXT("SkillDataContext"));
        // DataTable�� ��� ���� �����ɴϴ�.
        TArray<FSkillData*> AllSkillRows;
        SkillDataTable->GetAllRows<FSkillData>(ContextString, AllSkillRows);

        // �� ���� CachedSkillData�� �߰��մϴ�.
        for (FSkillData* SkillRow : AllSkillRows)
        {
            if (SkillRow)
            {
                CachedSkillData.Add(SkillRow->SkillID, *SkillRow);
                UE_LOG(LogTemp, Log, TEXT("SkillManager::Initialize - Loaded Skill ID: %d"), SkillRow->SkillID);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SkillManager::Initialize - Failed to load Skill Data Table at %s"), *DataTablePath);
    }
}

void USkillManager::Deinitialize()
{
    CachedSkillData.Empty();
    Super::Deinitialize();
}

ASkill2DCollisionActor* USkillManager::SpawnSkillCollisionActor(const FSkillData& SkillData, const FTransform& SpawnTransform)
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("SkillManager::SpawnSkillCollisionActor: GetWorld() is null."));
        return nullptr;
    }

    ASkill2DCollisionActor* SpawnedActor = nullptr;


    switch (SkillData.SkillShape)
    {
    case ESkillShape::SCS_Sphere:
        SpawnedActor = GetWorld()->SpawnActorDeferred<ASkill2DSphereCollisionActor>(ASkill2DSphereCollisionActor::StaticClass(), SpawnTransform);
        break;
    //case ESkillShape::SCS_BeeLine:
        //SpawnedActor = GetWorld()->SpawnActorDeferred<ASkill2DBoxCollisionActor>(ASkill2DBoxCollisionActor::StaticClass(), SpawnTransform);
        //break;
    case ESkillShape::SCS_Box:
        SpawnedActor = GetWorld()->SpawnActorDeferred<ASkill2DBoxCollisionActor>(ASkill2DBoxCollisionActor::StaticClass(), SpawnTransform);
        break;
    case ESkillShape::SCS_Cone:
        SpawnedActor = GetWorld()->SpawnActorDeferred<ASkill2DConeCollisionActor>(ASkill2DConeCollisionActor::StaticClass(), SpawnTransform);
        break;
    //case ESkillShape::SCS_Dot:
        //SpawnedActor = GetWorld()->SpawnActorDeferred<ASkill2DBoxCollisionActor>(ASkill2DBoxCollisionActor::StaticClass(), SpawnTransform);
        //break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("SkillManager::SpawnSkillCollisionActor: Unknown CollisionShape."));
        break;
    }
    if (SpawnedActor)
    {
        SpawnedActor->SkillData = SkillData;
        UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnSkillCollisionActor: Failed to spawn actor."));
    }


    return SpawnedActor;
}

void USkillManager::LoadSkillDataFromJson(const FString& FilePath)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("LoadSkillDataFromJson: Failed to load file: %s"), *FilePath);
        return;
    }

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("LoadSkillDataFromJson: Failed to parse JSON."));
        return;
    }

    // ���� ���, JSON ������ �迭 �������� ��ų �����͸� �����ϰ� �ִٰ� ����
    const TArray<TSharedPtr<FJsonValue>>* SkillArray;
    if (RootObject->TryGetArrayField(TEXT("Skills"), SkillArray))
    {
        for (const TSharedPtr<FJsonValue>& Value : *SkillArray)
        {
            FSkillData NewSkill;
            if (FJsonObjectConverter::JsonObjectToUStruct(Value->AsObject().ToSharedRef(), &NewSkill, 0, 0))
            {
                CachedSkillData.Add(NewSkill.SkillID, NewSkill);
                UE_LOG(LogTemp, Log, TEXT("Loaded Skill ID: %d"), NewSkill.SkillID);
            }
        }
    }
}