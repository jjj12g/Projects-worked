// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EquipmentSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "ProjectKLBPlayerController.h"
#include "ProjectKLBCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/InventorySlot.h"




void UEquipmentSlot::InitializeEquipmentSlot(const FItemData& InEquipmentData, int32 InSlotIndex)
{
    // �⺻ �κ��丮 ������ �ʱ�ȭ ���� ȣ��
    InitializeSlot(InEquipmentData, InSlotIndex);

    EquipmentData = InEquipmentData;
    SlotIndex = InSlotIndex;
    UE_LOG(LogTemp, Log, TEXT("InitializeEquipmentSlot: Slot %d initialized with item: %s"), SlotIndex, *EquipmentData.ItemName);


    if (ItemIconImage)
    {
        if (!EquipmentData.ItemName.IsEmpty() && EquipmentData.ItemIcon)
        {
            ItemIconImage->SetBrushFromTexture(EquipmentData.ItemIcon);
            ItemIconImage->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIconImage->SetBrushFromTexture(nullptr);
            ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (ItemNameText)
    {
        if (!EquipmentData.ItemName.IsEmpty())
        {
            ItemNameText->SetText(FText::FromString(EquipmentData.ItemName));
            ItemNameText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemNameText->SetText(FText::FromString(""));
            ItemNameText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

 /*   EquipmentData = InEquipmentData;
    SlotIndex = InSlotIndex;*/


    // ��) SlotType = EEquipmentType::Weapon;  // ������ Ÿ���� Weapon���� ���� (�ʿ��)
    // ���� �κ��丮 �������� ���� �ε����� �����Ѵٸ�, �� ���� SlotIndex�� �����մϴ�.
    // SlotIndex = ���޹��� �ε���;  (���� ���, �Ķ���ͷ� �߰��ϰų� ������ ����)

   //  UI ������Ʈ: ��� ������, �̸� ���� ����
   // if (EquipmentIcon)
   // {
   //     if (InEquipmentData.ItemIcon)
   //     {
   //         EquipmentIcon->SetBrushFromTexture(InEquipmentData.ItemIcon);
   //         EquipmentIcon->SetVisibility(ESlateVisibility::Visible);
   //     }
   //     else
   //     {
   //         EquipmentIcon->SetBrushFromTexture(nullptr);
   //         EquipmentIcon->SetVisibility(ESlateVisibility::Hidden);
   //     }
   // }
   // if (EquipmentNameText)
   // {
   //     if (!InEquipmentData.ItemName.IsEmpty())
   //     {
   //         EquipmentNameText->SetText(FText::FromString(InEquipmentData.ItemName));
   //         EquipmentNameText->SetVisibility(ESlateVisibility::Visible);
   //     }
   //     else
   //     {
   //         EquipmentNameText->SetText(FText::FromString(""));
   //         EquipmentNameText->SetVisibility(ESlateVisibility::Hidden);
   //     }
   // }*/

}

FItemData UEquipmentSlot::GetCurrentEquippedItem() const
{
    return EquipmentData;
}



FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // ���콺 ���� ��ư Ŭ�� �� �巡�� ����
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    // �巡�� ���� �� ���� ������ ������ �����͸� �����ɴϴ�.
    FItemData CurrentItem = GetCurrentEquippedItem();
    if (CurrentItem.ItemName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Dragged equipment slot is already empty at drag start."));
        return; // �������� ������ �巡�׸� �������� ����.
    }

    // �巡�� ���� ������ ������ ������ �α׷� ����մϴ�.
    UE_LOG(LogTemp, Log, TEXT("Drag start - Slot %d, Dragged Item: %s, Quantity: %d"),
        GetSlotIndex(), *CurrentItem.ItemName, CurrentItem.Quantity);

    // Ŀ���� �巡�� ���۷��̼� ����
    UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>(this);
    DragOp->DefaultDragVisual = this;
    DragOp->Payload = this;
    DragOp->FromSlotIndex = GetSlotIndex();
    // �巡�� ���� ������ ������ �����͸� �����ؼ� ����
    DragOp->DraggedItemData = CurrentItem;

    OutOperation = DragOp;

    // �巡�� �߿��� ���� ������ Visibility�� �����ϰ� ����(Ŭ�� ������ �ǵ���)
    SetVisibility(ESlateVisibility::HitTestInvisible);

    UE_LOG(LogTemp, Log, TEXT("EquipmentSlot drag detected for slot index: %d, saved DraggedItemData: %s"),
        GetSlotIndex(), *DragOp->DraggedItemData.ItemName);
}

void UEquipmentSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    // �巡�װ� ��ҵǾ����� Visibility�� ����
    SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("EquipmentSlot drag cancelled for slot index: %d"), SlotIndex);

}


bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (InOperation && InOperation->Payload)
    {
        UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
        if (DragOp)
        {
            // ���⼭�� �κ��丮���� �巡���� ��츦 ó���մϴ�.
            UInventorySlot* InvSlot = Cast<UInventorySlot>(DragOp->Payload);
            if (InvSlot)
            {
                // �κ��丮 ���Կ��� �巡���� ���, NewItem �����͸� ���÷� �����մϴ�.
                FItemData NewItem = InvSlot->GetItemData();
                if (NewItem.ItemName.IsEmpty())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Dropped item from inventory is empty."));
                    return false;
                }

                // �÷��̾� ��Ʈ�ѷ� �� ���� ĳ���� ��������
                AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
                if (PC)
                {
                    AProjectKLBCharacter* CurrentCharacter = Cast<AProjectKLBCharacter>(PC->GetPawn());
                    if (CurrentCharacter)
                    {
                        // ���� ��� ���Կ� �������� �ִٸ�, �κ��丮�� �� �������� �̵�
                        FItemData ExistingItem = GetCurrentEquippedItem();
                        if (!ExistingItem.ItemName.IsEmpty())
                        {
                            int32 EmptyIndex = PC->FindEmptyInventorySlot();
                            if (EmptyIndex != -1)
                            {
                                PC->Inventory[EmptyIndex] = ExistingItem;
                                UE_LOG(LogTemp, Log, TEXT("Moved existing equipped item (%s) to inventory slot %d"),
                                    *ExistingItem.ItemName, EmptyIndex);
                            }
                            else
                            {
                                UE_LOG(LogTemp, Warning, TEXT("No empty inventory slot available for existing equipped item."));
                                return false;
                            }
                        }

                        // �� �������� ��� ���Կ� ����
                        CurrentCharacter->EquipItemInSlot(GetSlotType(), NewItem);
                        UE_LOG(LogTemp, Log, TEXT("Equipped item %s in equipment slot of type %d"),
                            *NewItem.ItemName, (int32)GetSlotType());

                        // �κ��丮���� �ش� ������ ����
                        PC->RemoveInventoryItem(DragOp->FromSlotIndex);

                        // ��� ���� UI ������Ʈ: �� ���������� ������Ʈ
                        InitializeEquipmentSlot(NewItem, GetSlotIndex());

                        // �κ��丮�� ��� UI ������Ʈ
                        PC->OnInventoryChanged.Broadcast(PC->Inventory);
                        PC->PCUpdateInventoryUI();
                        PC->PCUpdateEquipmentUI();

                        // ��� �Ϸ� �� Visibility ����
                        SetVisibility(ESlateVisibility::Visible);
                        return true;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: CurrentCharacter is null."));
                        return false;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: Failed to cast to PlayerController."));
                }
            }
            // ���� Payload�� �κ��丮 ���� ������ �ƴ� ��� (��: �κ��丮 ���� �� ��ȯ)
            else
            {
                // �κ��丮 ���� �� ��ȯ ���� ó��
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







