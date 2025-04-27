// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemData.h"
#include "Inventory.generated.h"

/**
 * 
 */

class UUniformGridPanel;
class UInventorySlot;

UCLASS()
class PROJECTKLB_API UInventory : public UUserWidget
{
	GENERATED_BODY()



public:

    // 인벤토리 초기화 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InitializeInventory(const TArray<FItemData>& Items);


protected:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* InventoryGrid;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UInventorySlot> InventorySlotClass;

    //bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

};
