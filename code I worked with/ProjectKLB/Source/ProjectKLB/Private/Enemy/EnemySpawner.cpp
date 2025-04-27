// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Tile/GridManager.h"
#include "Tile/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKLBGameMode.h"
#include "Enemy/Enemy.h"
#include "ProjectKLBPlayerController.h"



// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
    // GridManager���� SpawnEnemies�� ȣ���ϵ��� ����
    UE_LOG(LogTemp, Log, TEXT("EnemySpawner initialized but waiting for GridManager to call SpawnEnemies."));
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemies()
{
    if (!GridManager)
    {
        GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
        if (!GridManager)
        {
            UE_LOG(LogTemp, Error, TEXT("GridManager not found in EnemySpawner!"));
            return;
        }
    }

    for (const FIntPoint& Coord : EnemySpawnCoordinates)
    {
        ATile* TargetTile = GridManager->GetTileAt(Coord.X, Coord.Y);
        if (!TargetTile || TargetTile->OccupyingEnemy != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Tile is invalid or occupied at Q=%d, R=%d!"), Coord.X, Coord.Y);
            continue;
        }

        FVector SpawnLocation = TargetTile->GetActorLocation();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        if (SpawnedEnemy)
        {
            AliveEnemyCount++; // ���ʹ� ���� �� ī��Ʈ ����
            SpawnedEnemy->GridManager = GridManager; // GridManager ����

            // SpawnedEnemy�� �� �������� �����͸� �����ϵ��� �� (AEnemy�� ��� ���� ���� �ʿ�)
            SpawnedEnemy->SetEnemySpawner(this);
            SpawnedEnemy->MoveToTile(TargetTile);
            UE_LOG(LogTemp, Log, TEXT("Enemy spawned at Q=%d, R=%d"), Coord.X, Coord.Y);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy at Q=%d, R=%d"), Coord.X, Coord.Y);
        }
    }
}

void AEnemySpawner::OnEnemyDestroyed()
{
    AliveEnemyCount--;
    UE_LOG(LogTemp, Log, TEXT("Enemy destroyed. Remaining count: %d"), AliveEnemyCount);
    if (AliveEnemyCount <= 0)
    {
        // ��� ���ʹ̰� ���ŵǾ��� �� UI�� ǥ���մϴ�.
        ShowAllEnemiesDefeatedUI();
    }

}

void AEnemySpawner::ShowAllEnemiesDefeatedUI()
{
    AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetWorld()->GetFirstPlayerController());
    PC->UpdateTutorialStageClearUI();
    UE_LOG(LogTemp, Log, TEXT("All enemies have been defeated. Showing UI."));
    // ���⼭ UI ������ �����ϰų�, �������Ʈ �̺�Ʈ�� ȣ���Ͽ� UI�� ǥ���մϴ�.
}

