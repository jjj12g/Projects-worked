// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/TurnManager.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "DataGameInstance.h"  // ���� �ν��Ͻ��� ����
#include "CardDate/CardData.h"  // FCardData ����ü ����
#include "ProjectKLBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectKLBCharacter.h"
#include "Enemy/Enemy.h"
#include "ProjectKLBCharacter.h"  // �÷��̾� ĳ���� Ŭ���� ��� ����
#include "Player/CharacterStatComponent.h"




UTurnManager::UTurnManager()
{
    // �⺻ �� ����
    ElapsedTimeDuringTurn = 0.0f;
    //TurnDuration = 10.0f;  // �⺻ �� ���� �ð� (10�� ����)

   
}

void UTurnManager::Initialize(UWorld* World)
{
    WorldContext = World;
    ElapsedTimeDuringTurn = 0.0f;
    CurrentPlayerIndex = 0;

    // ��� �÷��̾� ��Ʈ�ѷ� ����
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


    // �÷��̾� ĳ���� �迭�� �߰�
    PlayerCharacters.Add(Character);

    float ActorSpeed = 0.0f;
    // StatComponent�� �����ϸ� FinalStats.Speed ���� ���
    if (Character->StatComponent)
    {
        ActorSpeed = Character->StatComponent->FinalStats.Speed;
        UE_LOG(LogTemp, Warning, TEXT("is StatComp"));
    }
    else
    {
        ActorSpeed = Character->CharacterStats.Speed; // ���� ��� (fallback)
        UE_LOG(LogTemp, Warning, TEXT("not StatComp"));
    }

    UE_LOG(LogTemp, Log, TEXT("Registering %s with speed: %f"), *Character->GetName(), ActorSpeed);

    if (ActorSpeed <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Warning: %s has invalid speed: %f"), *Character->GetName(), ActorSpeed);
    }


    // �� ��Ʈ�� ����
    FTurnEntry Entry;
    Entry.Actor = Character;
    Entry.Speed = ActorSpeed;
    Entry.NextTurnTime = (ActorSpeed > 0.0f) ? (100.0f / ActorSpeed) : FLT_MAX;

    // �� ť�� �߰� �� ����
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
            // ī�� ���� �� ī�� �й�
            GameInstance->ShuffleCards();
            ElapsedTimeDuringTurn = 0.0f;
            PlayerCards.Empty(); // ���� ī�� ����

            // ������ ���̺��� ī�� �����͸� ������
            TArray<FCardData*> CardDataRows;
            GameInstance->CardDataTable->GetAllRows(TEXT("PlayerTurn"), CardDataRows);
            for (FCardData* CardData : CardDataRows)
            {
                if (CardData)
                {
                    PlayerCards.Add(*CardData); // ī�� �߰�
                }
            }
            UE_LOG(LogTemp, Log, TEXT("Player turn started with %d cards."), PlayerCards.Num());

            // ���� �÷��̾� �� ����
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

    // ���� �� �� ���� ����
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
            //// �� ���� �ð� �ʰ� �� �� ����
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

    // �÷��̾� �� ���� �� ���� �� ����
    ProcessNextTurn();
}

void UTurnManager::InitializeTurnQueue()
{
    // �� ť �ʱ�ȭ
    TurnQueue.Empty();

    // �÷��̾�� �� ���͸� ������ ����
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, AProjectKLBCharacter::StaticClass(), PlayerActors);
    UE_LOG(LogTemp, Log, TEXT("Found %d player characters."), PlayerActors.Num());

    TArray<AActor*> EnemyActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, AEnemy::StaticClass(), EnemyActors);
    UE_LOG(LogTemp, Log, TEXT("Found %d enemy characters."), EnemyActors.Num());

    // �� �迭�� ����
    TArray<AActor*> AllActors = PlayerActors;
    AllActors.Append(EnemyActors);
    UE_LOG(LogTemp, Log, TEXT("Total actors after combining: %d."), AllActors.Num());

    // �� ���Ϳ� ���� �� ��Ʈ�� ����
    for (AActor* Actor : AllActors)
    {
        if (!Actor) continue;

        ACharacter* Character = Cast<ACharacter>(Actor);
        if (!Character) continue;

        float ActorSpeed = 0.0f;
        if (AProjectKLBCharacter* PlayerChar = Cast<AProjectKLBCharacter>(Character))
        {
            // StatComponent�� �����ϸ� FinalStats.Speed ���� ����մϴ�.
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
            // �� ĳ���ʹ� ���� ���(�Ǵ� ������ �ý��� ����)
            ActorSpeed = EnemyChar->CharacterStats.Speed;
            UE_LOG(LogTemp, Log, TEXT("Enemy %s speed: %f"), *EnemyChar->GetName(), ActorSpeed);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Character %s is neither PlayerChar nor Enemy."), *Character->GetName());
        }

       

        // �� ��Ʈ�� ���� �� �ʱ�ȭ
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

    // NextTurnTime �������� ����
    TurnQueue.Sort([](const FTurnEntry& A, const FTurnEntry& B) {
        return A.NextTurnTime < B.NextTurnTime;
        });
}

