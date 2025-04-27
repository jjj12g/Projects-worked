// Fill out your copyright notice in the Description page of Project Settings.
//에너미용 탄환 액터

#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "GunPlayer.h"
#include "Enemy/BaseEnemy.h"

// Sets default values
ABulletActor::ABulletActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletBody = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCollision"));
	SetRootComponent(bulletBody);
	bulletBody->SetSphereRadius(50.0f);
	bulletBody->SetRelativeScale3D(FVector(0.05f));

	bulletShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	bulletShape->SetupAttachment(bulletBody);
	bulletShape->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

	particleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle Component"));
	particleComp->SetupAttachment(bulletBody);
	particleComp->SetWorldScale3D(FVector(10.0f));
	particleComp->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	bulletBody->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::CallHit);

	SetLifeSpan(3.0f);

	particleComp->Activate(true);
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime + FVector(2.0f, 0.0f, 0.0f), true);

}

void ABulletActor::CallHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGunPlayer* playerREF = Cast<AGunPlayer>(OtherActor);
	if (playerREF) {
		playerREF->OnDamaged();
		//UE_LOG(LogTemp, Warning, TEXT("HIT!!!"));
		Destroy();
	}
}

