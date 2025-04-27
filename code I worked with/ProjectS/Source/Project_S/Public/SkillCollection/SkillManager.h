// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillCollection/SkillData.h"
#include "SkillManager.generated.h"

/*
 * 전역 스킬 관리자. JSON 또는 DataTable에서 스킬 데이터를 로드하고,
 * 스킬 ID를 기반으로 알맞은 스킬 충돌 액터를 스폰하며,
 * 필요 시 스킬 업데이트 및 기타 로직을 처리합니다.
 */

 class ASkill2DCollisionActor;

UCLASS()
class PROJECT_S_API USkillManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    // UGameInstanceSubsystem 오버라이드: 초기화, 해제
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /**
     * 스킬 데이터를 기반으로 알맞은 스킬 충돌 액터를 스폰합니다.
     * @param SkillData - 스킬 데이터
     * @param SpawnTransform - 스폰 위치 및 회전
     * @return 생성된 스킬 충돌 액터. 실패 시 nullptr.
     */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    ASkill2DCollisionActor* SpawnSkillCollisionActor(const FSkillData& SkillData, const FTransform& SpawnTransform);

    /**
     * JSON 파일 경로를 받아 스킬 데이터를 로드하여 CachedSkillData에 저장합니다.
     * 이후 필요 시 스폰할 때 사용할 수 있습니다.
     * @param FilePath - JSON 파일 경로
     */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void LoadSkillDataFromJson(const FString& FilePath);

    // 스킬 데이터를 캐싱하는 TMap: Key는 SkillID
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
    TMap<int32, FSkillData> CachedSkillData;

};
