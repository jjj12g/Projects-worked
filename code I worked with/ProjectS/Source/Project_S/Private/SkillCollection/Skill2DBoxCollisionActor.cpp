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

    // �ڽ� ������Ʈ ����
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    // �浹 �������� ����
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionBox->SetGenerateOverlapEvents(true);
}

void ASkill2DBoxCollisionActor::InitializeCollision(const FSkillData& InSkillData)
{
    // �ڽ��� X, Y Ȯ���� InSkillData.Size ������ ����ϰ�, Z�� ���� �� ����
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

    //2D �浹 ������ ���� �ڽ��� ��ġ�� 2D �浹 �ݰ� ���
    FVector MyLocation = GetActorLocation();
    // �ڽ� ������Ʈ�� X-Y ������ ������: �ڽ��� Ȯ�尪�� X, Y ������ 2D ���� ũ��
    FVector BoxExtent = CollisionBox->GetScaledBoxExtent();
    float MyCollisionRadius2D = FVector2D(BoxExtent.X, BoxExtent.Y).Size();

    //���忡�� ABaseCharacters(�÷��̾�) ���͵��� �˻�
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

        //Z�� ���̸� ����ϰ�, 300 �̻� ���̳��� �浹 ó�� �� �� (����, ���߿� �߰��� �ʿ� 2���� ������ ���� �ʵ���)
        float ZDiff = FMath::Abs(MyLocation.Z - PlayerLocation.Z);
        if (ZDiff >= 300.f)
        {
            continue;
        }

        //X-Y ������ �Ÿ� ���
        FVector2D My2D(MyLocation.X, MyLocation.Y);
        FVector2D Player2D(PlayerLocation.X, PlayerLocation.Y);
        float Distance2D = FVector2D::Distance(My2D, Player2D);

        // �÷��̾��� 2D �浹 �ݰ��� �÷��̾� Ŭ������ ���ǵ� CollisionRadius2D�� ���
        float PlayerCollisionRadius2D = Player->CollisionRadius2D;  // �÷��̾� Ŭ������ �� ������ �����ؾ� ��

        float CombinedRadius = MyCollisionRadius2D + PlayerCollisionRadius2D;

        if (Distance2D <= CombinedRadius)
        {
            UE_LOG(LogTemp, Warning, TEXT("ASkill2DBoxCollisionActor: 2D Overlap with %s (ZDiff=%.1f)"), *Player->GetName(), ZDiff);

            // �� ������ �߰� ������ �浹 �������� ���� (�Ǵ� �ٸ� ������� ���)
            FVector CollisionPoint = (MyLocation + PlayerLocation) * 0.5f;

            // �浹 ������ ����� ��ü�� �׷��� Ȯ�� (������, ������ 15)
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
