// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/DragDropOperation.h"
#include "ProjectKLBPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "ProjectKLBCharacter.h"
#include "Inventory/EquipmentSlot.h"

void UInventorySlot::InitializeSlot(const FItemData& InItemData, int32 InSlotIndex)
{
    ItemData = InItemData;
    SlotIndex = InSlotIndex;

    UE_LOG(LogTemp, Log, TEXT("Slot %d initialized with ItemName: %s"), SlotIndex, *ItemData.ItemName);


     // 아이콘
    if (ItemIconImage)
    {
        if (ItemData.ItemIcon)
        {
            ItemIconImage->SetBrushFromTexture(ItemData.ItemIcon);
            ItemIconImage->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIconImage->SetBrushFromTexture(nullptr);
            // 아이콘은 숨기되, 슬롯 위젯 자체는 Visible이어야 함
            ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 이름 텍스트
    if (ItemNameText)
    {
        if (!ItemData.ItemName.IsEmpty())
        {
            ItemNameText->SetText(FText::FromString(ItemData.ItemName));
            ItemNameText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemNameText->SetText(FText::FromString(""));
            ItemNameText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 수량 텍스트
    if (ItemQuantityText)
    {
        if (ItemData.Quantity > 0)
        {
            ItemQuantityText->SetText(FText::AsNumber(ItemData.Quantity));
            ItemQuantityText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemQuantityText->SetText(FText::FromString(""));
            ItemQuantityText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

 

}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    // 아이템 이름이 비어있으면(즉, 아이템이 없는 슬롯이면) 드래그하지 않음
    if (ItemData.ItemName.IsEmpty())
    {
        return;
    }

    UInventoryDragDropOperation* DragOperation = NewObject<UInventoryDragDropOperation>();
    DragOperation->DefaultDragVisual = this; // 시각적으로 현재 슬롯을 사용
    DragOperation->Payload = this; // 'this'는 UInventorySlot* 타입, UObject*와 호환됨
    DragOperation->FromSlotIndex = SlotIndex; // 드래그된 슬롯 인덱스 설정

    OutOperation = DragOperation;

    // 드래그 중 아이템을 투명하게 만듦
    SetVisibility(ESlateVisibility::HitTestInvisible);
    //SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("Drag detected for slot index: %d"), SlotIndex);
}



bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (InOperation && InOperation->Payload)
    {
        UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
        if (DragOp)
        {
            // 먼저, Payload가 장비 슬롯 위젯(UEquipmentSlot)인지 확인합니다.
            UEquipmentSlot* EquipSlot = Cast<UEquipmentSlot>(DragOp->Payload);
            if (EquipSlot)
            {
                AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
                if (!PC)
                {
                    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: Failed to cast to PlayerController."));
                    return false;
                }

                AProjectKLBCharacter* CurrentCharacter = Cast<AProjectKLBCharacter>(PC->GetPawn());
                if (!CurrentCharacter)
                {
                    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: CurrentCharacter is null."));
                    return false;
                }

                // 장비 슬롯에서 드래그한 아이템 데이터(로컬 복사본)
                FItemData DraggedItem = DragOp->DraggedItemData;
                if (DraggedItem.ItemName.IsEmpty())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Dragged equipment slot is already empty."));
                    return false;
                }

                // 현재 드롭 대상 인벤토리 슬롯(this)의 데이터를 확인합니다.
                FItemData CurrentSlotItem = GetItemData();
                int32 TargetIndex = -1;
                if (CurrentSlotItem.ItemName.IsEmpty())
                {
                    // 현재 인벤토리 슬롯이 비어있으면, 바로 이 슬롯의 인덱스를 사용
                    TargetIndex = SlotIndex;
                    UE_LOG(LogTemp, Log, TEXT("Target inventory slot %d is empty; using it."), TargetIndex);
                }
                else
                {
                    // 이미 아이템이 있는 경우, 스왑을 진행합니다.
                    TargetIndex = SlotIndex;  // 타겟 슬롯의 인덱스를 사용 (스왑 처리)
                    UE_LOG(LogTemp, Log, TEXT("Target inventory slot %d is occupied; swapping items."), TargetIndex);
                }

                // 두 경우 모두, 이제 스왑 또는 이동 처리를 진행합니다.
                // 인벤토리 슬롯의 아이템(InventoryItem)을 가져옵니다.
                FItemData InventoryItem = GetItemData();

                if (InventoryItem.ItemName.IsEmpty())
                {
                    // 비어있는 경우: 단순히 DraggedItem을 타겟 슬롯에 저장
                    PC->Inventory[TargetIndex] = DraggedItem;
                    UE_LOG(LogTemp, Log, TEXT("Moved dragged equipment item (%s) to inventory slot %d"),
                        *DraggedItem.ItemName, TargetIndex);
                }
                else
                {
                    // 스왑 처리: 타겟 슬롯에 DraggedItem을 저장하고,
                    // 인벤토리에서 원래 있던 아이템(InventoryItem)을 장비 슬롯으로 이동합니다.
                    PC->Inventory[TargetIndex] = DraggedItem;
                    UE_LOG(LogTemp, Log, TEXT("Swapped: Moved equipment item (%s) to inventory slot %d"),
                        *DraggedItem.ItemName, TargetIndex);

                    // 장비 슬롯에 InventoryItem을 장착 (스왑)
                    CurrentCharacter->EquipItemInSlot(EquipSlot->GetSlotType(), InventoryItem);
                    UE_LOG(LogTemp, Log, TEXT("Swapped: Equipped inventory item (%s) in equipment slot of type %d"),
                        *InventoryItem.ItemName, (int32)EquipSlot->GetSlotType());

                    // 그리고 장비 슬롯 UI를 InventoryItem으로 업데이트
                    EquipSlot->InitializeEquipmentSlot(InventoryItem, EquipSlot->GetSlotIndex());
                }

                // 이제, 장비 슬롯에서 드래그한 아이템을 제거하여 빈 상태로 만듭니다.
                // (장비 → 인벤토리로 이동 후, 해당 장비 슬롯은 빈 상태로 업데이트)
                if (InventoryItem.ItemName.IsEmpty())
                {
                    // 만약 스왑 처리 없이 단순 이동인 경우, 드래그한 아이템이 장비 슬롯의 기존 아이템과 동일하므로
                    // 장비 슬롯을 빈 상태로 설정
                    CurrentCharacter->EquipItemInSlot(EquipSlot->GetSlotType(), FItemData());
                    EquipSlot->InitializeEquipmentSlot(FItemData(), EquipSlot->GetSlotIndex());
                }

                // 인벤토리 UI 업데이트
                PC->OnInventoryChanged.Broadcast(PC->Inventory);
                PC->PCUpdateInventoryUI();
                PC->PCUpdateEquipmentUI();

                // 복원: 양쪽 위젯의 Visibility 복원
                SetVisibility(ESlateVisibility::Visible);
                EquipSlot->SetVisibility(ESlateVisibility::Visible);

                return true;
            }
            else
            {
                // Payload가 인벤토리 슬롯 위젯인 경우, 기존 인벤토리 슬롯 간 교환 로직 처리
                AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
                if (PC)
                {
                    PC->SwapInventoryItems(DragOp->FromSlotIndex, SlotIndex);
                    SetVisibility(ESlateVisibility::Visible);
                    UInventorySlot* DraggedSlot = Cast<UInventorySlot>(DragOp->Payload);
                    if (DraggedSlot)
                    {
                        DraggedSlot->SetVisibility(ESlateVisibility::Visible);
                    }
                    return true;
                }
            }
        }
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

    // 드래그가 취소되었으므로 슬롯의 Visibility를 다시 Visible로 설정
    SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("Drag cancelled for slot index: %d"), SlotIndex);
}
