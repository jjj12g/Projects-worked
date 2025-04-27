// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageGameModeBase.h"
#include "ProjectKLBPlayerController.h"
#include "ProjectKLBCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Turn/TurnManager.h"
#include "Tile/Tile.h"
#include "Player/CombatRabbitChar.h"
#include "Player/CombatWolfChar.h"
#include "Widget/PlacementWidget.h"


AStageGameModeBase::AStageGameModeBase()
{
    // use our custom PlayerController class
    PlayerControllerClass = AProjectKLBPlayerController::StaticClass();

    DefaultPawnClass = nullptr;

    //// set default pawn class to our Blueprinted character
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
    //if (PlayerPawnBPClass.Class != nullptr)
    //{
    //	DefaultPawnClass = PlayerPawnBPClass.Class;
    //}

    // set default controller to our Blueprinted controller
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
    if (PlayerControllerBPClass.Class != NULL)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }

    // GridManagerClass 초기화
    static ConstructorHelpers::FClassFinder<AGridManager> GridManagerBPClass(TEXT("/Script/Engine.Blueprint'/Game/test/HexTile/BP_GridManager.BP_GridManager_C'"));
    if (GridManagerBPClass.Class)
    {
        GridManagerClass = GridManagerBPClass.Class;
    }
}

void AStageGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World) return;

    // 1. GridManager 설정
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(World, AGridManager::StaticClass()));
    if (!GridManager)
    {
        GridManager = World->SpawnActor<AGridManager>(GridManagerClass);
        if (GridManager)
        {
            UE_LOG(LogTemp, Warning, TEXT("GridManager spawned successfully: %s"), *GridManager->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn GridManager!"));
            return;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Using existing GridManager: %s"), *GridManager->GetName());
    }


    // 배치 UI를 열기
    if (PlacementWidgetClass)  // UPROPERTY(EditAnywhere)로 블루프린트 지정
    {
        PlacementUI = CreateWidget<UPlacementWidget>(GetWorld(), PlacementWidgetClass);
        if (PlacementUI)
        {
            PlacementUI->AddToViewport(0);

            // 필요하면, PlacementUI->AvailableCharacters 에 
            // [늑대, 토끼, ...] 클래스를 채워넣을 수도 있음
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlacementWidgetClass is not set in AStageGameModeBase!"));
    }
}

// GridManager 참조를 반환
AGridManager* AStageGameModeBase::GetGridManager() const
{
    if (GridManager)
    {
        UE_LOG(LogTemp, Log, TEXT("GridManager found in GameMode: %s"), *GridManager->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GridManager is null in GameMode!"));
    }
    return GridManager;
}

void AStageGameModeBase::SpawnCharacterAtTile(TSubclassOf<AProjectKLBCharacter> CharacterClass, ATile* Tile)
{
    if (!Tile || !CharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnCharacterAtTile: Invalid parameters."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnCharacterAtTile: World is null."));
        return;
    }

    FVector TileLocation = Tile->GetActorLocation();
    UE_LOG(LogTemp, Warning, TEXT("Tile location for (Q=%d, R=%d): %s"), Tile->Q, Tile->R, *TileLocation.ToString());

    FVector SpawnLocation = TileLocation;
    SpawnLocation.Z += 90.f;
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    UE_LOG(LogTemp, Warning, TEXT("Spawning %s at %s"), *CharacterClass->GetName(), *SpawnLocation.ToString());

    AProjectKLBCharacter* SpawnedChar = World->SpawnActor<AProjectKLBCharacter>(CharacterClass, SpawnLocation, SpawnRotation, Params);
    if (SpawnedChar)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawned character %s at Tile (Q=%d, R=%d)"),
            *SpawnedChar->GetName(), Tile->Q, Tile->R);

        if (SpawnedCharacters.Num() == 0)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
            if (PC)
            {
                PC->Possess(SpawnedChar);
            }
        }

        if (TurnManagerInstance)
        {
            TurnManagerInstance->RegisterCharacter(SpawnedChar);
        }

        SpawnedCharacters.Add(SpawnedChar);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character at tile (Q=%d, R=%d)"), Tile->Q, Tile->R);
    }
}

void AStageGameModeBase::OnPlacementFinished()
{

    UWorld* World = GetWorld();
    if (!World) return;

    AProjectKLBPlayerController* APC = Cast<AProjectKLBPlayerController>(GetWorld()->GetFirstPlayerController());


    // 2. TurnManager 초기화 (필요 시)
    TurnManagerInstance = NewObject<UTurnManager>(this);
    if (TurnManagerInstance)
    {
        TurnManagerInstance->Initialize(World);
        TurnManagerInstance->StartPlayerTurn();
       
       

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [this]()
            {
                TurnManagerInstance->InitializeTurnQueue();
                TurnManagerInstance->ProcessNextTurn();  // 첫 턴 시작
            }, 2.0f, false);
    }

    UE_LOG(LogTemp, Log, TEXT("Placement finished. Game start logic here."));

    //// 모든 캐릭터가 배치 완료된 시점
 //   // → 이제 TurnManager 시작, 혹은 전투/게임 시작
 //if (TurnManagerInstance)
 //{
 //    TurnManagerInstance->InitializeTurnQueue();
 //    TurnManagerInstance->ProcessNextTurn();
 //}
}
