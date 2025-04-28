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
    // �ش� ������ ��Ÿ���� ��� Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    EEquipmentType SlotType;

    // ��� ���� �ʱ�ȭ �Լ� 
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void InitializeEquipmentSlot(const FItemData& InEquipmentData, int32 InSlotIndex);

    // ���� ������ ������ �����͸� ��ȯ 
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    FItemData GetCurrentEquippedItem() const;

    // ������ ��� Ÿ�� ��ȯ
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    EEquipmentType GetSlotType() const { return SlotType; }

 
    int32 GetSlotIndex() const { return SlotIndex; }

protected:

    int32 SlotIndex;

    // ���Կ� ������ ������ ������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    FItemData EquipmentData;

    // �巡�� �� ��� ���� �Լ��� �������̵�
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // �巡�� �� ��� �̺�Ʈ ó��
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
