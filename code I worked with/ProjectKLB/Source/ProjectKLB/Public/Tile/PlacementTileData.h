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
    // 이 타일에 캐릭터가 배치되었는지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied = false;

    // 배치된 캐릭터 클래스를 저장 (AProjectKLBCharacter 파생 클래스)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AProjectKLBCharacter> OccupiedCharacterClass;

    // 해당 타일의 그리드 좌표(혹은 인덱스)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Q;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 R;
};