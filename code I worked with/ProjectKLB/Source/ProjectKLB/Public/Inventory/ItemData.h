// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

/**
 * 
 */

 //Row Name: �� ���� Row Name�� �����ؾ� ��. �ߺ��� ��� ���� �ε� �� �浹

 // ��� ������ ��Ÿ���� Enum
UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
    Weapon    UMETA(DisplayName = "Weapon"),
    Chest     UMETA(DisplayName = "Chest"),
    Legs      UMETA(DisplayName = "Legs"),
    Helmet    UMETA(DisplayName = "Helmet"),

    // ��Ÿ ������ ����(��: Consumable)�� �߰��� �� ����
    Consumable UMETA(DisplayName = "Consumable")
};

// ������ ������ ����ü
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // �⺻ ������
    FItemData()
        : ItemName(TEXT("")), ItemDescription(TEXT("")), ItemIcon(nullptr),
        Quantity(0), MaxQuantity(100), Attack(0.f), Defense(0.f),
        Health(0.f), Speed(0.f), EquipmentType(EEquipmentType::Consumable)
    {}


    // ������ �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDescription;

    // ������ ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemIcon;

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Quantity;

    // �ִ� ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxQuantity;

    // ���ݷ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Attack;

    // ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense;

    // ü��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health;

    // ���ǵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Speed;

    // ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EEquipmentType EquipmentType;

};