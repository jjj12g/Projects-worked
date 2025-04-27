// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollection/Skill2DSphereCollisionActor.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Character/BaseCharacters.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"     


ASkill2DSphereCollisionActor::ASkill2DSphereCollisionActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 스피어 컴포넌트를 생성하여 RootComponent로 설정
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;

    // 기본 충돌 프로파일
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionSphere->SetGenerateOverlapEvents(true);
}

void ASkill2DSphereCollisionActor::InitializeCollision(const FSkillData& InSkillData)
{
    // 데이터 테이블의 Size 값을 반지름으로 설정
    //UE_LOG(LogTemp, Warning, TEXT("SkillData Size: %f"), SkillData.Size);

    float NewRadius = InSkillData.ShapeParam1; // 이 값이 300이어야 함.
    CollisionSphere->InitSphereRadius(NewRadius);
    UE_LOG(LogTemp, Log, TEXT("Initialized Sphere Radius: %f"), NewRadius);
}

void ASkill2DSphereCollisionActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    FVector Apex = GetActorLocation();

    if (bs)
    {
        DrawCollisionDebug();
        bs = false;
    }

    // 1) 스킬의 2D 반경 (CollisionSphere->GetScaledSphereRadius() 사용)
    float MySkillRadius2D = CollisionSphere->GetScaledSphereRadius();

    // 2) 월드에서 플레이어(ABaseCharacters)를 전부 찾아옴
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundPlayers);

    // 3) 각 플레이어와 2D 거리 비교
    for (AActor* Actor : FoundPlayers)
    {
        ABaseCharacters* Player = Cast<ABaseCharacters>(Actor);
        if (!Player) continue;

        // 스킬 자신의 위치(X,Y) vs 플레이어 위치(X,Y)
        FVector MyLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();

        // Z축 차이를 계산합니다.
        float ZDiff = FMath::Abs(MyLocation.Z - PlayerLocation.Z);
        // Z축 차이가 100 이상이면 충돌 판정을 하지 않습니다.
        if (ZDiff >= 300.f)
        {
            ProcessedOverlaps.Remove(Player);
            continue;
        }

        FVector2D My2D(MyLocation.X, MyLocation.Y);
        FVector2D Player2D(PlayerLocation.X, PlayerLocation.Y);

        float Distance2D = FVector2D::Distance(My2D, Player2D);

        // 플레이어의 2D 충돌 반경 (직접 만든 변수나 CapsuleComponent에서 가져와도 됨)
        float PlayerRadius2D = Player->CollisionRadius2D;

        float CombinedRadius = MySkillRadius2D + PlayerRadius2D;

        // 4) 2D 충돌 판정
        if (Distance2D <= CombinedRadius)
        {
            // 아직 처리되지 않은 플레이어라면 처리
            if (!ProcessedOverlaps.Contains(Player))
            {
                ProcessedOverlaps.Add(Player);
                
          // 여기서 충돌 처리 (데미지 적용, 사운드 재생 등)
          UE_LOG(LogTemp, Warning, TEXT("ASkill2DSphereCollisionActor: 2D Overlap with %s (ZDiff=%.1f)"), *Player->GetName(), ZDiff);
          // 두 액터의 중간 지점을 충돌 지점으로 가정
            FVector CollisionPoint = (MyLocation + PlayerLocation) * 0.5f;


            if (!SkillData.ImpactEffectAsset.IsNull())
            {
                UParticleSystem* PS = SkillData.ImpactEffectAsset.LoadSynchronous();
                if (PS)
                {
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS, GetActorLocation());
                }
            }

            // 충돌 지점에 디버그 구체 표시 (빨간색, 반지름 15)
            DrawDebugSphere(
                GetWorld(),
                CollisionPoint,
                15.f,
                16,
                FColor::Red,
                false,
                2.f,  // 2초 동안 표시
                0,
                2.f
            );
            }
        }
    }

    
}

void ASkill2DSphereCollisionActor::DrawCollisionDebug()
{
    bs = true;
    // 데이터 테이블의 Size 값을 반지름으로 적용
 
    if (CollisionSphere)
    {
        DrawDebugSphere(GetWorld(),
            CollisionSphere->GetComponentLocation(),
            CollisionSphere->GetScaledSphereRadius(),
            32,
            FColor::Green,
            false,       // persistent: true로 하면 계속 남아 있음
            3.f,        // 5초 동안 표시 (persistent일 경우에도 시간 지정 가능)
            0,
            2.f);
    }
}
