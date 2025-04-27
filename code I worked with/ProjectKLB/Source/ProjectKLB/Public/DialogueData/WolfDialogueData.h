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

    // 최소 호감도, 최대 호감도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    int32 MinFavorability = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    int32 MaxFavorability = 100;

    // 해당 호감도 범위에서 출력할 대사
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText DialogueText;
};

