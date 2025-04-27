// Fill out your copyright notice in the Description page of Project Settings.
// 플레이어가 사격했을때 해당 위치에 폭발 이펙트를 재생한다.
// 최적화를 하기 위해 플레이어가 이 액터가 플레이어가 사격한 위치로 이동해서 이펙트를 재생한다.

#include "BulletFXActor.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABulletFXActor::ABulletFXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp->SetupAttachment(RootComponent);
	particleComp->SetAutoActivate(true);
}


void ABulletFXActor::PlayFX()
{
	particleComp->Activate(true);
}

