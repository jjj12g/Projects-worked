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


     // ������
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
            // �������� �����, ���� ���� ��ü�� Visible�̾�� ��
            ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // �̸� �ؽ�Ʈ
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

    // ���� �ؽ�Ʈ
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

    // ������ �̸��� ���������(��, �������� ���� �����̸�) �巡������ ����
    if (ItemData.ItemName.IsEmpty())
    {
        return;
    }

    UInventoryDragDropOperation* DragOperation = NewObject<UInventoryDragDropOperation>();
    DragOperation->DefaultDragVisual = this; // �ð������� ���� ������ ���
    DragOperation->Payload = this; // 'this'�� UInventorySlot* Ÿ��, UObject*�� ȣȯ��
    DragOperation->FromSlotIndex = SlotIndex; // �巡�׵� ���� �ε��� ����

    OutOperation = DragOperation;

    // �巡�� �� �������� �����ϰ� ����
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
            // ����, Payload�� ��� ���� ����(UEquipmentSlot)���� Ȯ���մϴ�.
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

                // ��� ���Կ��� �巡���� ������ ������(���� ���纻)
                FItemData DraggedItem = DragOp->DraggedItemData;
                if (DraggedItem.ItemName.IsEmpty())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Dragged equipment slot is already empty."));
                    return false;
                }

                // ���� ��� ��� �κ��丮 ����(this)�� �����͸� Ȯ���մϴ�.
                FItemData CurrentSlotItem = GetItemData();
                int32 TargetIndex = -1;
                if (CurrentSlotItem.ItemName.IsEmpty())
                {
                    // ���� �κ��丮 ������ ���������, �ٷ� �� ������ �ε����� ���
                    TargetIndex = SlotIndex;
                    UE_LOG(LogTemp, Log, TEXT("Target inventory slot %d is empty; using it."), TargetIndex);
                }
                else
                {
                    // �̹� �������� �ִ� ���, ������ �����մϴ�.
                    TargetIndex = SlotIndex;  // Ÿ�� ������ �ε����� ��� (���� ó��)
                    UE_LOG(LogTemp, Log, TEXT("Target inventory slot %d is occupied; swapping items."), TargetIndex);
                }

                // �� ��� ���, ���� ���� �Ǵ� �̵� ó���� �����մϴ�.
                // �κ��丮 ������ ������(InventoryItem)�� �����ɴϴ�.
                FItemData InventoryItem = GetItemData();

                if (InventoryItem.ItemName.IsEmpty())
                {
                    // ����ִ� ���: �ܼ��� DraggedItem�� Ÿ�� ���Կ� ����
                    PC->Inventory[TargetIndex] = DraggedItem;
                    UE_LOG(LogTemp, Log, TEXT("Moved dragged equipment item (%s) to inventory slot %d"),
                        *DraggedItem.ItemName, TargetIndex);
                }
                else
                {
                    // ���� ó��: Ÿ�� ���Կ� DraggedItem�� �����ϰ�,
                    // �κ��丮���� ���� �ִ� ������(InventoryItem)�� ��� �������� �̵��մϴ�.
                    PC->Inventory[TargetIndex] = DraggedItem;
                    UE_LOG(LogTemp, Log, TEXT("Swapped: Moved equipment item (%s) to inventory slot %d"),
                        *DraggedItem.ItemName, TargetIndex);

                    // ��� ���Կ� InventoryItem�� ���� (����)
                    CurrentCharacter->EquipItemInSlot(EquipSlot->GetSlotType(), InventoryItem);
                    UE_LOG(LogTemp, Log, TEXT("Swapped: Equipped inventory item (%s) in equipment slot of type %d"),
                        *InventoryItem.ItemName, (int32)EquipSlot->GetSlotType());

                    // �׸��� ��� ���� UI�� InventoryItem���� ������Ʈ
                    EquipSlot->InitializeEquipmentSlot(InventoryItem, EquipSlot->GetSlotIndex());
                }

                // ����, ��� ���Կ��� �巡���� �������� �����Ͽ� �� ���·� ����ϴ�.
                // (��� �� �κ��丮�� �̵� ��, �ش� ��� ������ �� ���·� ������Ʈ)
                if (InventoryItem.ItemName.IsEmpty())
                {
                    // ���� ���� ó�� ���� �ܼ� �̵��� ���, �巡���� �������� ��� ������ ���� �����۰� �����ϹǷ�
                    // ��� ������ �� ���·� ����
                    CurrentCharacter->EquipItemInSlot(EquipSlot->GetSlotType(), FItemData());
                    EquipSlot->InitializeEquipmentSlot(FItemData(), EquipSlot->GetSlotIndex());
                }

                // �κ��丮 UI ������Ʈ
                PC->OnInventoryChanged.Broadcast(PC->Inventory);
                PC->PCUpdateInventoryUI();
                PC->PCUpdateEquipmentUI();

                // ����: ���� ������ Visibility ����
                SetVisibility(ESlateVisibility::Visible);
                EquipSlot->SetVisibility(ESlateVisibility::Visible);

                return true;
            }
            else
            {
                // Payload�� �κ��丮 ���� ������ ���, ���� �κ��丮 ���� �� ��ȯ ���� ó��
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

    // �巡�װ� ��ҵǾ����Ƿ� ������ Visibility�� �ٽ� Visible�� ����
    SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("Drag cancelled for slot index: %d"), SlotIndex);
}
