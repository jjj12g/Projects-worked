// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDate/DropZoneWidget.h"
#include "CardDate/CardWidget.h"
#include "CardDate/CardDragDropOperation.h"
#include "ProjectKLBPlayerController.h"


void UDropZoneWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

    // �巡�װ� ������ ������ �� ó�� (�߰� �۾� ����)
    UE_LOG(LogTemp, Log, TEXT("Drag entered DropZone"));
}

void UDropZoneWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);

    // �巡�װ� ������ ������ �� ó�� (�߰� �۾� ����)
    UE_LOG(LogTemp, Log, TEXT("Drag left DropZone"));
}

bool UDropZoneWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // UCardDragDropOperation���� ĳ����
    if (UCardDragDropOperation* CardOperation = Cast<UCardDragDropOperation>(InOperation))
    {
        // �巡�� �ð�ȭ ������ UCardWidget���� Ȯ��
        if (UCardWidget* DroppedCardWidget = Cast<UCardWidget>(CardOperation->DefaultDragVisual))
        {
            // ī�� �����͸� ������ ó��
            HandleCardDrop(DroppedCardWidget->GetCardData());

            // ��ӵ� ī�� ������ ȭ�鿡�� ����
            DroppedCardWidget->RemoveFromParent();

            return true;
        }
    }

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UDropZoneWidget::HandleCardDrop(const FCardData& CardData)
{
    UE_LOG(LogTemp, Log, TEXT("Card dropped: %s"), *CardData.CardName);

    // �÷��̾� ��Ʈ�ѷ� ��������
    AProjectKLBPlayerController* PlayerController = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found."));
        return;
    }

    // ī�� ��� ȣ��
    PlayerController->UseCard(CardData);
    
}

