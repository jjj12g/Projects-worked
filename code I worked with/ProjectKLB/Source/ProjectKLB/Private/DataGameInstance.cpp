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
                AllCards.Add(*Card); // 값 복사
            }
        }

        // 카드 섞기
        FMath::RandInit(FDateTime::Now().GetTicks());
        for (int32 i = 0; i < AllCards.Num(); ++i)
        {
            int32 j = FMath::RandRange(0, AllCards.Num() - 1);
            AllCards.Swap(i, j);
        }

        // 섞인 카드 덱 저장
        ShuffledCardDeck = AllCards;

        // 초기 표시할 카드 설정
        UpdateDisplayedCards();
    }
}

FCardData UDataGameInstance::DrawCard()
{
    if (ShuffledCardDeck.Num() > 0)
    {
        FCardData DrawnCard = ShuffledCardDeck[0];
        ShuffledCardDeck.RemoveAt(0);

        // DisplayedCards 업데이트
        UpdateDisplayedCards();

        // 로그 출력
        UE_LOG(LogTemp, Log, TEXT("Drawn Card: %s"), *DrawnCard.CardName);

        return DrawnCard;
    }

    UE_LOG(LogTemp, Warning, TEXT("No cards left to draw."));
    return FCardData(); // 기본값 반환
}

void UDataGameInstance::UpdateDisplayedCards()
{
    DisplayedCards.Empty();

    for (int32 i = 0; i < FMath::Min(MaxDisplayCardCount, ShuffledCardDeck.Num()); ++i)
    {
        DisplayedCards.Add(ShuffledCardDeck[i]);
    }

    // 로그 출력
    for (const FCardData& Card : DisplayedCards)
    {
        UE_LOG(LogTemp, Log, TEXT("Displayed Card: %s"), *Card.CardName);
    }
}



void UDataGameInstance::LoadCharacterStats()
{
    static const FString ContextString(TEXT("Character Stats Context"));

    // 데이터 테이블 로드
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
        // 특정 행을 가져옴 (예: 플레이어)
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
    // 사용한 카드를 덱에 추가
    ShuffledCardDeck.Add(UsedCard);

    // 표시된 카드를 덱에 추가
    for (const FCardData& Card : DisplayedCards)
    {
        ShuffledCardDeck.Add(Card);
    }

    // 덱 셔플
    ShuffleCards();

    // 새로 표시할 카드를 갱신
    UpdateDisplayedCards();

    // 로그 출력
    for (const FCardData& Card : DisplayedCards)
    {
        UE_LOG(LogTemp, Log, TEXT("New Displayed Card: %s"), *Card.CardName);
    }
}

// Init 함수: 게임 인스턴스가 초기화될 때 데이터 테이블을 로드하고, DisplayedItems 배열에 모든 아이템을 추가.
void UDataGameInstance::Init()
{
    Super::Init();

    // 데이터 테이블 로드
    if (LoadItemDataTable())
    {
        UE_LOG(LogTemp, Log, TEXT("ItemDataTable loaded successfully."));

        // DisplayedItems에 모든 아이템 추가
        if (ItemDataTable)
        {
            TArray<FItemData*> AllRows;
            // 두 번째 매개변수로 데이터를 저장할 배열을 전달
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


// LoadItemDataTable 함수: 데이터 테이블이 설정되어 있는지 확인합니다. 에디터에서 ItemDataTable을 할당하도록 설정.
bool UDataGameInstance::LoadItemDataTable()
{
    // ItemDataTable을 에디터에서 설정했는지 확인
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is not set in GameInstance."));
        return false;
    }

    return true;
}


// GetDisplayedItems 함수 : 인벤토리 UI나 다른 시스템에서 사용할 수 있도록 DisplayedItems 배열을 반환.
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

