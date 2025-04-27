// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDate/CardListWidget.h"
#include "Components/HorizontalBox.h"    // UHorizontalBox 사용
#include "Components/HorizontalBoxSlot.h" // UHorizontalBoxSlot 사용
#include "DataGameInstance.h"
#include "CardDate/CardWidget.h"
#include "Components/GridPanel.h" // 그리드 패널 헤더
#include "CardDate/DropZoneWidget.h"


void UCardListWidget::AddCard(UCardWidget* CardWidget)
{
    if (CardContainer && CardWidget)
    {
        CardContainer->AddChildToHorizontalBox(CardWidget);

        // 카드 간 간격 설정
        UHorizontalBoxSlot* CardSlot = Cast<UHorizontalBoxSlot>(CardWidget->Slot);
        if (CardSlot)
        {
            CardSlot->SetPadding(FMargin(5.0f));  // 간격 설정
            CardSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
        }
    }
}

void UCardListWidget::ClearAllCards()
{
    if (CardContainer)
    {
        CardContainer->ClearChildren(); // 컨테이너에서 모든 카드 제거
    }
}

void UCardListWidget::SetCardVisibility(ESlateVisibility NewVisibility)
{
    if (CardContainer)
    {
        UE_LOG(LogTemp, Log, TEXT("CardContainer is valid, setting visibility."));
        CardContainer->SetVisibility(NewVisibility);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CardContainer is null!"));
    }
}

void UCardListWidget::UpdateCardUI()
{
    if (UDataGameInstance* GameInstance = GetWorld()->GetGameInstance<UDataGameInstance>())
    {
        const TArray<FCardData>& DisplayedCards = GameInstance->GetDisplayedCards();

        // 기존 UI 초기화
        ClearAllCards();

        // 새 카드를 UI에 추가
        for (const FCardData& CardData : DisplayedCards)
        {
            if (CardWidgetClass)
            {
                UCardWidget* NewCardWidget = CreateWidget<UCardWidget>(this, CardWidgetClass);
                if (NewCardWidget)
                {
                    NewCardWidget->InitializeCard(CardData);
                    AddCard(NewCardWidget);
                }
            }
        }
    }
}

