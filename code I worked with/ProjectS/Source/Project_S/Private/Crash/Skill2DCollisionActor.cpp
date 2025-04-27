// Fill out your copyright notice in the Description page of Project Settings.


#include "Crash/Skill2DCollisionActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Character/BaseCharacters.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
//#include "Particles/ParticleSystemComponent.h"  
#include "Animation/AnimMontage.h"



// Sets default values
ASkill2DCollisionActor::ASkill2DCollisionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ASkill2DCollisionActor::BeginPlay()
{
	Super::BeginPlay();
	
    // BeginPlay���� ������ ���̺��� �����͸� ������ �浹 ������Ʈ�� �ʱ�ȭ
    InitializeCollision(SkillData);

    // ������ ���̺� ������ EffectAsset �ε� & ����
    if (!SkillData.EffectAsset.IsNull())
    {
        UParticleSystem* PS = SkillData.EffectAsset.LoadSynchronous();
        if (PS)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS, GetActorLocation());
        }
    }

 

    // ���� ���� ��...
    //SetLifeSpan(SkillData.Cooldown);

	// ������ ���̺��� LifeSpan �� ���: ���� ��� 5�ʶ�� ������ ���̺� 5 �Է�
	//SetLifeSpan(SkillData.LifeSpan);

	//UE_LOG(LogTemp, Warning, TEXT("ASkill2DCollisionActor spawned and will be destroyed in %f seconds."), SkillData.LifeSpan);
}





// Called every frame
void ASkill2DCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �� Tick���� ����� �ð�ȭ ȣ��
	//DrawCollisionDebug();

  
}
