// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/TurnManager.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "DataGameInstance.h"  // 게임 인스턴스를 포함
#include "CardDate/CardData.h"  // FCardData 구조체 포함
#include "ProjectKLBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectKLBCharacter.h"
#include "Enemy/Enemy.h"
#include "ProjectKLBCharacter.h"  // 플레이어 캐릭터 클래스 헤더 포함
#include "Player/CharacterStatComponent.h"




UTurnManager::UTurnManager()
{
    // 기본 값 설정
    ElapsedTimeDuringTurn = 0.0f;
    //TurnDuration = 10.0f;  // 기본 턴 지속 시간 (10초 예시)

   
}

void UTurnManager::Initialize(UWorld* World)
{
    WorldContext = World;
    ElapsedTimeDuringTurn = 0.0f;
    CurrentPlayerIndex = 0;

    // 모든 플레이어 컨트롤러 수집
    if (WorldContext)
    {
        for (FConstPlayerControllerIterator Iterator = WorldContext->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            if (AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(*Iterator))
            {
                PlayerControllers.Add(PC);
            }
        }
    }
}

void UTurnManager::RegisterCharacter(AProjectKLBCharacter* Character)
{
    if (!Character) return;


    // 플레이어 캐릭터 배열에 추가
    PlayerCharacters.Add(Character);

    float ActorSpeed = 0.0f;
    // StatComponent가 존재하면 FinalStats.Speed 값을 사용
    if (Character->StatComponent)
    {
        ActorSpeed = Character->StatComponent->FinalStats.Speed;
        UE_LOG(LogTemp, Warning, TEXT("is StatComp"));
    }
    else
    {
        ActorSpeed = Character->CharacterStats.Speed; // 이전 방식 (fallback)
        UE_LOG(LogTemp, Warning, TEXT("not StatComp"));
    }

    UE_LOG(LogTemp, Log, TEXT("Registering %s with speed: %f"), *Character->GetName(), ActorSpeed);

    if (ActorSpeed <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Warning: %s has invalid speed: %f"), *Character->GetName(), ActorSpeed);
    }


    // 턴 엔트리 생성
    FTurnEntry Entry;
    Entry.Actor = Character;
    Entry.Speed = ActorSpeed;
    Entry.NextTurnTime = (ActorSpeed > 0.0f) ? (100.0f / ActorSpeed) : FLT_MAX;

    // 턴 큐에 추가 및 정렬
    TurnQueue.Add(Entry);
    TurnQueue.Sort([](const FTurnEntry& A, const FTurnEntry& B) {
        return A.NextTurnTime < B.NextTurnTime;
        });
}



void UTurnManager::StartPlayerTurn()
{
    if (WorldContext)
    {
        UDataGameInstance* GameInstance = Cast<UDataGameInstance>(WorldContext->GetGameInstance());
        if (GameInstance)
        {
            // 카드 셔플 및 카드 분배
            GameInstance->ShuffleCards();
            ElapsedTimeDuringTurn = 0.0f;
            PlayerCards.Empty(); // 기존 카드 제거

            // 데이터 테이블에서 카드 데이터를 가져옴
            TArray<FCardData*> CardDataRows;
            GameInstance->CardDataTable->GetAllRows(TEXT("PlayerTurn"), CardDataRows);
            for (FCardData* CardData : CardDataRows)
            {
                if (CardData)
                {
                    PlayerCards.Add(*CardData); // 카드 추가
                }
            }
            UE_LOG(LogTemp, Log, TEXT("Player turn started with %d cards."), PlayerCards.Num());

            // 현재 플레이어 턴 시작
            if (PlayerControllers.IsValidIndex(CurrentPlayerIndex))
            {
                PlayerControllers[CurrentPlayerIndex]->StartTurn();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No valid player controller at index %d"), CurrentPlayerIndex);
            }
        }
    }

    // 기존 적 턴 로직 실행
    TArray<AActor*> FoundAIControllers;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, AEnemyAIController::StaticClass(), FoundAIControllers);

    for (AActor* Actor : FoundAIControllers)
    {
        if (AEnemyAIController* AIController = Cast<AEnemyAIController>(Actor))
        {
            if (AIController->GetBlackboardComponent())
            {
                AIController->GetBlackboardComponent()->SetValueAsBool("PlayerCardUsed?", false);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AIController BlackboardComponent null."));
            }
        }
    }
}

