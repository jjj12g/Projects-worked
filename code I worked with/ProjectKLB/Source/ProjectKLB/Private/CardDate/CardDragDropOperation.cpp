// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDate/CardDragDropOperation.h"
#include "CardDate/CardWidget.h"
#include "Components/GridPanel.h"


//bool UCardWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//    if (UCardDragDropOperation* DragOperation = Cast<UCardDragDropOperation>(InOperation))
//    {
//        if (DragOperation->OriginalParent == CardPanel)
//        {
//            // GridPanel에 새 위치로 드래그된 위젯 추가
//            CardPanel->AddChildToGrid(DragOperation->DraggedCardWidget);
//
//            // 드래그된 위젯의 가시성 복원
//            DragOperation->DraggedCardWidget->SetVisibility(ESlateVisibility::Visible);
//
//            return true;
//        }
//    }
//
//    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//}