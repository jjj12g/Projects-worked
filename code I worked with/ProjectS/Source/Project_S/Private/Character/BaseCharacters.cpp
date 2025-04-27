// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacters.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Schema/ConfigManager.h"
#include "Schema/DataManager.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h" // ����� ����� �Լ� 
#include "SkillCollection/SkillManager.h"
#include "SkillCollection/SkillData.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Engine/GameInstance.h" // GetSubsystem()�� �ʿ�



// Sets default values
ABaseCharacters::ABaseCharacters()
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
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does ĳ���� ���� ȸ���ϵ��� �ҷ��� false
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// ���� ������Ʈ�� �����Ͽ� ĳ���Ϳ� ����
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

	// �⺻ �÷��̾� �̸� ���� (�ʿ信 ���� �ڽ� Ŭ���� �Ǵ� �����Ϳ��� ����)
	PlayerName = FName("Default");


	// �ʱ� ��ǥ ȸ���� �� ���� �ӵ� ����
	TargetCameraRotation = CameraBoom->GetRelativeRotation();
	CameraRotationInterpSpeed = 3.f;


}

// Called when the game starts or when spawned
void ABaseCharacters::BeginPlay()
{
	Super::BeginPlay();
	

	// ���� StatComponent�� �����ϸ�, PlayerName�� RowName���� ���� �� ������ ���̺��� ������ �ε��մϴ�.
	if (StatComponent)
	{
		StatComponent->StatRowName = PlayerName;
		StatComponent->LoadStatsFromDataTable();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StatComponent is not valid on %s"), *GetName());
	}

	UseSkill(3);

	UConfigManager::LoadConfig();
	UDataManager::LoadData();

	//UE_LOG(LogTemp, Warning, TEXT("Start"));
	//UConfigManager::LoadConfig();
	//UDataManager::LoadData();

	//// ���� StatRowName�� ����ִٸ� �⺻�� "Default" ����
	//if (PlayerName.IsNone())
	//{
	//	PlayerName = FName("Default");
	//}

	//UDataGameInstance* GameInstance = Cast<UDataGameInstance>(GetGameInstance());
	//if (GameInstance && GameInstance->CharacterStatsTable)
	//{
	//	static const FString ContextString(TEXT("Character Stats Context"));
	//	FCharacterStat* Row = GameInstance->CharacterStatsTable->FindRow<FCharacterStat>(PlayerName, ContextString);
	//	if (Row)
	//	{
	//		if (StatComponent)
	//		{
	//			StatComponent->InitializeBaseStats(*Row);
	//			UpdatePlayerStatsFromComponent();
	//		}
	//		UE_LOG(LogTemp, Warning, TEXT("Character Stats Initialized for %s: Health=%d, Attack=%d, Speed=%f"),
	//			*PlayerName.ToString(), Row->Health, Row->Attack, Row->Speed);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Stats not found for PlayerName: %s"), *PlayerName.ToString());
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("CharacterStatsTable is not loaded in GameInstance."));
	//}


}

// Called every frame
void ABaseCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!bUsingKeyboardControl)
	{
		// ĳ���ʹ� ��� ȸ���� ���¶�� �����ϰ�, �̸� �������� ī�޶��� ��ǥ ��� ȸ���� ����մϴ�.
		// ��ǥ: ĳ������ ���� ȸ���� Yaw ���� ���󰡵�, ������ ��ġ(-60��)�� ��(0)�� ����
		FRotator DesiredRelativeRotation = FRotator(-60.f, GetActorRotation().Yaw, 0.f);

		// ���� CameraBoom�� ��� ȸ������ DesiredRelativeRotation���� �ε巴�� �����Ͽ� �����մϴ�.
		FRotator CurrentRelativeRotation = CameraBoom->GetRelativeRotation();
		FRotator NewRelativeRotation = FMath::RInterpTo(CurrentRelativeRotation, DesiredRelativeRotation, DeltaTime, CameraRotationInterpSpeed);
		CameraBoom->SetRelativeRotation(NewRelativeRotation);
	}

	// �÷��̾� ĸ�� ������Ʈ ����� �׸���
	//if (GetCapsuleComponent())
	//{
	//	// ĸ�� ������Ʈ�� ���� ������ �������� �ݳ��̸� �����ɴϴ�.
	//	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	//	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//	// ĸ���� ���� ��ġ�� ȸ���� �����ɴϴ�.
	//	FTransform CapsuleTransform = GetCapsuleComponent()->GetComponentTransform();

	//	// DrawDebugCapsule �Լ� ȣ��: 
	//	// - ĸ���� �߽�, �ݳ���, ������, ȸ��(quaternion) 
	//	// - ����� ����, ���� �ð�, �켱����, �� �β� ���� �����մϴ�.
	//	DrawDebugCapsule(
	//		GetWorld(),
	//		CapsuleTransform.GetLocation(),
	//		CapsuleHalfHeight,
	//		CapsuleRadius,
	//		CapsuleTransform.GetRotation(),
	//		FColor::Blue,       // ĸ���� �Ķ������� ǥ��
	//		false,              // �� ������ ���� (false�� �� Tick���� �׷���)
	//		-1.0f,              // ���� �ð� (-1�̸� �� ������ ����)
	//		0,
	//		2.0f              // �� �β�
	//	);
	//}


	CircleDebug();

}

void ABaseCharacters::OnSideRightViewPressed()
{
	bUsingKeyboardControl = false;
	 //���� ī�޶� ���� ��� ȸ���� �������� TargetCameraRotation ������Ʈ (���������� 90�� �߰�)
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.f;
	SetActorRotation(CurrentRotation);
}

