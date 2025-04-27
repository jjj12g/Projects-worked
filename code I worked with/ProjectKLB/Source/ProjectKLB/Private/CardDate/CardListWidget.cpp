// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDate/CardListWidget.h"
#include "Components/HorizontalBox.h"    // UHorizontalBox ���
#include "Components/HorizontalBoxSlot.h" // UHorizontalBoxSlot ���
#include "DataGameInstance.h"
#include "CardDate/CardWidget.h"
#include "Components/GridPanel.h" // �׸��� �г� ���
#include "CardDate/DropZoneWidget.h"


void UCardListWidget::AddCard(UCardWidget* CardWidget)
{
    if (CardContainer && CardWidget)
    {
        CardContainer->AddChildToHorizontalBox(CardWidget);

        // ī�� �� ���� ����
        UHorizontalBoxSlot* CardSlot = Cast<UHorizontalBoxSlot>(CardWidget->Slot);
        if (CardSlot)
        {
            CardSlot->SetPadding(FMargin(5.0f));  // ���� ����
            CardSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
        }
    }
}

void UCardListWidget::ClearAllCards()
{
    if (CardContainer)
    {
        CardContainer->ClearChildren(); // �����̳ʿ��� ��� ī�� ����
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

        // ���� UI �ʱ�ȭ
        ClearAllCards();

        // �� ī�带 UI�� �߰�
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