void UTurnManager::ProcessNextTurn()
{
    // �� ť�� ��������� �ٷ� ����
    if (TurnQueue.Num() == 0) return;

    // �� ť���� ���� ���� �� ����
    FTurnEntry NextTurn = TurnQueue[0];
    TurnQueue.RemoveAt(0);

    // ���� NextTurn.Actor�� ��ȿ���� �ʴٸ�(�ı��Ǿ��ų� PendingKill ����)
    if (!NextTurn.Actor || !IsValid(NextTurn.Actor))
    {
        UE_LOG(LogTemp, Warning, TEXT("ProcessNextTurn: Actor is invalid, skipping."));
        // ���� �� ó�� ���� �ش� ��Ʈ���� �ǳʶݴϴ�.
        ProcessNextTurn();
        return;
    }

    // NextTurn.Actor�� �� ����
    ExecuteTurnForActor(NextTurn.Actor);

    // �� �� ������Ʈ: ���� �� �ð� ���
    // ���� Actor�� �̹� �ı��Ǿ� �ִٸ� Speed ���� 0�� �� �� �����Ƿ�, ���ǹ��� �߰�
    if (NextTurn.Speed > 0.0f)
    {
        NextTurn.NextTurnTime += 100.0f / NextTurn.Speed;
    }
    else
    {
        NextTurn.NextTurnTime = FLT_MAX;
    }
    TurnQueue.Add(NextTurn);

    // NextTurnTime �������� ����
    TurnQueue.Sort([](const FTurnEntry& A, const FTurnEntry& B) {
        return A.NextTurnTime < B.NextTurnTime;
        });
}

void UTurnManager::ExecuteTurnForActor(AActor* Actor)
{
    // ��ȿ�� �˻�: Actor�� ��ȿ���� ������ �ƹ��͵� �������� �ʽ��ϴ�.
    if (!Actor || !IsValid(Actor))
    {
        UE_LOG(LogTemp, Warning, TEXT("ExecuteTurnForActor: Actor is invalid, skipping."));
        return;
    }

    if (AProjectKLBCharacter* PlayerChar = Cast<AProjectKLBCharacter>(Actor))
    {
        UE_LOG(LogTemp, Log, TEXT("Executing turn for player: %s"), *PlayerChar->GetName());

        // PlayerControllers �迭�� ��ȸ�ϸ鼭 �ش� ĳ���͸� �����ϰ� �� ����
        for (AProjectKLBPlayerController* PC : PlayerControllers)
        {
            if (PC)
            {
                PC->UnPossess();              // ���� ���� ���� Pawn ����
                PC->Possess(PlayerChar);      // �� ĳ���� ����
                APawn* CurrentPawn = PC->GetPawn();
                if (CurrentPawn)
                {
                    UE_LOG(LogTemp, Log, TEXT("Now possessed pawn: %s"), *CurrentPawn->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Possession failed for %s"), *PlayerChar->GetName());
                }
                PC->StartTurn();              // �� ����
                break;                        // �� ��Ʈ�ѷ��� ���� �����ϸ� ���� ����
            }
        }
    }
    else if (AEnemy* EnemyChar = Cast<AEnemy>(Actor))
    {
        UE_LOG(LogTemp, Log, TEXT("Executing turn for enemy: %s"), *EnemyChar->GetName());
        // �� �ൿ ���� ���� ����
        StartAITurn();
        // ��: AIController�� ���� �ൿ ����
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown actor type in ExecuteTurnForActor: %s"), *Actor->GetName());
    }
}

void UTurnManager::StartTurnLoop()
{
    // ���� ����(��: 0.1��)���� ProcessNextTurn ȣ��
    WorldContext->GetTimerManager().SetTimer(TurnTimerHandle, this, &UTurnManager::ProcessNextTurn, 2.0f, true);
}


void UTurnManager::StartAITurn()
{
    UE_LOG(LogTemp, Log, TEXT("AI Turn Started."));

    // ���� �� �� ���� ����
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


    // AI �� ���� �� ���� �÷��̾� �� ����
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
        // �ε��� ���� �� ��ȿ�� �˻�
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
