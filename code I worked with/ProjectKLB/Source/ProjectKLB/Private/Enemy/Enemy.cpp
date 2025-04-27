// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKLBCharacter.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataGameInstance.h"
#include "Components/WidgetComponent.h"
#include "Widget/HealthBarWidget.h"
#include "Tile/Tile.h"
#include "Tile/GridManager.h"
#include "Enemy/EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Enemy/EnemySpawner.h"



// ���ʹ� �ִϸ��̼� ���߿� ��Ƽ���� �� ������ ó���ϴ��� �����ø����� �޾Ƽ� ó���ϱ�

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 3D ���� ������Ʈ ����
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
    HealthWidgetComponent->SetupAttachment(RootComponent);
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 3D�� ���̰� ����
    HealthWidgetComponent->SetDrawSize(FVector2D(150.0f, 15.0f)); // ���� ũ�� ����

    // �̵� ���⿡ ���� �ڵ� ȸ�� ��Ȱ��ȭ
    //GetCharacterMovement()->bOrientRotationToMovement = false;
    //bUseControllerRotationYaw = false; // AIController�� ȸ���� ����

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (EnemyName.IsNone())
    {
        EnemyName = TEXT("Enemy"); // �⺻ �̸� ����
    }

    InitializeStats();

    MaxHP = CharacterStats.Health;
    CurrentHP = MaxHP;

    // ���� Ŭ���� Ȯ�� �� �ʱ�ȭ
    if (HealthWidgetComponent)
    {
        UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetWidget());
        if (HealthWidget)
        {
            // �ʱ� ü�� ����
            HealthWidget->UpdateHealthBar(CurrentHP / MaxHP);
        }
    }

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AEnemy::LookAtPlayer()
{
    if (TargetPlayer)
    {
        // �÷��̾�� �� ������ ���� ���
        FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

        // Yaw ȸ���� ���� (Pitch, Roll ����)
        LookAtRotation.Pitch = 0.f;
        LookAtRotation.Roll = 0.f;

        // ������ ȸ�� (���� ����)
        FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, GetWorld()->DeltaTimeSeconds, 3.0f);

        // �� ȸ�� ����
        //SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtRotation, GetWorld()->DeltaTimeSeconds, 10.0f));

        UE_LOG(LogTemp, Log, TEXT("Enemy is looking at player: %s"), *TargetPlayer->GetName());
    }
}



void AEnemy::Damage(float DamageAmount)
{
	Health -= DamageAmount;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health);

	// ü���� 0 ������ �� ���� ���δ�.
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy has died!"));

        // ���ʹ� �����ʰ� �ִٸ� ī���� ����
        if (EnemySpawner)
        {
            EnemySpawner->OnEnemyDestroyed();
        }

        CurrentTile->SetOccupyingEnemy(nullptr);

        
        

		Destroy();  // �� ������Ʈ ����
	}




}



UBehaviorTree* AEnemy::GetBehaviorTree() const
{
	return Tree;
}

void AEnemy::SetTargetPlayer(AProjectKLBCharacter* Player)
{
    TargetPlayer = Player;
}

AProjectKLBCharacter* AEnemy::GetTargetPlayer() const
{
    return TargetPlayer;
}

void AEnemy::InitializeStats()
{
    UDataGameInstance* GameInstance = Cast<UDataGameInstance>(GetGameInstance());

    if (GameInstance && GameInstance->CharacterStatsTable)
    {
        static const FString ContextString(TEXT("Character Stats Context"));

        // ������ ���̺��� PlayerName ���� ã�� CurrentStats�� ����
        FCharacterStat* PlayerStats = GameInstance->CharacterStatsTable->FindRow<FCharacterStat>(EnemyName, ContextString);
        if (PlayerStats)
        {
            CharacterStats = *PlayerStats;
            this->EnemyName = EnemyName;
            GameInstance->LoadCharacterStats();
            UE_LOG(LogTemp, Warning, TEXT("Enemy Stats Initialized for %s: Health = %d, Attack = %d"), *EnemyName.ToString(), CharacterStats.Health, CharacterStats.Attack);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Stats not found for PlayerName: %s"), *EnemyName.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterStatsTable is not loaded in GameInstance."));
    }
}

