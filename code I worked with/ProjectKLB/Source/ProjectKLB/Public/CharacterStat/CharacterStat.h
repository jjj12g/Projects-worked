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

    // ĳ���� �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FName Name;

    // ü��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health;

    // ���ݷ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Attack;

    // ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Defense;

    // �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Speed;

    // ���?


    // Ư�� �ɷ� ���� (Ư��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FString SpecialAbilityDescription;

    // ȣ���� �ý���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Favorability;


};