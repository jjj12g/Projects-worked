// Fill out your copyright notice in the Description page of Project Settings.


#include "DataGameInstance.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "CharacterStat/CharacterStat.h"


void UDataGameInstance::LoadCardData()
{
    if (CardDataTable)
    {
        TArray<FCardData*> CardDataRows;
        CardDataTable->GetAllRows(TEXT("CardLoad"), CardDataRows);
    }
}

void UDataGameInstance::ShuffleCards()
{
    if (CardDataTable)
    {
        TArray<FCardData*> AllCardPointers;
        CardDataTable->GetAllRows(TEXT("CardShuffle"), AllCardPointers);

        TArray<FCardData> AllCards;
        for (FCardData* Card : AllCardPointers)
        {
            if (Card)
            {
                AllCards.Add(*Card); // �� ����
            }
        }

        // ī�� ����
        FMath::RandInit(FDateTime::Now().GetTicks());
        for (int32 i = 0; i < AllCards.Num(); ++i)
        {
            int32 j = FMath::RandRange(0, AllCards.Num() - 1);
            AllCards.Swap(i, j);
        }

        // ���� ī�� �� ����
        ShuffledCardDeck = AllCards;

        // �ʱ� ǥ���� ī�� ����
        UpdateDisplayedCards();
    }
}

FCardData UDataGameInstance::DrawCard()
{
    if (ShuffledCardDeck.Num() > 0)
    {
        FCardData DrawnCard = ShuffledCardDeck[0];
        ShuffledCardDeck.RemoveAt(0);

        // DisplayedCards ������Ʈ
        UpdateDisplayedCards();

        // �α� ���
        UE_LOG(LogTemp, Log, TEXT("Drawn Card: %s"), *DrawnCard.CardName);

        return DrawnCard;
    }

    UE_LOG(LogTemp, Warning, TEXT("No cards left to draw."));
    return FCardData(); // �⺻�� ��ȯ
}

void UDataGameInstance::UpdateDisplayedCards()
{
    DisplayedCards.Empty();

    for (int32 i = 0; i < FMath::Min(MaxDisplayCardCount, ShuffledCardDeck.Num()); ++i)
    {
        DisplayedCards.Add(ShuffledCardDeck[i]);
    }

    // �α� ���
    for (const FCardData& Card : DisplayedCards)
    {
        UE_LOG(LogTemp, Log, TEXT("Displayed Card: %s"), *Card.CardName);
    }
}



void UDataGameInstance::LoadCharacterStats()
{
    static const FString ContextString(TEXT("Character Stats Context"));

    // ������ ���̺� �ε�
    if (!CharacterStatsTable)
    {
        CharacterStatsTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/CharacterStats.CharacterStats"));
        UE_LOG(LogTemp, Warning, TEXT("Failed to load CharacterStatsTable!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterStatsTable loaded successfully!"));
    }

    if (CharacterStatsTable)
    {
        // Ư�� ���� ������ (��: �÷��̾�)
        FCharacterStat* PlayerStats = CharacterStatsTable->FindRow<FCharacterStat>(TEXT("Player"), ContextString);
        if (PlayerStats)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player Health: %d"), PlayerStats->Health);
            UE_LOG(LogTemp, Warning, TEXT("Player Attack: %d"), PlayerStats->Attack);
        }
    }

}

void UDataGameInstance::UseCardAndShuffle(const FCardData& UsedCard)
{
    // ����� ī�带 ���� �߰�
    ShuffledCardDeck.Add(UsedCard);

    // ǥ�õ� ī�带 ���� �߰�
    for (const FCardData& Card : DisplayedCards)
    {
        ShuffledCardDeck.Add(Card);
    }

    // �� ����
    ShuffleCards();

    // ���� ǥ���� ī�带 ����
    UpdateDisplayedCards();

    // �α� ���
    for (const FCardData& Card : DisplayedCards)
    {
        UE_LOG(LogTemp, Log, TEXT("New Displayed Card: %s"), *Card.CardName);
    }
}

// Init �Լ�: ���� �ν��Ͻ��� �ʱ�ȭ�� �� ������ ���̺��� �ε��ϰ�, DisplayedItems �迭�� ��� �������� �߰�.
void UDataGameInstance::Init()
{
    Super::Init();

    // ������ ���̺� �ε�
    if (LoadItemDataTable())
    {
        UE_LOG(LogTemp, Log, TEXT("ItemDataTable loaded successfully."));

        // DisplayedItems�� ��� ������ �߰�
        if (ItemDataTable)
        {
            TArray<FItemData*> AllRows;
            // �� ��° �Ű������� �����͸� ������ �迭�� ����
            ItemDataTable->GetAllRows<FItemData>(TEXT("ItemDataTableContext"), AllRows);

            for (auto& Row : AllRows)
            {
                if (Row)
                {
                    DisplayedItems.Add(*Row);
                    UE_LOG(LogTemp, Log, TEXT("Added item: %s"), *Row->ItemName);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Found null row in ItemDataTable."));
                }
            }

            UE_LOG(LogTemp, Log, TEXT("Loaded %d items into DisplayedItems."), DisplayedItems.Num());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is null after loading."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load ItemDataTable."));
    }
}


// LoadItemDataTable �Լ�: ������ ���̺��� �����Ǿ� �ִ��� Ȯ���մϴ�. �����Ϳ��� ItemDataTable�� �Ҵ��ϵ��� ����.
bool UDataGameInstance::LoadItemDataTable()
{
    // ItemDataTable�� �����Ϳ��� �����ߴ��� Ȯ��
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is not set in GameInstance."));
        return false;
    }

    return true;
}


// GetDisplayedItems �Լ� : �κ��丮 UI�� �ٸ� �ý��ۿ��� ����� �� �ֵ��� DisplayedItems �迭�� ��ȯ.
const TArray<FItemData>& UDataGameInstance::GetDisplayedItems() const
{
    return DisplayedItems;
}



FCardData* UDataGameInstance::FindCardDataByName(const FString& CardName)
{
    if (CardDataTable)
    {
        return CardDataTable->FindRow<FCardData>(*CardName, TEXT("CardSearch"));
    }
    return nullptr;
}

