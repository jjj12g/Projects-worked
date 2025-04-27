// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDate/DropZoneWidget.h"
#include "CardDate/CardWidget.h"
#include "CardDate/CardDragDropOperation.h"
#include "ProjectKLBPlayerController.h"


void UDropZoneWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

    // 드래그가 영역에 들어왔을 때 처리 (추가 작업 가능)
    UE_LOG(LogTemp, Log, TEXT("Drag entered DropZone"));
}

void UDropZoneWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);

    // 드래그가 영역을 떠났을 때 처리 (추가 작업 가능)
    UE_LOG(LogTemp, Log, TEXT("Drag left DropZone"));
}

bool UDropZoneWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // UCardDragDropOperation으로 캐스팅
    if (UCardDragDropOperation* CardOperation = Cast<UCardDragDropOperation>(InOperation))
    {
        // 드래그 시각화 위젯이 UCardWidget인지 확인
        if (UCardWidget* DroppedCardWidget = Cast<UCardWidget>(CardOperation->DefaultDragVisual))
        {
            // 카드 데이터를 가져와 처리
            HandleCardDrop(DroppedCardWidget->GetCardData());

            // 드롭된 카드 위젯을 화면에서 제거
            DroppedCardWidget->RemoveFromParent();

            return true;
        }
    }

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UDropZoneWidget::HandleCardDrop(const FCardData& CardData)
{
    UE_LOG(LogTemp, Log, TEXT("Card dropped: %s"), *CardData.CardName);

    // 플레이어 컨트롤러 가져오기
    AProjectKLBPlayerController* PlayerController = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found."));
        return;
    }

    // 카드 사용 호출
    PlayerController->UseCard(CardData);
    
}

