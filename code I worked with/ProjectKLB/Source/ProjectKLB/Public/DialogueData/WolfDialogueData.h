// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WolfDialogueData.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FWolfDialogueData : public FTableRowBase
{
    GENERATED_BODY()

    // �ּ� ȣ����, �ִ� ȣ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    int32 MinFavorability = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    int32 MaxFavorability = 100;

    // �ش� ȣ���� �������� ����� ���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText DialogueText;
};

