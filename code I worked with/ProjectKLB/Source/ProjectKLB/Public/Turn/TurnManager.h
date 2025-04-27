// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardDate/CardData.h" // FCardData�� ���ǵ� ��� ������ include
#include "UObject/NoExportTypes.h"
#include "TurnManager.generated.h"

/**
 * 
 */

 // �� ���� �ý���
class UDataGameInstance;
class AProjectKLBPlayerController;
class AProjectKLBCharacter;


struct FTurnEntry
{
    AActor* Actor;
    float NextTurnTime;  // ���� �ϱ��� ���� �ð� �Ǵ� �̴ϼ�Ƽ�� ��
    float Speed;

    // ������ ���� �� ������ (NextTurnTime ����)
    bool operator<(const FTurnEntry& Other) const
    {
        return NextTurnTime < Other.NextTurnTime;
    }
};



UCLASS()
class PROJECTKLB_API UTurnManager : public UObject
{
	GENERATED_BODY()

public:

    // �⺻ ������
    UTurnManager();

    // �ʱ�ȭ �Լ�
    void Initialize(UWorld* World);

    // �÷��̾� �� ����
    void StartPlayerTurn();

    // AI �� ����
    void StartAITurn();

    // ī�带 ����� �� ȣ��
    void CardUsed(const FString& CardName);

    // �� ���� ó��
    void EndTurn();

    void InitializeTurnQueue();

    void ProcessNextTurn();

    void ExecuteTurnForActor(AActor* Actor);

    void StartTurnLoop();


    // ��Ƽ�÷��̾� ���� ���� �߰�
    TArray<AProjectKLBPlayerController*> PlayerControllers;

    UPROPERTY()
    AProjectKLBPlayerController* PlayerControllerReference;

    // �÷��̾� ĳ���͸� �� ť�� ���
    void RegisterCharacter(AProjectKLBCharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "Turn")
    AProjectKLBCharacter* SwitchToNextPlayer();


    // �÷��̾� ĳ���� �迭
    UPROPERTY()
    TArray<AProjectKLBCharacter*> PlayerCharacters;

private:
    TArray<FTurnEntry> TurnQueue;

    // ���� ���� ��� �ð�
    float ElapsedTimeDuringTurn;

    // ���� �ִ� ���� �ð�
    float TurnDuration;

    // ���� ���ؽ�Ʈ
    UWorld* WorldContext;

    // ���� ���� ī�带 ����
    TArray<FCardData> PlayerCards;

  
    int32 CurrentPlayerIndex = 0;


    // Ÿ�̸� �ڵ� �߰�
    FTimerHandle TurnTimerHandle;

  


};
