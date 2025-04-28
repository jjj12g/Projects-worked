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
    // 기본 인벤토리 슬롯의 초기화 로직 호출
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
}

FItemData UEquipmentSlot::GetCurrentEquippedItem() const
{
    return EquipmentData;
}



FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 마우스 왼쪽 버튼 클릭 시 드래그 시작
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    // 드래그 시작 시 현재 장착된 아이템 데이터를 가져옵니다.
    FItemData CurrentItem = GetCurrentEquippedItem();
    if (CurrentItem.ItemName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Dragged equipment slot is already empty at drag start."));
        return; // 아이템이 없으면 드래그를 시작하지 않음.
    }

    // 드래그 시작 시점의 아이템 정보를 로그로 출력
    UE_LOG(LogTemp, Log, TEXT("Drag start - Slot %d, Dragged Item: %s, Quantity: %d"),
        GetSlotIndex(), *CurrentItem.ItemName, CurrentItem.Quantity);

    // 커스텀 드래그 오퍼레이션 생성
    UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>(this);
    DragOp->DefaultDragVisual = this;
    DragOp->Payload = this;
    DragOp->FromSlotIndex = GetSlotIndex();
    // 드래그 시작 시점의 아이템 데이터를 복사해서 저장
    DragOp->DraggedItemData = CurrentItem;

    OutOperation = DragOp;

    // 드래그 중에는 슬롯 위젯의 Visibility를 투명하게 만듦
    SetVisibility(ESlateVisibility::HitTestInvisible);

    UE_LOG(LogTemp, Log, TEXT("EquipmentSlot drag detected for slot index: %d, saved DraggedItemData: %s"),
        GetSlotIndex(), *DragOp->DraggedItemData.ItemName);
}

void UEquipmentSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    // 드래그가 취소되었으면 Visibility를 복원
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
            // 인벤토리에서 드래그한 경우를 처리
            UInventorySlot* InvSlot = Cast<UInventorySlot>(DragOp->Payload);
            if (InvSlot)
            {
                // 인벤토리 슬롯에서 드래그한 경우, NewItem 데이터를 로컬로 복사
                FItemData NewItem = InvSlot->GetItemData();
                if (NewItem.ItemName.IsEmpty())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Dropped item from inventory is empty."));
                    return false;
                }

                // 플레이어 컨트롤러 및 현재 캐릭터 가져오기
                AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
                if (PC)
                {
                    AProjectKLBCharacter* CurrentCharacter = Cast<AProjectKLBCharacter>(PC->GetPawn());
                    if (CurrentCharacter)
                    {
                        // 기존 장비 슬롯에 아이템이 있다면, 인벤토리의 빈 슬롯으로 이동
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

                        // 새 아이템을 장비 슬롯에 장착
                        CurrentCharacter->EquipItemInSlot(GetSlotType(), NewItem);
                        UE_LOG(LogTemp, Log, TEXT("Equipped item %s in equipment slot of type %d"),
                            *NewItem.ItemName, (int32)GetSlotType());

                        // 인벤토리에서 해당 아이템 제거
                        PC->RemoveInventoryItem(DragOp->FromSlotIndex);

                        // 장비 슬롯 UI 업데이트: 새 아이템으로 업데이트
                        InitializeEquipmentSlot(NewItem, GetSlotIndex());

                        // 인벤토리와 장비 UI 업데이트
                        PC->OnInventoryChanged.Broadcast(PC->Inventory);
                        PC->PCUpdateInventoryUI();
                        PC->PCUpdateEquipmentUI();

                        // 드롭 완료 후 Visibility 복원
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
            // 만약 Payload가 인벤토리 슬롯 위젯이 아닌 경우
            else
            {
                // 인벤토리 슬롯 간 교환 로직 처리
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







