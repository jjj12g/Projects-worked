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
    // 드래그한 아이템 데이터를 복사해서 저장할 변수
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    FItemData DraggedItemData;

    // 드래그된 슬롯의 인덱스
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    int32 FromSlotIndex;

    // 드롭된 슬롯의 인덱스
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    int32 ToSlotIndex;
	
};
