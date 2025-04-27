// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h" // 카드 데이터 구조체 헤더
#include "DropZoneWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UDropZoneWidget : public UUserWidget
{
	GENERATED_BODY()

    // 드래그가 DropZone에 들어왔을 때 호출
    virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드래그가 DropZone을 떠났을 때 호출
    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드래그가 DropZone에 드롭되었을 때 호출
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드롭된 카드 데이터 처리 함수
    void HandleCardDrop(const FCardData& CardData);


};
