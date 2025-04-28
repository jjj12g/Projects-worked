// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKLBCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "DataGameInstance.h"
#include "Components/WidgetComponent.h"
#include "Widget/HealthBarWidget.h"
#include "Player/CharacterStatComponent.h"
#include "ProjectKLBPlayerController.h"
#include "Kismet/GameplayStatics.h"


AProjectKLBCharacter::AProjectKLBCharacter()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 3D 위젯 컴포넌트 생성
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 3D로 보이게 설정
	HealthWidgetComponent->SetDrawSize(FVector2D(150.0f, 15.0f)); // 위젯 크기 설정



	// 스탯 컴포넌트를 생성하고 캐릭터에 부착
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

	// 기본값을 비워두고, BeginPlay나 자식 클래스에서 설정
	PlayerName = FName("Default");

}

void AProjectKLBCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);



}

void AProjectKLBCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 만약 StatRowName이 비어있다면 기본값 "Default" 설정
	if (PlayerName.IsNone())
	{
		PlayerName = FName("Default");
	}

	InitializeStats();

	MaxHP = StatComponent->FinalStats.Health;
	CurrentHP = MaxHP;

	UpdatePlayerStatsFromComponent();


	UE_LOG(LogTemp, Warning, TEXT("Player BeginPlay: %s, Stats: Health=%d, Attack=%d, Speed=%f"),
		*PlayerName.ToString(),
		StatComponent->FinalStats.Health,
		StatComponent->FinalStats.Attack,
		StatComponent->FinalStats.Speed);

	// 위젯 클래스 확인 및 초기화
	if (HealthWidgetComponent)
	{
		HealthWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetWidget());
		if (HealthWidget)
		{
			// 초기 체력 설정
			HealthWidget->UpdateHealthBar(CurrentHP / MaxHP);
		}
	}


}

void AProjectKLBCharacter::InitializeStats()
{
	UDataGameInstance* GameInstance = Cast<UDataGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->CharacterStatsTable)
	{
		static const FString ContextString(TEXT("Character Stats Context"));
		FCharacterStat* Row = GameInstance->CharacterStatsTable->FindRow<FCharacterStat>(PlayerName, ContextString);
		if (Row)
		{
			if (StatComponent)
			{
				StatComponent->InitializeBaseStats(*Row);
				UpdatePlayerStatsFromComponent();
			}
			UE_LOG(LogTemp, Warning, TEXT("Character Stats Initialized for %s: Health=%d, Attack=%d, Speed=%f"),
				*PlayerName.ToString(), Row->Health, Row->Attack, Row->Speed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Stats not found for PlayerName: %s"), *PlayerName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterStatsTable is not loaded in GameInstance."));
	}
}

void AProjectKLBCharacter::UpdatePlayerStatsFromComponent()
{
	if (StatComponent)
	{
		// 예를 들어, 캐릭터의 CurrentAttack, CurrentDefense 등을 StatComponent->FinalStats에서 가져옴.
		CurrentAttack = StatComponent->FinalStats.Attack;
		CurrentDefense = StatComponent->FinalStats.Defense;
		CurrentHP = StatComponent->FinalStats.Health;
		CurrentSpeed = StatComponent->FinalStats.Speed;

		UE_LOG(LogTemp, Log, TEXT("Player stats updated: Attack=%f, Defense=%f, Health=%f, Speed=%f"),
			CurrentAttack, CurrentDefense, CurrentHP, CurrentSpeed);
	}
}



void AProjectKLBCharacter::ApplyDamage(int32 DamageAmount)
{
	// 체력 감소
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, MaxHP);
	
	UE_LOG(LogTemp, Warning, TEXT("Current: %f"), CurrentHP);

	// 체력이 0보다 작으면 0으로 설정
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		
		AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->Possess(this);
			UE_LOG(LogTemp, Warning, TEXT("ApplyDamage"));
		}
	}

	UpdateHealthBar();
	UE_LOG(LogTemp, Warning, TEXT("Applied %d damage to %s"), DamageAmount, *GetName());

}


void AProjectKLBCharacter::UpdateHealthBar()
{
	if (HealthWidgetComponent)
	{
		HealthWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetWidget());
		if (HealthWidget)
		{
			float HealthPercent = (MaxHP > 0) ? (CurrentHP / MaxHP) : 0.0f;
			HealthWidget->UpdateHealthBar(HealthPercent);
			UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBar: Updated health percent: %f"), HealthPercent);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBar: HealthWidget is null."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBar: HealthWidgetComponent is null."));
	}
}

void AProjectKLBCharacter::EquipItemInSlot(EEquipmentType SlotType, const FItemData& NewItem)
{
	if (StatComponent)
	{
		switch (SlotType)
		{
		case EEquipmentType::Weapon:
			StatComponent->Weapon = NewItem;
			break;
		case EEquipmentType::Helmet:
			StatComponent->Helmet = NewItem;
			break;
		case EEquipmentType::Chest:
			StatComponent->Chest = NewItem;
			break;
		case EEquipmentType::Legs:
			StatComponent->Legs = NewItem;
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("EquipItemInSlot: Unsupported equipment type."));
			break;
		}
		// 장비 아이템이 변경되면 스탯 재계산
		StatComponent->RecalculateStats();

		

		// 장비가 변경되었음을 로그로 출력하거나, 추가적인 처리
		UE_LOG(LogTemp, Log, TEXT("EquipItemInSlot: Equipped %s in slot type %d"), *NewItem.ItemName, (int32)SlotType);
	}
}







