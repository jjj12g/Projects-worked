// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/ItemData.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
    // �巡���� ������ �����͸� �����ؼ� ������ ����
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    FItemData DraggedItemData;

    // �巡�׵� ������ �ε���
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    int32 FromSlotIndex;

    // ��ӵ� ������ �ε���
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    int32 ToSlotIndex;
	
};
