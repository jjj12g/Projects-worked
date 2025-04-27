// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillCollection/SkillData.h"
#include "SkillManager.generated.h"

/*
 * ���� ��ų ������. JSON �Ǵ� DataTable���� ��ų �����͸� �ε��ϰ�,
 * ��ų ID�� ������� �˸��� ��ų �浹 ���͸� �����ϸ�,
 * �ʿ� �� ��ų ������Ʈ �� ��Ÿ ������ ó���մϴ�.
 */

 class ASkill2DCollisionActor;

UCLASS()
class PROJECT_S_API USkillManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    // UGameInstanceSubsystem �������̵�: �ʱ�ȭ, ����
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /**
     * ��ų �����͸� ������� �˸��� ��ų �浹 ���͸� �����մϴ�.
     * @param SkillData - ��ų ������
     * @param SpawnTransform - ���� ��ġ �� ȸ��
     * @return ������ ��ų �浹 ����. ���� �� nullptr.
     */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    ASkill2DCollisionActor* SpawnSkillCollisionActor(const FSkillData& SkillData, const FTransform& SpawnTransform);

    /**
     * JSON ���� ��θ� �޾� ��ų �����͸� �ε��Ͽ� CachedSkillData�� �����մϴ�.
     * ���� �ʿ� �� ������ �� ����� �� �ֽ��ϴ�.
     * @param FilePath - JSON ���� ���
     */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void LoadSkillDataFromJson(const FString& FilePath);

    // ��ų �����͸� ĳ���ϴ� TMap: Key�� SkillID
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
    TMap<int32, FSkillData> CachedSkillData;

};
