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
    // 드래그된 카드의 데이터를 보관
    UPROPERTY(BlueprintReadWrite, Category = "Drag and Drop")
    UCardWidget* DraggedCardWidget;

    // 드래그가 시작된 원래의 부모 컨테이너
    UPROPERTY(BlueprintReadWrite, Category = "Drag and Drop")
    UGridPanel* OriginalParent;

    // 추가적으로 필요한 데이터
    UPROPERTY(BlueprintReadWrite, Category = "Drag and Drop")
    FVector2D DragOffset;  // 드래그 시작 위치의 오프셋
};
