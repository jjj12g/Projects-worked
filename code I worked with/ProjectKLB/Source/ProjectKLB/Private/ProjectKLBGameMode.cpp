// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKLBGameMode.h"
#include "ProjectKLBPlayerController.h"
#include "ProjectKLBCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Turn/TurnManager.h"
#include "Tile/Tile.h"
#include "Player/CombatRabbitChar.h"
#include "Player/CombatWolfChar.h"


AProjectKLBGameMode::AProjectKLBGameMode()
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
	if(PlayerControllerBPClass.Class != NULL)
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

void AProjectKLBGameMode::BeginPlay()
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

    // 3. 서로 다른 플레이어 캐릭터 클래스를 배열로 준비합니다.
    //TArray<TSubclassOf<AProjectKLBCharacter>> PlayerCharacterClasses;
    // 예를 들어, 늑대 캐릭터와 토끼 캐릭터 클래스가 각각 아래와 같이 정의되어 있다고 가정합니다.
    PlayerCharacterClasses.Add(ACombatWolfChar::StaticClass());
    //PlayerCharacterClasses.Add(ACombatRabbitChar::StaticClass());
    // 추가 캐릭터가 있다면 더 추가합니다.

    // 4. 플레이어 스폰 좌표 배열 (타일 좌표를 미리 정의)
    TArray<FIntPoint> PlayerSpawnCoordinates;
    PlayerSpawnCoordinates.Add(FIntPoint(0, 0));  // 플레이어 0번 스폰 좌표
    //PlayerSpawnCoordinates.Add(FIntPoint(0, 1));  // 플레이어 1번 스폰 좌표
    // 배열의 크기는 PlayerCharacterClasses와 맞춰야 합니다.

    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // 5. 각 캐릭터 클래스를 순회하며 스폰
    for (int32 i = 0; i < PlayerCharacterClasses.Num(); i++)
    {
        if (i >= PlayerSpawnCoordinates.Num())
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough spawn coordinates for character index %d"), i);
            break;
        }

        FIntPoint Coord = PlayerSpawnCoordinates[i];
        ATile* TargetTile = GridManager->GetTileAt(Coord.X, Coord.Y);
        if (!TargetTile)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid spawn tile at Q=%d, R=%d"), Coord.X, Coord.Y);
            continue;
        }

        // 타일의 위치를 기반으로 SpawnLocation 계산 (Z 오프셋 적용)
        FVector SpawnLocation = TargetTile->GetActorLocation();
        SpawnLocation.Z += 90.0f; // Z 오프셋

        AProjectKLBCharacter* SpawnedPlayer = World->SpawnActor<AProjectKLBCharacter>(PlayerCharacterClasses[i], SpawnLocation, SpawnRotation, SpawnParams);
      
        if (SpawnedPlayer)
        {
            // 첫 번째 플레이어는 PlayerController가 Possess하도록 합니다.
            if (i == 0)
            {
                APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
                if (PC)
                {
                    PC->Possess(SpawnedPlayer);
                    UE_LOG(LogTemp, Log, TEXT("PlayerController possessed spawned Pawn: %s"), *SpawnedPlayer->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
                }
            }

            UE_LOG(LogTemp, Log, TEXT("Player %d spawned at Q=%d, R=%d, Location=%s"),
                i, Coord.X, Coord.Y, *SpawnLocation.ToString());
            // TurnManager에 등록
            if (TurnManagerInstance)
            {
                TurnManagerInstance->RegisterCharacter(SpawnedPlayer);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to spawn player at Q=%d, R=%d"), Coord.X, Coord.Y);
        }
    }
}

// GridManager 참조를 반환
AGridManager* AProjectKLBGameMode::GetGridManager() const
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