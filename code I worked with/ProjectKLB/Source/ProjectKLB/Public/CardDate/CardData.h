// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CardData.generated.h"

class UDataTable;

// DataTable(���̽� ����Ʈ)
USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
	GENERATED_BODY()

    // ī�� �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString CardName;

    // ī�� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString CardDescription;

    // ī�� �׸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* CardIcon;

    // ī�帶�� ����� �� �ִ� ĳ���Ͱ� ����������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString AllowedCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MoveType; // �̵� ���� (e.g., "HexRange", "Knight", etc.)

};


