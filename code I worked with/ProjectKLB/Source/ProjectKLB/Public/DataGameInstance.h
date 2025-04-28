// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CardDate/CardData.h"  // FCardData 구조체 포함
#include "Inventory/ItemData.h"
#include "DataGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UDataGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    // 카드 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    UDataTable* CardDataTable;

    // 데이터 테이블 레퍼런스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
    UDataTable* CharacterStatsTable;

    // 카드 데이터를 로드
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    void LoadCardData();

    // 카드 셔플
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    void ShuffleCards();

    // 카드 뽑기
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    FCardData DrawCard();

    // 현재 표시 중인 카드 반환
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    const TArray<FCardData>& GetDisplayedCards() const { return DisplayedCards; }

    // 캐릭터 스탯을 로드하는 함수
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void LoadCharacterStats();

    // 카드셔플
    void UseCardAndShuffle(const FCardData& UsedCard);

    virtual void Init() override;

    //   // 데이터 테이블에서 GetDisplayedItems 가져오기
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    const TArray<FItemData>& GetDisplayedItems() const;

    // 데이터 테이블 로드 및 초기화
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool LoadItemDataTable();

private:
    // 카드 찾기
    FCardData* FindCardDataByName(const FString& CardName);

     // 카드 덱
    UPROPERTY()
    TArray<FCardData> ShuffledCardDeck;

    // 표시할 카드 리스트
    UPROPERTY()
    TArray<FCardData> DisplayedCards;

    // 최대 표시 카드 수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data", meta = (AllowPrivateAccess = "true"))
    int32 MaxDisplayCardCount = 4;

    // 표시할 카드 리스트 갱신
    void UpdateDisplayedCards();

    // 아이템 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    UDataTable* ItemDataTable;

    // 표시할 아이템 리스트
    UPROPERTY()
    TArray<FItemData> DisplayedItems;
};
