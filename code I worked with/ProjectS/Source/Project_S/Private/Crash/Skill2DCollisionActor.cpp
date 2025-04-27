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
	
    // BeginPlay에서 데이터 테이블의 데이터를 가지고 충돌 컴포넌트를 초기화
    InitializeCollision(SkillData);

    // 데이터 테이블에 지정된 EffectAsset 로드 & 스폰
    if (!SkillData.EffectAsset.IsNull())
    {
        UParticleSystem* PS = SkillData.EffectAsset.LoadSynchronous();
        if (PS)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS, GetActorLocation());
        }
    }

 

    // 수명 설정 등...
    //SetLifeSpan(SkillData.Cooldown);

	// 데이터 테이블의 LifeSpan 값 사용: 예를 들어 5초라면 데이터 테이블에 5 입력
	//SetLifeSpan(SkillData.LifeSpan);

	//UE_LOG(LogTemp, Warning, TEXT("ASkill2DCollisionActor spawned and will be destroyed in %f seconds."), SkillData.LifeSpan);
}





// Called every frame
void ASkill2DCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 Tick마다 디버그 시각화 호출
	//DrawCollisionDebug();

  
}
