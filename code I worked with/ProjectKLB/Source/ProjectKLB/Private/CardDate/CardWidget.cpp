// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDate/CardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CardDate/CardDragDropOperation.h"
#include "Components/GridPanel.h"
#include "ProjectKLBPlayerController.h"




void UCardWidget::InitializeCard(const FCardData& InCardData)
{
    if (CardNameText)
    {
        CardNameText->SetText(FText::FromString(InCardData.CardName));
    }

    if (CardDescriptionText)
    {
        CardDescriptionText->SetText(FText::FromString(InCardData.CardDescription));
    }

    if (CardIconImage && InCardData.CardIcon)
    {
        CardIconImage->SetBrushFromTexture(InCardData.CardIcon);
    }

    // Ŭ���� ��� ���� CardData�� �Ű����� �����͸� �Ҵ�
    CardData = InCardData;

}



void UCardWidget::OnCardUsed()
{
     //���� UI ������ ������ �÷��̾� ��Ʈ�ѷ� ��������
      AProjectKLBPlayerController* PlayerController = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
      if (!PlayerController)
      {
          UE_LOG(LogTemp, Warning, TEXT("PlayerController not found."));
          return;
      }
  
      UE_LOG(LogTemp, Warning, TEXT("OnCardUsed."));
    
}

// �巡�� ������ ������ �� ȣ��
FReply UCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

// ���콺 ��ư�� ������ �� ȣ��
void UCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    if (!CardPanel)
    {
        UE_LOG(LogTemp, Warning, TEXT("GridPanel is not initialized!"));
        return;
    }

    UCardDragDropOperation* DragOperation = NewObject<UCardDragDropOperation>();
    if (DragOperation)
    {
        DragOperation->DefaultDragVisual = this;
        DragOperation->DraggedCardWidget = this;

        // CardPanel�� �θ� �����̳ʷ� ����
        DragOperation->OriginalParent = CardPanel;

        // �巡�� ���� ��ġ�� ������ ����
        DragOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        OutOperation = DragOperation;

        // �巡�� �� ī�� ������ ����ȭ
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}

bool UCardWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (UCardDragDropOperation* DragOperation = Cast<UCardDragDropOperation>(InOperation))
    {
        if (DragOperation->OriginalParent == CardPanel)
        {
            // CardPanel�� �� ��ġ�� �巡�׵� ���� �߰�
            CardPanel->AddChildToGrid(DragOperation->DraggedCardWidget);

            // ��� ���� �� ī�� ��� ó��
            OnCardUsed();
            UE_LOG(LogTemp, Warning, TEXT("On Drop"));
            // �巡�׵� ������ ���ü� ����
            DragOperation->DraggedCardWidget->SetVisibility(ESlateVisibility::Visible);

            return true;
        }
    }

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}