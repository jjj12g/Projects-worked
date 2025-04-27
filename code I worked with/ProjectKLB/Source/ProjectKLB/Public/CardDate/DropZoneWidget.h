// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h" // ī�� ������ ����ü ���
#include "DropZoneWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UDropZoneWidget : public UUserWidget
{
	GENERATED_BODY()

    // �巡�װ� DropZone�� ������ �� ȣ��
    virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // �巡�װ� DropZone�� ������ �� ȣ��
    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // �巡�װ� DropZone�� ��ӵǾ��� �� ȣ��
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // ��ӵ� ī�� ������ ó�� �Լ�
    void HandleCardDrop(const FCardData& CardData);


};
