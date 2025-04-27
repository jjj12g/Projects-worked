// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/InventorySlot.h"





void UInventory::InitializeInventory(const TArray<FItemData>& Items)
{
    if (!InventoryGrid || !InventorySlotClass) return;

    // ���� ���� ��� ����
    InventoryGrid->ClearChildren();

    const int32 NumColumns = 8;
    const int32 NumRows = 5;
    const int32 TotalSlots = NumColumns * NumRows;

    // ��� ������ �����ϸ鼭 ���� �ε����� �Ҵ�
    for (int32 Row = 0; Row < NumRows; ++Row)
    {
        for (int32 Col = 0; Col < NumColumns; ++Col)
        {
            int32 SlotIndex = Row * NumColumns + Col;  // ���� �ε��� ���
            UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
            if (NewSlot)
            {
                // �ش� ���Կ� ������ ������ ����: Items �迭�� ���� ������ ����, ������ �� ������ ������
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