void ABaseCharacters::OnSideLeftViewPressed()
{
	bUsingKeyboardControl = false;
	 //���� ī�޶� ���� ��� ȸ���� �������� TargetCameraRotation ������Ʈ (���������� 90�� �߰�)
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw -= 90.f;
	SetActorRotation(CurrentRotation);
}

void ABaseCharacters::UseSkill(int32 SkillID)
{
	// GameInstance���� USkillManager ����ý����� �����ɴϴ�.
	USkillManager* SkillManager = GetGameInstance()->GetSubsystem<USkillManager>();
	if (!SkillManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UseSkill: SkillManager is null."));
		return;
	}

	// SkillManager���� CachedSkillData�� �ش� ��ų ID�� �ִ��� ã���ϴ�.
	FSkillData* SkillData = SkillManager->CachedSkillData.Find(SkillID);
	if (!SkillData)
	{
		UE_LOG(LogTemp, Error, TEXT("UseSkill: SkillID %d not found."), SkillID);
		return;
	}


	// �ִϸ��̼�
	if (!SkillData->AnimationMontage.IsNull())
	{
		// ���� �ε�
		UAnimMontage* Montage = SkillData->AnimationMontage.LoadSynchronous();
		if (Montage)
		{
			// ĳ������ �޽� ������Ʈ���� AnimInstance ��������
			if (USkeletalMeshComponent* MeshComp = GetMesh())
			{
				UE_LOG(LogTemp, Warning, TEXT("1"));
				if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
				{
					UE_LOG(LogTemp, Warning, TEXT("2"));
					AnimInst->Montage_Play(Montage);
				}
			}
		}
	}

	// 2) SpawnDelay ��ŭ ��ٸ� �� �浹 �ݸ��� ����
	float Delay = SkillData->SpawnDelay;  // DataTable���� �о�� ��
	if (Delay <= 0.f)
	{
		// ��� ����
		SpawnSkillCollision(SkillID);
	}
	else
	{
		FTimerHandle Handle;
		// ���ٷ� �����ϰ� ���ε�
		GetWorldTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateUObject(this, &ABaseCharacters::SpawnSkillCollision, SkillID),
			Delay,
			false  // �� ����
		);
	}


//
//	// ���� ��ġ �� ȸ�� ����
//// ���� ���, ĳ���� ���� 100 ���� ��ġ�� �����ϵ��� ��.
//	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
//	FRotator SpawnRotation = GetActorRotation();
//	FTransform SpawnTransform;
//	SpawnTransform.SetLocation(SpawnLocation);
//	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
//	UE_LOG(LogTemp, Warning, TEXT("SpawnTransform Location: %s"), *SpawnTransform.GetLocation().ToString());
//	// USkillManager�� SpawnSkillCollisionActor �Լ��� ȣ���ؼ� ��ų �浹 ���͸� �����մϴ�.
//	ASkill2DCollisionActor* SpawnedSkillActor = SkillManager->SpawnSkillCollisionActor(*SkillData, SpawnTransform);
//
//	if (SpawnedSkillActor)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Spawned Actor Location: %s"), *SpawnedSkillActor->GetActorLocation().ToString());
//		UE_LOG(LogTemp, Log, TEXT("UseSkill: Spawned Skill Actor for SkillID %d"), SkillID);
//	}


}

void ABaseCharacters::CircleDebug()
{
	// �÷��̾� ĸ�� ��ġ (�߽�)
	FVector Center = (GetCapsuleComponent()) ? GetCapsuleComponent()->GetComponentLocation() : GetActorLocation();

	// ĸ���� ���� ���̸� ����
	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Z���� ĸ�� �Ʒ���(�� ��ġ)�� �̵�
	// ��: �߹ٴ� ��ó�� ���� �׸�����, �ݳ��̸�ŭ ������
	Center.Z -= HalfHeight;

	// ���� �׸� ������
	float CircleRadius = CollisionRadius2D;

	// ����� �� �׸��� (�ٴڿ� ��������)
	//  - X���� ���� ���� ����,  Z���� ����(����) ���ͷ� ����
	DrawDebugCircle(
		GetWorld(),
		Center,                   // �ٴ����� ���� ��ġ
		CircleRadius,            // ������
		32,                      // ���׸�Ʈ ��
		FColor::Blue,            // ����
		false,                   // ������ ǥ�� ����
		-1.f,                    // ǥ�� �ð� (-1�̸� �� ������ / persistent)
		0,                       // �켱����
		2.f,                     // �� �β�
		FVector(1.f, 0.f, 0.f),  // ���� ���� (X��)
		FVector(0.f, 1.f, 0.f),  // ����(0,1,0) �� XY ��鿡 �׸�
		false
	);
}

void ABaseCharacters::SpawnSkillCollision(int32 SkillID)
{
	USkillManager* SkillManager = GetGameInstance()->GetSubsystem<USkillManager>();
	if (!SkillManager) return;

	FSkillData* Data = SkillManager->CachedSkillData.Find(SkillID);
	if (!Data) return;

	// ���� ��ġ/ȸ�� ��� (����)
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * Data->Range + GetActorForwardVector() * 100.f;
	FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

	// ��ų �ݸ��� ���� ����
	ASkill2DCollisionActor* Actor = SkillManager->SpawnSkillCollisionActor(*Data, SpawnTransform);
	if (Actor)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned Skill Collision after %.2f sec"), Data->SpawnDelay);
	}
}





