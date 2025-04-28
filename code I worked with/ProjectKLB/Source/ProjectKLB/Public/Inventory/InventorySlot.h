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
    // ���� �ʱ�ȭ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    virtual void InitializeSlot(const FItemData& InItemData, int32 InSlotIndex);

    // ���� ���Կ� ����� ������ ��ȯ
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    virtual FItemData GetItemData() const { return ItemData; }


    // SlotIndex ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetSlotIndex() const { return SlotIndex; }

    // ���� �� ������ ��ȯ�� ���� �Լ� 
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetItemData(const FItemData& NewItemData) { ItemData = NewItemData; }



protected:
    // ������ �ε���
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 SlotIndex;

    // ������ ������ ������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    FItemData ItemData;
    
    // ���Կ� ǥ���� ������ �̹���
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIconImage;

    // ���Կ� ǥ���� ������ �̸�
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    // ���Կ� ǥ���� ������ ����, ���߿� ȣ���� ������?
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemQuantityText;


    // �巡�� �̺�Ʈ �Լ�
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // �巡�� ���� ����
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // �巡�� ���� �� �����ֱ�
    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
