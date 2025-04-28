// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CardDate/CardData.h"  // FCardData ����ü ����
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
    // ī�� ������ ���̺�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    UDataTable* CardDataTable;

    // ������ ���̺� ���۷���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
    UDataTable* CharacterStatsTable;

    // ī�� �����͸� �ε�
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    void LoadCardData();

    // ī�� ����
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    void ShuffleCards();

    // ī�� �̱�
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    FCardData DrawCard();

    // ���� ǥ�� ���� ī�� ��ȯ
    UFUNCTION(BlueprintCallable, Category = "Card Data")
    const TArray<FCardData>& GetDisplayedCards() const { return DisplayedCards; }

    // ĳ���� ������ �ε��ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void LoadCharacterStats();

    // ī�����
    void UseCardAndShuffle(const FCardData& UsedCard);

    virtual void Init() override;

    //   // ������ ���̺��� GetDisplayedItems ��������
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    const TArray<FItemData>& GetDisplayedItems() const;

    // ������ ���̺� �ε� �� �ʱ�ȭ
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool LoadItemDataTable();

private:
    // ī�� ã��
    FCardData* FindCardDataByName(const FString& CardName);

     // ī�� ��
    UPROPERTY()
    TArray<FCardData> ShuffledCardDeck;

    // ǥ���� ī�� ����Ʈ
    UPROPERTY()
    TArray<FCardData> DisplayedCards;

    // �ִ� ǥ�� ī�� ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data", meta = (AllowPrivateAccess = "true"))
    int32 MaxDisplayCardCount = 4;

    // ǥ���� ī�� ����Ʈ ����
    void UpdateDisplayedCards();

    // ������ ������ ���̺�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    UDataTable* ItemDataTable;

    // ǥ���� ������ ����Ʈ
    UPROPERTY()
    TArray<FItemData> DisplayedItems;
};
