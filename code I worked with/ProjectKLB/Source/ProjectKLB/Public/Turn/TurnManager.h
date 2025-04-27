// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardDate/CardData.h" // FCardData가 정의된 헤더 파일을 include
#include "UObject/NoExportTypes.h"
#include "TurnManager.generated.h"

/**
 * 
 */

 // 턴 관리 시스템
class UDataGameInstance;
class AProjectKLBPlayerController;
class AProjectKLBCharacter;


struct FTurnEntry
{
    AActor* Actor;
    float NextTurnTime;  // 다음 턴까지 남은 시간 또는 이니셔티브 값
    float Speed;

    // 정렬을 위한 비교 연산자 (NextTurnTime 기준)
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

    // 기본 생성자
    UTurnManager();

    // 초기화 함수
    void Initialize(UWorld* World);

    // 플레이어 턴 시작
    void StartPlayerTurn();

    // AI 턴 시작
    void StartAITurn();

    // 카드를 사용할 때 호출
    void CardUsed(const FString& CardName);

    // 턴 종료 처리
    void EndTurn();

    void InitializeTurnQueue();

    void ProcessNextTurn();

    void ExecuteTurnForActor(AActor* Actor);

    void StartTurnLoop();


    // 멀티플레이어 관련 변수 추가
    TArray<AProjectKLBPlayerController*> PlayerControllers;

    UPROPERTY()
    AProjectKLBPlayerController* PlayerControllerReference;

    // 플레이어 캐릭터를 턴 큐에 등록
    void RegisterCharacter(AProjectKLBCharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "Turn")
    AProjectKLBCharacter* SwitchToNextPlayer();


    // 플레이어 캐릭터 배열
    UPROPERTY()
    TArray<AProjectKLBCharacter*> PlayerCharacters;

private:
    TArray<FTurnEntry> TurnQueue;

    // 현재 턴의 경과 시간
    float ElapsedTimeDuringTurn;

    // 턴의 최대 지속 시간
    float TurnDuration;

    // 월드 컨텍스트
    UWorld* WorldContext;

    // 현재 턴의 카드를 관리
    TArray<FCardData> PlayerCards;

  
    int32 CurrentPlayerIndex = 0;


    // 타이머 핸들 추가
    FTimerHandle TurnTimerHandle;

  


};
