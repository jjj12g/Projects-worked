// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

/**
 * 
 */

 //Row Name: 각 행의 Row Name은 고유해야 함. 중복될 경우 에셋 로드 시 충돌

 // 장비 종류를 나타내는 Enum
UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
    Weapon    UMETA(DisplayName = "Weapon"),
    Chest     UMETA(DisplayName = "Chest"),
    Legs      UMETA(DisplayName = "Legs"),
    Helmet    UMETA(DisplayName = "Helmet"),

    // 기타 아이템 유형(예: Consumable)도 추가할 수 있음
    Consumable UMETA(DisplayName = "Consumable")
};

// 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 기본 생성자
    FItemData()
        : ItemName(TEXT("")), ItemDescription(TEXT("")), ItemIcon(nullptr),
        Quantity(0), MaxQuantity(100), Attack(0.f), Defense(0.f),
        Health(0.f), Speed(0.f), EquipmentType(EEquipmentType::Consumable)
    {}


    // 아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    // 아이템 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDescription;

    // 아이템 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemIcon;

    // 아이템 갯수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Quantity;

    // 최대 아이템 수량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxQuantity;

    // 공격력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Attack;

    // 방어력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense;

    // 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health;

    // 스피드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Speed;

    // 장비 종류
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EEquipmentType EquipmentType;

};