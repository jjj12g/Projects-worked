// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CardDragDropOperation.generated.h"

/**
 * 
 */
class UCardWidget;
class UGridPanel;

UCLASS()
class PROJECTKLB_API UCardDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
    // �巡�׵� ī���� �����͸� ����
    UPROPERTY(BlueprintReadWrite, Category = "Drag and Drop")
    UCardWidget* DraggedCardWidget;

    // �巡�װ� ���۵� ������ �θ� �����̳�
    UPROPERTY(BlueprintReadWrite, Category = "Drag and Drop")
    UGridPanel* OriginalParent;

    // �߰������� �ʿ��� ������
    UPROPERTY(BlueprintReadWrite, Category = "Drag and Drop")
    FVector2D DragOffset;  // �巡�� ���� ��ġ�� ������
};
