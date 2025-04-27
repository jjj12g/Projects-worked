// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollection/Skill2DConeCollisionActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Character/BaseCharacters.h"



ASkill2DConeCollisionActor::ASkill2DConeCollisionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // RootComponent 설정 (SceneComponent를 생성하여 할당)
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 기본 값 설정 (필요에 따라 조정)
	ConeRange = 500.f;     // 최대 500cm까지 충돌 판정
	ConeAngle = 60.f;      // 전체 60도 (반각 30도)
	// 기본적으로 액터의 Forward를 사용하되, 나중에 InitializeCollision이나 Tick에서 재설정할 수 있습니다.
	ConeDirection = GetActorForwardVector();

}

void ASkill2DConeCollisionActor::InitializeCollision(const FSkillData& InSkillData)
{
	// 예제에서는 SkillData.Size 값을 ConeRange로 사용하고, 각도는 기본값 유지합니다.
     // 예시: ConeRange는 Range 필드를 사용하고, ConeAngle은 ShapeParam2로 사용합니다.
    ConeRange = InSkillData.Range;                      // 예: 최대 길이
    ConeAngle = InSkillData.ShapeParam2;                // 예: 전체 각도 (도 단위)

    // 필요하다면 ConeDirection도 업데이트 (대부분 액터의 Forward Vector를 사용)
    ConeDirection = GetActorForwardVector();

	UE_LOG(LogTemp, Log, TEXT("Initialized Cone Collision: Range = %f, Angle = %f"), ConeRange, ConeAngle);
}


void ASkill2DConeCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector Apex = GetActorLocation();

    // ConeDirection을 정규화
    FVector NormDir = ConeDirection.GetSafeNormal();

    // 원뿔의 반각 (라디안)
    float HalfAngleRad = FMath::DegreesToRadians(ConeAngle * 0.5f);

    DrawDebugCone(
        GetWorld(),
        Apex,
        NormDir,
        ConeRange,
        HalfAngleRad,
        HalfAngleRad,
        16,
        FColor::Yellow,
        false,      // PersistentLines: true
        5.f,       // Duration: 5초 동안 표시
        0,
        2.f       // 선 두께
    );
    
    //DrawDebugPoint(GetWorld(), Apex, 10.f, FColor::Red, true, 5.f);

    // 이제 바닥 기준 2D 충돌 판정을 실시합니다.
    // 여기서는 원뿔의 방향을 ConeDirection (이미 설정됨)으로 사용합니다.
    // 플레이어(ABaseCharacters)를 찾습니다.
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundPlayers);

    for (AActor* Actor : FoundPlayers)
    {
        ABaseCharacters* Player = Cast<ABaseCharacters>(Actor);
        if (!Player)
            continue;
        //UE_LOG(LogTemp, Warning, TEXT("1"));
        FVector TargetLocation = Player->GetActorLocation();

        // 먼저, Z축 차이를 확인하여 너무 높거나 낮으면 무시
        float ZDiff = FMath::Abs(Apex.Z - TargetLocation.Z);
        if (ZDiff >= 300.f)
            continue;
        //UE_LOG(LogTemp, Warning, TEXT("2"));
        // X-Y 평면 기준 거리(예: 플레이어 중심과 원뿔의 apex 사이의 2D 거리)
        FVector2D Apex2D(Apex.X, Apex.Y);
        FVector2D Target2D(TargetLocation.X, TargetLocation.Y);
        float Distance2D = FVector2D::Distance(Apex2D, Target2D);

        // 만약 2D 거리보다 Target이 가까우면서, 원뿔 방향과 타겟으로 향하는 벡터의 각도를 체크합니다.
        if (Distance2D > ConeRange)
            continue; // 범위 밖
        //UE_LOG(LogTemp, Warning, TEXT("3"));
        // 원뿔 중심 방향과 플레이어까지의 벡터를 각각 2D (혹은 전체 3D)에서 비교합니다.
        FVector ToTarget = (TargetLocation - Apex).GetSafeNormal();
        float DotVal = FVector::DotProduct(ConeDirection.GetSafeNormal(), ToTarget);
        // 각도를 비교: if cos(halfAngle) <= DotVal then Target은 원뿔 내부.
        float MinDot = FMath::Cos(FMath::DegreesToRadians(ConeAngle * 0.5f));
        if (DotVal >= MinDot)
        {
            // 충돌 판정: 원뿔 내부에 대상이 있음.
            UE_LOG(LogTemp, Warning, TEXT("ASkill2DConeCollisionActor: 2D Cone Overlap with %s (Distance2D=%.1f, Dot=%f, ZDiff=%.1f)"),
                *Player->GetName(), Distance2D, DotVal, ZDiff);

            // 중간 지점 또는 플레이어 위치에 디버그 구체 표시
            DrawDebugSphere(GetWorld(), TargetLocation, 15.f, 16, FColor::Red, false, 2.f, 0, 2.f);
            //UE_LOG(LogTemp, Warning, TEXT("4"));
        }
    }

}


void ASkill2DConeCollisionActor::DrawCollisionDebug()
{
    //Bso = true;
    // DrawDebugCone는 원뿔의 apex, 방향, 길이, 반각(라디안) 등으로 원뿔을 그립니다.
    //FVector Apex = GetActorLocation(); // 원뿔의 시작점(액터 위치)
    //float HalfAngleRad = FMath::DegreesToRadians(ConeAngle * 0.5f);

    //DrawDebugCone(
    //    GetWorld(),
    //    Apex,                          // 원뿔 시작점
    //    ConeDirection.GetSafeNormal(), // 원뿔의 중심 방향
    //    ConeRange,                     // 원뿔 길이 (최대 거리)
    //    HalfAngleRad,                  // 반각 (라디안)
    //    HalfAngleRad,                  // 수평 반각 (동일하게)
    //    16,                            // 분할 수 (세그먼트)
    //    FColor::Yellow,                // 색상
    //    true,                         // 지속적 그리기
    //    -1.f,                          // 지속 시간
    //    0,
    //    2.f                           // 선 두께
    //);

    //UE_LOG(LogTemp, Warning, TEXT("Cone Draw"));
}