void UTurnManager::CardUsed(const FString& CardName)
{
    if (WorldContext)
    {
        UDataGameInstance* GameInstance = Cast<UDataGameInstance>(WorldContext->GetGameInstance());

        if (GameInstance)
        {
            EndTurn();
            //// 턴 지속 시간 초과 시 턴 종료
            //if (ElapsedTimeDuringTurn >= TurnDuration)
            //{
            //    
            //}
            //else
            //{
            //    UE_LOG(LogTemp, Warning, TEXT("Card '%s' not found or invalid speed."), *CardName);
            //}
        }
    }
}

void UTurnManager::EndTurn()
{
    ElapsedTimeDuringTurn = 0.0f;
    UE_LOG(LogTemp, Log, TEXT("Turn ended. Resetting elapsed time."));

    // 플레이어 턴 종료 시 다음 턴 실행
    ProcessNextTurn();
}

void UTurnManager::InitializeTurnQueue()
{
    // 턴 큐 초기화
    TurnQueue.Empty();

    // 플레이어와 적 액터를 별도로 수집
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, AProjectKLBCharacter::StaticClass(), PlayerActors);
    UE_LOG(LogTemp, Log, TEXT("Found %d player characters."), PlayerActors.Num());

    TArray<AActor*> EnemyActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, AEnemy::StaticClass(), EnemyActors);
    UE_LOG(LogTemp, Log, TEXT("Found %d enemy characters."), EnemyActors.Num());

    // 두 배열을 결합
    TArray<AActor*> AllActors = PlayerActors;
    AllActors.Append(EnemyActors);
    UE_LOG(LogTemp, Log, TEXT("Total actors after combining: %d."), AllActors.Num());

    // 각 액터에 대해 턴 엔트리 생성
    for (AActor* Actor : AllActors)
    {
        if (!Actor) continue;

        ACharacter* Character = Cast<ACharacter>(Actor);
        if (!Character) continue;

        float ActorSpeed = 0.0f;
        if (AProjectKLBCharacter* PlayerChar = Cast<AProjectKLBCharacter>(Character))
        {
            // StatComponent가 존재하면 FinalStats.Speed 값을 사용합니다.
            if (PlayerChar->StatComponent)
            {
                ActorSpeed = PlayerChar->StatComponent->FinalStats.Speed;
            }
            else
            {
                ActorSpeed = 0.0f;
                UE_LOG(LogTemp, Warning, TEXT("Player %s has no StatComponent."), *PlayerChar->GetName());
            }
            UE_LOG(LogTemp, Log, TEXT("Player %s speed: %f"), *PlayerChar->GetName(), ActorSpeed);
        }
        else if (AEnemy* EnemyChar = Cast<AEnemy>(Character))
        {
            // 적 캐릭터는 기존 방식(또는 동일한 시스템 적용)
            ActorSpeed = EnemyChar->CharacterStats.Speed;
            UE_LOG(LogTemp, Log, TEXT("Enemy %s speed: %f"), *EnemyChar->GetName(), ActorSpeed);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Character %s is neither PlayerChar nor Enemy."), *Character->GetName());
        }

       

        // 턴 엔트리 생성 및 초기화
        FTurnEntry Entry;
        Entry.Actor = Actor;
        Entry.Speed = ActorSpeed;
        if (ActorSpeed <= 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Warning: %s has invalid speed: %f"), *Character->GetName(), ActorSpeed);
            Entry.NextTurnTime = FLT_MAX;
        }
        else
        {
            Entry.NextTurnTime = 100.0f / ActorSpeed;
        }

        TurnQueue.Add(Entry);
    }

    // NextTurnTime 기준으로 정렬
    TurnQueue.Sort([](const FTurnEntry& A, const FTurnEntry& B) {
        return A.NextTurnTime < B.NextTurnTime;
        });
}

void UTurnManager::ProcessNextTurn()
{
    // 턴 큐가 비어있으면 바로 리턴
    if (TurnQueue.Num() == 0) return;

    // 턴 큐에서 가장 빠른 턴 추출
    FTurnEntry NextTurn = TurnQueue[0];
    TurnQueue.RemoveAt(0);

    // 만약 NextTurn.Actor가 유효하지 않다면(파괴되었거나 PendingKill 상태)
    if (!NextTurn.Actor || !IsValid(NextTurn.Actor))
    {
        UE_LOG(LogTemp, Warning, TEXT("ProcessNextTurn: Actor is invalid, skipping."));
        // 다음 턴 처리 전에 해당 엔트리를 건너뜁니다.
        ProcessNextTurn();
        return;
    }

    // NextTurn.Actor의 턴 실행
    ExecuteTurnForActor(NextTurn.Actor);

    // 턴 후 업데이트: 다음 턴 시간 계산
    // 만약 Actor가 이미 파괴되어 있다면 Speed 값이 0이 될 수 있으므로, 조건문을 추가
    if (NextTurn.Speed > 0.0f)
    {
        NextTurn.NextTurnTime += 100.0f / NextTurn.Speed;
    }
    else
    {
        NextTurn.NextTurnTime = FLT_MAX;
    }
    TurnQueue.Add(NextTurn);

    // NextTurnTime 기준으로 정렬
    TurnQueue.Sort([](const FTurnEntry& A, const FTurnEntry& B) {
        return A.NextTurnTime < B.NextTurnTime;
        });
}

