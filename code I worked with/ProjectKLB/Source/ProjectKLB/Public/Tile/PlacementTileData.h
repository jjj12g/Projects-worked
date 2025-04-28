// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlacementTileData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlacementTileData
{
    GENERATED_BODY()

public:
    // �� Ÿ�Ͽ� ĳ���Ͱ� ��ġ�Ǿ����� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied = false;

    // ��ġ�� ĳ���� Ŭ������ ���� 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AProjectKLBCharacter> OccupiedCharacterClass;

    // �ش� Ÿ���� �׸��� ��ǥ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Q;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 R;
};