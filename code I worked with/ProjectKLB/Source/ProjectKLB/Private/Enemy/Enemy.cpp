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



// 에너미 애니메이션 나중에 노티파이 찍어서 데미지 처리하던가 무기컬리전에 달아서 처리하기

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 3D 위젯 컴포넌트 생성
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
    HealthWidgetComponent->SetupAttachment(RootComponent);
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 3D로 보이게 설정
    HealthWidgetComponent->SetDrawSize(FVector2D(150.0f, 15.0f)); // 위젯 크기 설정

    // 이동 방향에 따른 자동 회전 비활성화
    //GetCharacterMovement()->bOrientRotationToMovement = false;
    //bUseControllerRotationYaw = false; // AIController의 회전도 무시

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (EnemyName.IsNone())
    {
        EnemyName = TEXT("Enemy"); // 기본 이름 설정
    }

    InitializeStats();

    MaxHP = CharacterStats.Health;
    CurrentHP = MaxHP;

    // 위젯 클래스 확인 및 초기화
    if (HealthWidgetComponent)
    {
        UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetWidget());
        if (HealthWidget)
        {
            // 초기 체력 설정
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
        // 플레이어와 적 사이의 방향 계산
        FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

        // Yaw 회전만 변경 (Pitch, Roll 제외)
        LookAtRotation.Pitch = 0.f;
        LookAtRotation.Roll = 0.f;

        // 스무스 회전 (선택 사항)
        FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, GetWorld()->DeltaTimeSeconds, 3.0f);

        // 적 회전 설정
        //SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtRotation, GetWorld()->DeltaTimeSeconds, 10.0f));

        UE_LOG(LogTemp, Log, TEXT("Enemy is looking at player: %s"), *TargetPlayer->GetName());
    }
}



void AEnemy::Damage(float DamageAmount)
{
	Health -= DamageAmount;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health);

	// 체력이 0 이하일 때 적을 죽인다.
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy has died!"));

        // 에너미 스포너가 있다면 카운터 감소
        if (EnemySpawner)
        {
            EnemySpawner->OnEnemyDestroyed();
        }

        CurrentTile->SetOccupyingEnemy(nullptr);

        
        

		Destroy();  // 적 오브젝트 제거
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

        // 데이터 테이블에서 PlayerName 행을 찾아 CurrentStats에 설정
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

    // 목표 타일이 점유되었는지 확인 (플레이어 또는 다른 에너미)
    if (TargetTile->OccupyingEnemy != nullptr && TargetTile->OccupyingEnemy != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTile Q=%d, R=%d is occupied by another enemy!"), TargetTile->Q, TargetTile->R);
        TargetTile = FindAlternativeTile(TargetTile);
        if (!TargetTile)
        {
            UE_LOG(LogTemp, Warning, TEXT("No alternative tile found. Staying in place."));
            return; // 이동 가능한 타일이 없으면 움직이지 않음
        }
    }

    if (TargetTile->bHasPlayer) // 플레이어가 있는지 확인
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTile Q=%d, R=%d is occupied by the player!"), TargetTile->Q, TargetTile->R);
        TargetTile = FindAlternativeTile(TargetTile); // 다른 타일 찾기
        if (!TargetTile)
        {
            UE_LOG(LogTemp, Warning, TEXT("No alternative tile found to avoid player."));
            return; // 대체 타일이 없으면 이동하지 않음
        }
    }

    // 기존 타일 점유 상태 해제
    if (CurrentTile)
    {
        CurrentTile->SetOccupyingEnemy(nullptr);
    }

    // 새로운 타일 점유 상태 설정
    TargetTile->SetOccupyingEnemy(this);
    CurrentTile = TargetTile;

    // AIController를 통해 타일로 이동
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        // MoveToActor를 사용하여 자연스럽게 이동
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalActor(TargetTile);
        MoveRequest.SetAcceptanceRadius(5.0f); // 허용 오차 거리 설정
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
        // AIController가 없을 경우, 순간이동으로 대체
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

    // 현재 타일의 이웃 타일 가져오기
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
        // 애니메이션 재생
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


// 나중에 노티파이 찍어서 실행
//void AEnemyCharacter::OnAttackHit()
//{
//    if (TargetPlayer)
//    {
//        TargetPlayer->ApplyDamage(DamageAmount);
//        UE_LOG(LogTemp, Log, TEXT("Attack hit! Applied %d damage to %s"), DamageAmount, *TargetPlayer->GetName());
//    }
//}