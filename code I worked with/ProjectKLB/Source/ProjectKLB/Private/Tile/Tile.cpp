// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKLBPlayerController.h"
#include "Enemy/Enemy.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 타일 메시 컴포넌트 생성 및 설정
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh;

	// 텍스트 렌더 컴포넌트 생성 및 설정
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	TextComponent->SetupAttachment(RootComponent);
	TextComponent->SetHorizontalAlignment(EHTA_Center);
	TextComponent->SetVerticalAlignment(EVRTA_TextCenter);
	TextComponent->SetTextRenderColor(FColor::Black);
	TextComponent->SetWorldSize(20.0f); // 텍스트 크기 설정
	TextComponent->SetRelativeLocation(FVector(0.f, 0.f, 30.f)); // 텍스트 위치 조정

}




// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	// 타일의 회전 (회전이 필요할 경우)
	if (IsPointyTop)
	{
		SetActorRotation(FRotator(0, 90, 0)); // 원하는 각도로 회전
	}

	// 텍스트 업데이트
	//UpdateTileText();

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	OnClicked.AddDynamic(this, &ATile::OnTileClicked);



}

void ATile::UpdateTileText()
{
	if (TextComponent)
	{
		TextComponent->SetText(FText::FromString(PrintTileCoordinates()));
	}
}


// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::InitializeTile(int32 InQ, int32 InR, bool bWalkable)
{
	Q = InQ;  // Q 좌표 설정
	R = InR;  // R 좌표 설정

	// 타일 텍스트 업데이트
	UpdateTileText();

	bIsWalkable = bWalkable;

	//UE_LOG(LogTemp, Warning, TEXT("Tile initialized: Q = %d, R = %d, Walkable = %s"),
		//Q, R, bIsWalkable ? TEXT("True") : TEXT("False"));
}

void ATile::OnTileClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	AProjectKLBPlayerController* PlayerController = Cast<AProjectKLBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->HandleTileClick(this);
	}
}

// 큐브 좌표 출력
FString ATile::PrintTileCoordinates() const
{


	return FString::Printf(TEXT("(%d, %d)"), Q, R); // Q와 R을 올바른 순서로 출력
}

void ATile::SetOccupyingEnemy(AEnemy* Enemy)
{
	OccupyingEnemy = Enemy;
	bHasEnemy = (Enemy != nullptr);  // bHasEnemy와 동기화
}


void ATile::Highlight(bool bEnable)
{
	if (TileMesh)
	{
		// 커스텀 뎁스 활성화/비활성화
		TileMesh->SetRenderCustomDepth(bEnable);

		// 머티리얼 변경
		if (bEnable && HighlightMaterial)
		{
			TileMesh->SetMaterial(0, HighlightMaterial);
		}
		else if (DefaultMaterial)
		{
			TileMesh->SetMaterial(0, DefaultMaterial);
		}
	}
}

void ATile::SetHasPlayer(bool bValue)
{
	bHasPlayer = bValue;
	if (bValue)
	{
		UE_LOG(LogTemp, Log, TEXT("Tile Q=%d, R=%d now has player."), Q, R);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Tile Q=%d, R=%d no longer has player."), Q, R);
	}
}

AEnemy* ATile::GetOccupyingEnemy()
{
	return OccupyingEnemy; // OccupyingActor는 타일에 있는 Actor
}
