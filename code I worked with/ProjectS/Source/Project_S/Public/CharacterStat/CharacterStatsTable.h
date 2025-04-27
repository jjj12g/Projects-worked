// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStatsTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterStats : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Mana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Defense;

    

};