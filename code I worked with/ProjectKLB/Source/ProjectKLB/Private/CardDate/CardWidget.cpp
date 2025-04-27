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

    // 클래스 멤버 변수 CardData에 매개변수 데이터를 할당
    CardData = InCardData;

}



void UCardWidget::OnCardUsed()
{
     //현재 UI 위젯을 소유한 플레이어 컨트롤러 가져오기
      AProjectKLBPlayerController* PlayerController = Cast<AProjectKLBPlayerController>(GetOwningPlayer());
      if (!PlayerController)
      {
          UE_LOG(LogTemp, Warning, TEXT("PlayerController not found."));
          return;
      }
  
      UE_LOG(LogTemp, Warning, TEXT("OnCardUsed."));
    
}

// 드래그 동작을 시작할 때 호출
FReply UCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

// 마우스 버튼을 눌렀을 때 호출
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

        // CardPanel을 부모 컨테이너로 설정
        DragOperation->OriginalParent = CardPanel;

        // 드래그 시작 위치의 오프셋 저장
        DragOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        OutOperation = DragOperation;

        // 드래그 중 카드 위젯을 투명화
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}

bool UCardWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (UCardDragDropOperation* DragOperation = Cast<UCardDragDropOperation>(InOperation))
    {
        if (DragOperation->OriginalParent == CardPanel)
        {
            // CardPanel에 새 위치로 드래그된 위젯 추가
            CardPanel->AddChildToGrid(DragOperation->DraggedCardWidget);

            // 드롭 성공 시 카드 사용 처리
            OnCardUsed();
            UE_LOG(LogTemp, Warning, TEXT("On Drop"));
            // 드래그된 위젯의 가시성 복원
            DragOperation->DraggedCardWidget->SetVisibility(ESlateVisibility::Visible);

            return true;
        }
    }

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}