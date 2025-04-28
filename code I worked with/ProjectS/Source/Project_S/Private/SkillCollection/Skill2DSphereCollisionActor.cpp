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

    // ���Ǿ� ������Ʈ�� �����Ͽ� RootComponent�� ����
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;

    // �⺻ �浹 ��������
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionSphere->SetGenerateOverlapEvents(true);
}

void ASkill2DSphereCollisionActor::InitializeCollision(const FSkillData& InSkillData)
{
    float NewRadius = InSkillData.ShapeParam1; // �� ���� 300�̾�� ��.
    CollisionSphere->InitSphereRadius(NewRadius);
    UE_LOG(LogTemp, Log, TEXT("Initialized Sphere Radius: %f"), NewRadius);
}

void ASkill2DSphereCollisionActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    FVector Apex = GetActorLocation();

    if (bDebugSphere)
    {
        DrawCollisionDebug();
        bDebugSphere = false;
    }

    // ��ų�� 2D �ݰ� (CollisionSphere->GetScaledSphereRadius() ���)
    float MySkillRadius2D = CollisionSphere->GetScaledSphereRadius();

    // ���忡�� �÷��̾�(ABaseCharacters)�� ���� ã�ƿ�
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundPlayers);

    // 3) �� �÷��̾�� 2D �Ÿ� ��
    for (AActor* Actor : FoundPlayers)
    {
        ABaseCharacters* Player = Cast<ABaseCharacters>(Actor);
        if (!Player) continue;

        // ��ų �ڽ��� ��ġ(X,Y) vs �÷��̾� ��ġ(X,Y)
        FVector MyLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();

        // Z�� ���̸� ���
        float ZDiff = FMath::Abs(MyLocation.Z - PlayerLocation.Z);
        // Z�� ���̰� 300 �̻��̸� �浹 ������ ��������.
        if (ZDiff >= 300.f)
        {
            ProcessedOverlaps.Remove(Player);
            continue;
        }

        FVector2D My2D(MyLocation.X, MyLocation.Y);
        FVector2D Player2D(PlayerLocation.X, PlayerLocation.Y);

        float Distance2D = FVector2D::Distance(My2D, Player2D);

        // �÷��̾��� 2D �浹 �ݰ�
        float PlayerRadius2D = Player->CollisionRadius2D;

        float CombinedRadius = MySkillRadius2D + PlayerRadius2D;

        // 2D �浹 ����
        if (Distance2D <= CombinedRadius)
        {
            // ���� ó������ ���� �÷��̾��� ó��
            if (!ProcessedOverlaps.Contains(Player))
            {
                ProcessedOverlaps.Add(Player);
                
          // �浹 ó�� 
          UE_LOG(LogTemp, Warning, TEXT("ASkill2DSphereCollisionActor: 2D Overlap with %s (ZDiff=%.1f)"), *Player->GetName(), ZDiff);
          // �� ������ �߰� ������ �浹 �������� ����
            FVector CollisionPoint = (MyLocation + PlayerLocation) * 0.5f;


            if (!SkillData.ImpactEffectAsset.IsNull())
            {
                UParticleSystem* PS = SkillData.ImpactEffectAsset.LoadSynchronous();
                if (PS)
                {
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS, GetActorLocation());
                }
            }

            // �浹 ������ ����� ��ü ǥ��
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

    
}

void ASkill2DSphereCollisionActor::DrawCollisionDebug()
{
    bDebugSphere = true;

    if (CollisionSphere)
    {
        DrawDebugSphere(GetWorld(),
            CollisionSphere->GetComponentLocation(),
            CollisionSphere->GetScaledSphereRadius(),
            32,
            FColor::Green,
            false,      
            3.f,        
            0,
            2.f);
    }
}
