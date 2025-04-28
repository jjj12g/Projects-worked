// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollection/Skill2DBoxCollisionActor.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Character/BaseCharacters.h"
#include "Kismet/GameplayStatics.h"


ASkill2DBoxCollisionActor::ASkill2DBoxCollisionActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 박스 컴포넌트 생성
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    // 충돌 프로파일 설정
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionBox->SetGenerateOverlapEvents(true);
}

void ASkill2DBoxCollisionActor::InitializeCollision(const FSkillData& InSkillData)
{
    // 박스의 X, Y 확장을 InSkillData.Size 값으로 사용하고, Z는 임의 값 설정
    FVector BoxExtent(InSkillData.ShapeParam1, InSkillData.ShapeParam2, InSkillData.Range);
    CollisionBox->InitBoxExtent(BoxExtent);
}

void ASkill2DBoxCollisionActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bDebugBox)
    {
        DrawCollisionDebug();
        bDebugBox = false;
    }

    //2D 충돌 판정을 위해 자신의 위치와 2D 충돌 반경 계산
    FVector MyLocation = GetActorLocation();
    // 박스 컴포넌트의 X-Y 평면상의 반지름: 박스의 확장값의 X, Y 성분의 2D 벡터 크기
    FVector BoxExtent = CollisionBox->GetScaledBoxExtent();
    float MyCollisionRadius2D = FVector2D(BoxExtent.X, BoxExtent.Y).Size();

    //월드에서 ABaseCharacters(플레이어) 액터들을 검색
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundPlayers);

    for (AActor* Actor : FoundPlayers)
    {
        ABaseCharacters* Player = Cast<ABaseCharacters>(Actor);
        if (!Player)
        {
            continue;
        }

        FVector PlayerLocation = Player->GetActorLocation();

        //Z축 차이를 계산하고, 300 이상 차이나면 충돌 처리 안 함 (만약, 나중에 추가될 맵에 2층이 있으면 맞지 않도록)
        float ZDiff = FMath::Abs(MyLocation.Z - PlayerLocation.Z);
        if (ZDiff >= 300.f)
        {
            continue;
        }

        //X-Y 평면상의 거리 계산
        FVector2D My2D(MyLocation.X, MyLocation.Y);
        FVector2D Player2D(PlayerLocation.X, PlayerLocation.Y);
        float Distance2D = FVector2D::Distance(My2D, Player2D);

        // 플레이어의 2D 충돌 반경은 플레이어 클래스에 정의된 CollisionRadius2D를 사용
        float PlayerCollisionRadius2D = Player->CollisionRadius2D;  // 플레이어 클래스에 이 변수가 존재해야 함

        float CombinedRadius = MyCollisionRadius2D + PlayerCollisionRadius2D;

        if (Distance2D <= CombinedRadius)
        {
            UE_LOG(LogTemp, Warning, TEXT("ASkill2DBoxCollisionActor: 2D Overlap with %s (ZDiff=%.1f)"), *Player->GetName(), ZDiff);

            // 두 액터의 중간 지점을 충돌 지점으로 가정 (또는 다른 방식으로 계산)
            FVector CollisionPoint = (MyLocation + PlayerLocation) * 0.5f;

            // 충돌 지점에 디버그 구체를 그려서 확인 (빨간색, 반지름 15)
            DrawDebugSphere(
                GetWorld(),
                CollisionPoint,
                15.f,
                16,
                FColor::Red,
                false,
                2.f,
                0,
                2.f
            );
        }
    }
}

void ASkill2DBoxCollisionActor::DrawCollisionDebug()
{
    bDebugBox = true;
    if (CollisionBox)
    {
        DrawDebugBox(GetWorld(), CollisionBox->GetComponentLocation(), CollisionBox->GetScaledBoxExtent(), CollisionBox->GetComponentQuat(), FColor::Green, false, 5.f, 0, 2.f);
    }
}
