// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemData.h"
#include "InventorySlot.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;

UCLASS()
class PROJECTKLB_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
    // 슬롯 초기화 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    virtual void InitializeSlot(const FItemData& InItemData, int32 InSlotIndex);

    // 현재 슬롯에 저장된 아이템 반환
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    virtual FItemData GetItemData() const { return ItemData; }


    // SlotIndex 접근 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetSlotIndex() const { return SlotIndex; }

    // 슬롯 간 아이템 교환을 위한 함수 
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetItemData(const FItemData& NewItemData) { ItemData = NewItemData; }



protected:
    // 슬롯의 인덱스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 SlotIndex;

    // 슬롯의 아이템 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    FItemData ItemData;
    
    // 슬롯에 표시할 아이콘 이미지
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIconImage;

    // 슬롯에 표시할 아이템 이름
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    // 슬롯에 표시할 아이템 수량, 나중에 호감도 아이템?
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemQuantityText;


    // 드래그 이벤트 함수
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드래그 동작 시작
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 드래그 실패 시 돌려주기
    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
