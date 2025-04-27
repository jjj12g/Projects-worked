// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CardData.generated.h"

class UDataTable;

// DataTable(베이스 포인트)
USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
	GENERATED_BODY()

    // 카드 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString CardName;

    // 카드 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString CardDescription;

    // 카드 그림
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* CardIcon;

    // 카드마다 사용할 수 있는 캐릭터가 정해져있음
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString AllowedCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MoveType; // 이동 유형 (e.g., "HexRange", "Knight", etc.)

};


