// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/InventorySlot.h"





void UInventory::InitializeInventory(const TArray<FItemData>& Items)
{
    if (!InventoryGrid || !InventorySlotClass) return;

    // 기존 슬롯 모두 제거
    InventoryGrid->ClearChildren();

    const int32 NumColumns = 8;
    const int32 NumRows = 5;
    const int32 TotalSlots = NumColumns * NumRows;

    // 모든 슬롯을 생성하면서 고유 인덱스를 할당
    for (int32 Row = 0; Row < NumRows; ++Row)
    {
        for (int32 Col = 0; Col < NumColumns; ++Col)
        {
            int32 SlotIndex = Row * NumColumns + Col;  // 고유 인덱스 계산
            UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
            if (NewSlot)
            {
                // 해당 슬롯에 아이템 데이터 적용: Items 배열에 값이 있으면 적용, 없으면 빈 아이템 데이터
                FItemData SlotItem = (SlotIndex < Items.Num()) ? Items[SlotIndex] : FItemData();
                NewSlot->InitializeSlot(SlotItem, SlotIndex);
                InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Col);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create InventorySlot widget."));
            }
        }
    }
 }






