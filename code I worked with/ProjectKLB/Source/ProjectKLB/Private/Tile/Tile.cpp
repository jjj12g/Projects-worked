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

	// Ÿ�� �޽� ������Ʈ ���� �� ����
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh;

	// �ؽ�Ʈ ���� ������Ʈ ���� �� ����
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	TextComponent->SetupAttachment(RootComponent);
	TextComponent->SetHorizontalAlignment(EHTA_Center);
	TextComponent->SetVerticalAlignment(EVRTA_TextCenter);
	TextComponent->SetTextRenderColor(FColor::Black);
	TextComponent->SetWorldSize(20.0f); // �ؽ�Ʈ ũ�� ����
	TextComponent->SetRelativeLocation(FVector(0.f, 0.f, 30.f)); // �ؽ�Ʈ ��ġ ����

}




// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	// Ÿ���� ȸ�� (ȸ���� �ʿ��� ���)
	if (IsPointyTop)
	{
		SetActorRotation(FRotator(0, 90, 0)); // ���ϴ� ������ ȸ��
	}

	// �ؽ�Ʈ ������Ʈ
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
	Q = InQ;  // Q ��ǥ ����
	R = InR;  // R ��ǥ ����

	// Ÿ�� �ؽ�Ʈ ������Ʈ
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

// ť�� ��ǥ ���
FString ATile::PrintTileCoordinates() const
{


	return FString::Printf(TEXT("(%d, %d)"), Q, R); // Q�� R�� �ùٸ� ������ ���
}

void ATile::SetOccupyingEnemy(AEnemy* Enemy)
{
	OccupyingEnemy = Enemy;
	bHasEnemy = (Enemy != nullptr);  // bHasEnemy�� ����ȭ
}


void ATile::Highlight(bool bEnable)
{
	if (TileMesh)
	{
		// Ŀ���� ���� Ȱ��ȭ/��Ȱ��ȭ
		TileMesh->SetRenderCustomDepth(bEnable);

		// ��Ƽ���� ����
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
	return OccupyingEnemy; // OccupyingActor�� Ÿ�Ͽ� �ִ� Actor
}