void AEnemy::MoveToTile(ATile* TargetTile)
{
    if (!TargetTile)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTile is null! MoveToTile aborted."));
        return;
    }

    // ��ǥ Ÿ���� �����Ǿ����� Ȯ�� (�÷��̾� �Ǵ� �ٸ� ���ʹ�)
    if (TargetTile->OccupyingEnemy != nullptr && TargetTile->OccupyingEnemy != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTile Q=%d, R=%d is occupied by another enemy!"), TargetTile->Q, TargetTile->R);
        TargetTile = FindAlternativeTile(TargetTile);
        if (!TargetTile)
        {
            UE_LOG(LogTemp, Warning, TEXT("No alternative tile found. Staying in place."));
            return; // �̵� ������ Ÿ���� ������ �������� ����
        }
    }

    if (TargetTile->bHasPlayer) // �÷��̾ �ִ��� Ȯ��
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTile Q=%d, R=%d is occupied by the player!"), TargetTile->Q, TargetTile->R);
        TargetTile = FindAlternativeTile(TargetTile); // �ٸ� Ÿ�� ã��
        if (!TargetTile)
        {
            UE_LOG(LogTemp, Warning, TEXT("No alternative tile found to avoid player."));
            return; // ��ü Ÿ���� ������ �̵����� ����
        }
    }

    // ���� Ÿ�� ���� ���� ����
    if (CurrentTile)
    {
        CurrentTile->SetOccupyingEnemy(nullptr);
    }

    // ���ο� Ÿ�� ���� ���� ����
    TargetTile->SetOccupyingEnemy(this);
    CurrentTile = TargetTile;

    // AIController�� ���� Ÿ�Ϸ� �̵�
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        // MoveToActor�� ����Ͽ� �ڿ������� �̵�
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalActor(TargetTile);
        MoveRequest.SetAcceptanceRadius(5.0f); // ��� ���� �Ÿ� ����
        MoveRequest.SetReachTestIncludesAgentRadius(true);
        MoveRequest.SetCanStrafe(false);

        FNavPathSharedPtr Path;
        EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &Path);

        if (MoveResult == EPathFollowingRequestResult::Failed)
        {
            UE_LOG(LogTemp, Warning, TEXT("MoveTo failed for Tile: Q=%d, R=%d"), TargetTile->Q, TargetTile->R);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Moving to Tile: Q=%d, R=%d"), TargetTile->Q, TargetTile->R);
        }
    }
    else
    {
        // AIController�� ���� ���, �����̵����� ��ü
        SetActorLocation(TargetTile->GetActorLocation());
        UE_LOG(LogTemp, Warning, TEXT("AIController not found. Teleporting to Tile: Q=%d, R=%d"), TargetTile->Q, TargetTile->R);
    }
}

ATile* AEnemy::FindAlternativeTile(ATile* OccupiedTile)
{
    if (!CurrentTile || !GridManager)
    {
        return nullptr;
    }

    // ���� Ÿ���� �̿� Ÿ�� ��������
    TArray<ATile*> NeighborTiles = GridManager->GetNeighboringTiles(CurrentTile);

    for (ATile* Tile : NeighborTiles)
    {
        if (Tile && Tile->IsWalkable() && Tile->OccupyingEnemy == nullptr)
        {
            UE_LOG(LogTemp, Log, TEXT("Found alternative tile: Q=%d, R=%d"), Tile->Q, Tile->R);
            return Tile;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No alternative tile found for Tile: Q=%d, R=%d"), OccupiedTile->Q, OccupiedTile->R);
    return nullptr;
}

void AEnemy::PlayAttackAnimation()
{
    if (AttackMontage)
    {
        // �ִϸ��̼� ���
        PlayAnimMontage(AttackMontage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackMontage is not set for this character!"));
    }
}



// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// ���߿� ��Ƽ���� �� ����
//void AEnemyCharacter::OnAttackHit()
//{
//    if (TargetPlayer)
//    {
//        TargetPlayer->ApplyDamage(DamageAmount);
//        UE_LOG(LogTemp, Log, TEXT("Attack hit! Applied %d damage to %s"), DamageAmount, *TargetPlayer->GetName());
//    }
//}