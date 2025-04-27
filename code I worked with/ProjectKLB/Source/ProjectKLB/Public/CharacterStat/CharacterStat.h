// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStat.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterStat : public FTableRowBase
{
    GENERATED_BODY()

    // 캐릭터 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FName Name;

    // 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health;

    // 공격력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Attack;

    // 방어력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Defense;

    // 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Speed;

    // 행운?


    // 특수 능력 설명 (특성)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FString SpecialAbilityDescription;

    // 호감도 시스템
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Favorability;


};