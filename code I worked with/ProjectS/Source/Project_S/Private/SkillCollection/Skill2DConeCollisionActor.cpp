// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollection/Skill2DConeCollisionActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Character/BaseCharacters.h"



ASkill2DConeCollisionActor::ASkill2DConeCollisionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // RootComponent ���� (SceneComponent�� �����Ͽ� �Ҵ�)
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// �⺻ �� ���� (�ʿ信 ���� ����)
	ConeRange = 500.f;     // �ִ� 500cm���� �浹 ����
	ConeAngle = 60.f;      // ��ü 60�� (�ݰ� 30��)
	// �⺻������ ������ Forward�� ����ϵ�, ���߿� InitializeCollision�̳� Tick���� �缳���� �� �ֽ��ϴ�.
	ConeDirection = GetActorForwardVector();

}

void ASkill2DConeCollisionActor::InitializeCollision(const FSkillData& InSkillData)
{
	// ���������� SkillData.Size ���� ConeRange�� ����ϰ�, ������ �⺻�� �����մϴ�.
     // ����: ConeRange�� Range �ʵ带 ����ϰ�, ConeAngle�� ShapeParam2�� ����մϴ�.
    ConeRange = InSkillData.Range;                      // ��: �ִ� ����
    ConeAngle = InSkillData.ShapeParam2;                // ��: ��ü ���� (�� ����)

    // �ʿ��ϴٸ� ConeDirection�� ������Ʈ (��κ� ������ Forward Vector�� ���)
    ConeDirection = GetActorForwardVector();

	UE_LOG(LogTemp, Log, TEXT("Initialized Cone Collision: Range = %f, Angle = %f"), ConeRange, ConeAngle);
}


void ASkill2DConeCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector Apex = GetActorLocation();

    // ConeDirection�� ����ȭ
    FVector NormDir = ConeDirection.GetSafeNormal();

    // ������ �ݰ� (����)
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
        5.f,       // Duration: 5�� ���� ǥ��
        0,
        2.f       // �� �β�
    );
    
    //DrawDebugPoint(GetWorld(), Apex, 10.f, FColor::Red, true, 5.f);

    // ���� �ٴ� ���� 2D �浹 ������ �ǽ��մϴ�.
    // ���⼭�� ������ ������ ConeDirection (�̹� ������)���� ����մϴ�.
    // �÷��̾�(ABaseCharacters)�� ã���ϴ�.
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundPlayers);

    for (AActor* Actor : FoundPlayers)
    {
        ABaseCharacters* Player = Cast<ABaseCharacters>(Actor);
        if (!Player)
            continue;
        //UE_LOG(LogTemp, Warning, TEXT("1"));
        FVector TargetLocation = Player->GetActorLocation();

        // ����, Z�� ���̸� Ȯ���Ͽ� �ʹ� ���ų� ������ ����
        float ZDiff = FMath::Abs(Apex.Z - TargetLocation.Z);
        if (ZDiff >= 300.f)
            continue;
        //UE_LOG(LogTemp, Warning, TEXT("2"));
        // X-Y ��� ���� �Ÿ�(��: �÷��̾� �߽ɰ� ������ apex ������ 2D �Ÿ�)
        FVector2D Apex2D(Apex.X, Apex.Y);
        FVector2D Target2D(TargetLocation.X, TargetLocation.Y);
        float Distance2D = FVector2D::Distance(Apex2D, Target2D);

        // ���� 2D �Ÿ����� Target�� �����鼭, ���� ����� Ÿ������ ���ϴ� ������ ������ üũ�մϴ�.
        if (Distance2D > ConeRange)
            continue; // ���� ��
        //UE_LOG(LogTemp, Warning, TEXT("3"));
        // ���� �߽� ����� �÷��̾������ ���͸� ���� 2D (Ȥ�� ��ü 3D)���� ���մϴ�.
        FVector ToTarget = (TargetLocation - Apex).GetSafeNormal();
        float DotVal = FVector::DotProduct(ConeDirection.GetSafeNormal(), ToTarget);
        // ������ ��: if cos(halfAngle) <= DotVal then Target�� ���� ����.
        float MinDot = FMath::Cos(FMath::DegreesToRadians(ConeAngle * 0.5f));
        if (DotVal >= MinDot)
        {
            // �浹 ����: ���� ���ο� ����� ����.
            UE_LOG(LogTemp, Warning, TEXT("ASkill2DConeCollisionActor: 2D Cone Overlap with %s (Distance2D=%.1f, Dot=%f, ZDiff=%.1f)"),
                *Player->GetName(), Distance2D, DotVal, ZDiff);

            // �߰� ���� �Ǵ� �÷��̾� ��ġ�� ����� ��ü ǥ��
            DrawDebugSphere(GetWorld(), TargetLocation, 15.f, 16, FColor::Red, false, 2.f, 0, 2.f);
            //UE_LOG(LogTemp, Warning, TEXT("4"));
        }
    }

}


void ASkill2DConeCollisionActor::DrawCollisionDebug()
{
    //Bso = true;
    // DrawDebugCone�� ������ apex, ����, ����, �ݰ�(����) ������ ������ �׸��ϴ�.
    //FVector Apex = GetActorLocation(); // ������ ������(���� ��ġ)
    //float HalfAngleRad = FMath::DegreesToRadians(ConeAngle * 0.5f);

    //DrawDebugCone(
    //    GetWorld(),
    //    Apex,                          // ���� ������
    //    ConeDirection.GetSafeNormal(), // ������ �߽� ����
    //    ConeRange,                     // ���� ���� (�ִ� �Ÿ�)
    //    HalfAngleRad,                  // �ݰ� (����)
    //    HalfAngleRad,                  // ���� �ݰ� (�����ϰ�)
    //    16,                            // ���� �� (���׸�Ʈ)
    //    FColor::Yellow,                // ����
    //    true,                         // ������ �׸���
    //    -1.f,                          // ���� �ð�
    //    0,
    //    2.f                           // �� �β�
    //);

    //UE_LOG(LogTemp, Warning, TEXT("Cone Draw"));
}

