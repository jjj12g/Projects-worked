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
    // 해당 슬롯이 나타내는 장비 타입 (예: 무기, 상의 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    EEquipmentType SlotType;

    // 장비 슬롯 초기화 함수 (장비 데이터와 함께 슬롯 타입 및 인덱스를 설정)
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void InitializeEquipmentSlot(const FItemData& InEquipmentData, int32 InSlotIndex);

    // 현재 장착된 아이템 데이터를 반환 (장비 슬롯 전용)
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    FItemData GetCurrentEquippedItem() const;

    // 슬롯의 장비 타입 반환
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    EEquipmentType GetSlotType() const { return SlotType; }

    // 슬롯 인덱스 반환
    //UFUNCTION(BlueprintCallable, Category = "Equipment")
    int32 GetSlotIndex() const { return SlotIndex; }

protected:
    // 슬롯의 인덱스 (몇 번째 슬롯인지 표시)
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    int32 SlotIndex;

    // 슬롯에 장착된 아이템 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    FItemData EquipmentData;

    // //장비 아이콘 이미지 등 UI 요소를 BindWidget으로 연결
    //UPROPERTY(meta = (BindWidget))
    //class UImage* EquipmentIcon;

    //UPROPERTY(meta = (BindWidget))
    //class UTextBlock* EquipmentNameText;

    // 드래그 앤 드롭 관련 함수들 오버라이드
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드래그 앤 드롭 이벤트 처리
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