void UTurnManager::ExecuteTurnForActor(AActor* Actor)
{
    // 유효성 검사: Actor가 유효하지 않으면 아무것도 수행하지 않습니다.
    if (!Actor || !IsValid(Actor))
    {
        UE_LOG(LogTemp, Warning, TEXT("ExecuteTurnForActor: Actor is invalid, skipping."));
        return;
    }

    if (AProjectKLBCharacter* PlayerChar = Cast<AProjectKLBCharacter>(Actor))
    {
        UE_LOG(LogTemp, Log, TEXT("Executing turn for player: %s"), *PlayerChar->GetName());

        // PlayerControllers 배열을 순회하면서 해당 캐릭터를 소유하고 턴 시작
        for (AProjectKLBPlayerController* PC : PlayerControllers)
        {
            if (PC)
            {
                PC->UnPossess();              // 현재 소유 중인 Pawn 해제
                PC->Possess(PlayerChar);      // 새 캐릭터 소유
                APawn* CurrentPawn = PC->GetPawn();
                if (CurrentPawn)
                {
                    UE_LOG(LogTemp, Log, TEXT("Now possessed pawn: %s"), *CurrentPawn->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Possession failed for %s"), *PlayerChar->GetName());
                }
                PC->StartTurn();              // 턴 시작
                break;                        // 한 컨트롤러가 턴을 수행하면 루프 종료
            }
        }
    }
    else if (AEnemy* EnemyChar = Cast<AEnemy>(Actor))
    {
        UE_LOG(LogTemp, Log, TEXT("Executing turn for enemy: %s"), *EnemyChar->GetName());
        // 적 행동 실행 로직 구현
        StartAITurn();
        // 예: AIController를 통한 행동 시작
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown actor type in ExecuteTurnForActor: %s"), *Actor->GetName());
    }
}

void UTurnManager::StartTurnLoop()
{
    // 일정 간격(예: 0.1초)마다 ProcessNextTurn 호출
    WorldContext->GetTimerManager().SetTimer(TurnTimerHandle, this, &UTurnManager::ProcessNextTurn, 2.0f, true);
}


void UTurnManager::StartAITurn()
{
    UE_LOG(LogTemp, Log, TEXT("AI Turn Started."));

    // 기존 적 턴 로직 실행
    TArray<AActor*> FoundAIControllers;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, AEnemyAIController::StaticClass(), FoundAIControllers);

    for (AActor* Actor : FoundAIControllers)
    {
        if (AEnemyAIController* AIController = Cast<AEnemyAIController>(Actor))
        {
            if (AIController->GetBlackboardComponent())
            {
                AIController->GetBlackboardComponent()->SetValueAsBool("PlayerCardUsed?", true);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AIController BlackboardComponent null."));
            }
        }
    }


    // AI 턴 종료 후 다음 플레이어 턴 시작
    EndTurn();
    //StartPlayerTurn();
}

AProjectKLBCharacter* UTurnManager::SwitchToNextPlayer()
{
    if (PlayerCharacters.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No registered player characters in TurnManager."));
        return nullptr;
    }

    int32 OriginalIndex = CurrentPlayerIndex;
    do
    {
        // 인덱스 증가 후 유효성 검사
        CurrentPlayerIndex = (CurrentPlayerIndex + 1) % PlayerCharacters.Num();
        AProjectKLBCharacter* NextPlayer = PlayerCharacters[CurrentPlayerIndex];
        if (NextPlayer && IsValid(NextPlayer))
        {
            UE_LOG(LogTemp, Log, TEXT("SwitchToNextPlayer: Next valid player is %s"), *NextPlayer->GetName());
            return NextPlayer;
        }
    } while (CurrentPlayerIndex != OriginalIndex);

    UE_LOG(LogTemp, Warning, TEXT("No valid player characters found in TurnManager."));
    return nullptr;
}
