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
//            // GridPanel�� �� ��ġ�� �巡�׵� ���� �߰�
//            CardPanel->AddChildToGrid(DragOperation->DraggedCardWidget);
//
//            // �巡�׵� ������ ���ü� ����
//            DragOperation->DraggedCardWidget->SetVisibility(ESlateVisibility::Visible);
//
//            return true;
//        }
//    }
//
//    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//}