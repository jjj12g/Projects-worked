// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventorySlot.h"
#include "Inventory/ItemData.h"
#include "EquipmentSlot.generated.h"

/**
 * 
 */
 


UCLASS()
class PROJECTKLB_API UEquipmentSlot : public UInventorySlot
{
	GENERATED_BODY()
	

public:
    // 해당 슬롯이 나타내는 장비 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    EEquipmentType SlotType;

    // 장비 슬롯 초기화 함수 
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void InitializeEquipmentSlot(const FItemData& InEquipmentData, int32 InSlotIndex);

    // 현재 장착된 아이템 데이터를 반환 
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    FItemData GetCurrentEquippedItem() const;

    // 슬롯의 장비 타입 반환
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    EEquipmentType GetSlotType() const { return SlotType; }

 
    int32 GetSlotIndex() const { return SlotIndex; }

protected:

    int32 SlotIndex;

    // 슬롯에 장착된 아이템 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    FItemData EquipmentData;

    // 드래그 앤 드롭 관련 함수들 오버라이드
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드래그 앤 드롭 이벤트 처리
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
