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

	// GridManagerClass �ʱ�ȭ
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

    // 1. GridManager ����
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

    // 2. TurnManager �ʱ�ȭ (�ʿ� ��)
    TurnManagerInstance = NewObject<UTurnManager>(this);
    if (TurnManagerInstance)
    {
        TurnManagerInstance->Initialize(World);
        TurnManagerInstance->StartPlayerTurn();

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [this]()
            {
                TurnManagerInstance->InitializeTurnQueue();
                TurnManagerInstance->ProcessNextTurn();  // ù �� ����
            }, 2.0f, false);
    }

    // 3. ���� �ٸ� �÷��̾� ĳ���� Ŭ������ �迭�� �غ��մϴ�.
    //TArray<TSubclassOf<AProjectKLBCharacter>> PlayerCharacterClasses;
    // ���� ���, ���� ĳ���Ϳ� �䳢 ĳ���� Ŭ������ ���� �Ʒ��� ���� ���ǵǾ� �ִٰ� �����մϴ�.
    PlayerCharacterClasses.Add(ACombatWolfChar::StaticClass());
    //PlayerCharacterClasses.Add(ACombatRabbitChar::StaticClass());
    // �߰� ĳ���Ͱ� �ִٸ� �� �߰��մϴ�.

    // 4. �÷��̾� ���� ��ǥ �迭 (Ÿ�� ��ǥ�� �̸� ����)
    TArray<FIntPoint> PlayerSpawnCoordinates;
    PlayerSpawnCoordinates.Add(FIntPoint(0, 0));  // �÷��̾� 0�� ���� ��ǥ
    //PlayerSpawnCoordinates.Add(FIntPoint(0, 1));  // �÷��̾� 1�� ���� ��ǥ
    // �迭�� ũ��� PlayerCharacterClasses�� ����� �մϴ�.

    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // 5. �� ĳ���� Ŭ������ ��ȸ�ϸ� ����
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

        // Ÿ���� ��ġ�� ������� SpawnLocation ��� (Z ������ ����)
        FVector SpawnLocation = TargetTile->GetActorLocation();
        SpawnLocation.Z += 90.0f; // Z ������

        AProjectKLBCharacter* SpawnedPlayer = World->SpawnActor<AProjectKLBCharacter>(PlayerCharacterClasses[i], SpawnLocation, SpawnRotation, SpawnParams);
      
        if (SpawnedPlayer)
        {
            // ù ��° �÷��̾�� PlayerController�� Possess�ϵ��� �մϴ�.
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
            // TurnManager�� ���
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

// GridManager ������ ��ȯ
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