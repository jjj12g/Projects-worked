// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillData.generated.h"

/**
 * 스킬 데이터 정리
 */


UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Instantaneous UMETA(DisplayName = "즉발형"),
    Projectile    UMETA(DisplayName = "투사체형"),
    AOE           UMETA(DisplayName = "범위형"),
    Channeling    UMETA(DisplayName = "채널링형")
};

UENUM(BlueprintType)
enum class ETargetType : uint8
{
    Nontarget     UMETA(DisplayName = "논타겟"),
    AutoTarget    UMETA(DisplayName = "자동타겟"),
    Buff          UMETA(DisplayName = "버프"),
    Installer     UMETA(DisplayName = "설치기")
};

UENUM(BlueprintType)
enum class ESkillShape : uint8
{
	SCS_Sphere    UMETA(DisplayName = "원형"),
    SCS_BeeLine   UMETA(DisplayName = "직선"),
	SCS_Box       UMETA(DisplayName = "사각"),
    SCS_Cone      UMETA(DisplayName = "부채꼴"),
    SCS_Dot       UMETA(DisplayName = "점")
};



UENUM(BlueprintType)
enum class ESkillTargetFilter : uint8
{
    Enemy         UMETA(DisplayName = "적"),
    Ally          UMETA(DisplayName = "아군"),
    Self          UMETA(DisplayName = "자신"),
    Team          UMETA(DisplayName = "모두")
};


USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
    GENERATED_BODY()

    // 스킬 식별자 (ex: "SKL_FIREBALL")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 SkillID;

    // 스킬 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Name;

    // 스킬 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Description;

    // 스킬 타입 (예: 근접, 투사체, 범위, 채널링 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;

    // 타겟 방식
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ETargetType TargetType;

    // 스킬 대상
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillTargetFilter TargetFilter;

    // 충돌 형태 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillShape SkillShape;

    // 반지름, 길이 등
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float ShapeParam1;

    // 너비, 각도 등
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float ShapeParam2;

    // 필요에 따라 추가
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float ShapeParam3;

    // 범위 / 사거리 (Range)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Range;

    // 데미지 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Damage;

    // 버프 / 디버프 수치
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float EffectValue;

    // 소모 마나
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float ManaCost;

    // 쿨타임 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown;

    // 시전 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float CastTime;

    // 스킬이 시전 중에 이동 가능/불가능 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool bCanMoveWhileCasting;

    // 스킬 시전 중 끊김 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool bInterruptible;

    // 스킬 효과
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
    TSoftObjectPtr<UParticleSystem> EffectAsset;

    // 충돌 또는 명중 시 이펙트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
    TSoftObjectPtr<UParticleSystem> ImpactEffectAsset;

    // 사운드 이펙트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
    TSoftObjectPtr<USoundBase> ImpactSound;

    // 스킬 재생용 애니메이션 몽타주 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Animation")
    TSoftObjectPtr<UAnimMontage> AnimationMontage;

    // 애니메이션 재생 후, 콜리전 스폰까지 기다릴 시간(초) 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Timing")
    float SpawnDelay = 0.f;

    // 추적형 투사체 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool bTracking;

};

// 나중 추가 예정
//    // 스킬 지속 시간 (초 단위)
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
//    float LifeSpan;
//};
