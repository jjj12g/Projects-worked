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
#include "DrawDebugHelpers.h" // 디버그 드로잉 함수 
#include "SkillCollection/SkillManager.h"
#include "SkillCollection/SkillData.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Engine/GameInstance.h" // GetSubsystem()에 필요



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
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does 캐릭터 따라 회전하도록 할려면 false
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 스탯 컴포넌트를 생성하여 캐릭터에 부착
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

	// 기본 플레이어 이름 설정 (필요에 따라 자식 클래스 또는 에디터에서 변경)
	PlayerName = FName("Default");


	// 초기 목표 회전값 및 보간 속도 설정
	TargetCameraRotation = CameraBoom->GetRelativeRotation();
	CameraRotationInterpSpeed = 3.f;


}

// Called when the game starts or when spawned
void ABaseCharacters::BeginPlay()
{
	Super::BeginPlay();
	

	// 만약 StatComponent가 존재하면, PlayerName을 RowName으로 설정 후 데이터 테이블에서 스탯을 로드합니다.
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

	//// 만약 StatRowName이 비어있다면 기본값 "Default" 설정
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
		// 캐릭터는 즉시 회전한 상태라고 가정하고, 이를 기준으로 카메라의 목표 상대 회전을 계산합니다.
		// 목표: 캐릭터의 월드 회전의 Yaw 값을 따라가되, 고정된 피치(-60도)와 롤(0)을 유지
		FRotator DesiredRelativeRotation = FRotator(-60.f, GetActorRotation().Yaw, 0.f);

		// 현재 CameraBoom의 상대 회전에서 DesiredRelativeRotation으로 부드럽게 보간하여 설정합니다.
		FRotator CurrentRelativeRotation = CameraBoom->GetRelativeRotation();
		FRotator NewRelativeRotation = FMath::RInterpTo(CurrentRelativeRotation, DesiredRelativeRotation, DeltaTime, CameraRotationInterpSpeed);
		CameraBoom->SetRelativeRotation(NewRelativeRotation);
	}

	// 플레이어 캡슐 컴포넌트 디버그 그리기
	//if (GetCapsuleComponent())
	//{
	//	// 캡슐 컴포넌트의 실제 스케일 반지름과 반높이를 가져옵니다.
	//	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	//	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//	// 캡슐의 월드 위치와 회전을 가져옵니다.
	//	FTransform CapsuleTransform = GetCapsuleComponent()->GetComponentTransform();

	//	// DrawDebugCapsule 함수 호출: 
	//	// - 캡슐의 중심, 반높이, 반지름, 회전(quaternion) 
	//	// - 디버그 색상, 지속 시간, 우선순위, 선 두께 등을 지정합니다.
	//	DrawDebugCapsule(
	//		GetWorld(),
	//		CapsuleTransform.GetLocation(),
	//		CapsuleHalfHeight,
	//		CapsuleRadius,
	//		CapsuleTransform.GetRotation(),
	//		FColor::Blue,       // 캡슐을 파란색으로 표시
	//		false,              // 매 프레임 갱신 (false면 매 Tick마다 그려짐)
	//		-1.0f,              // 지속 시간 (-1이면 매 프레임 갱신)
	//		0,
	//		2.0f              // 선 두께
	//	);
	//}


	CircleDebug();

}

void ABaseCharacters::OnSideRightViewPressed()
{
	bUsingKeyboardControl = false;
	 //현재 카메라 붐의 상대 회전을 기준으로 TargetCameraRotation 업데이트 (오른쪽으로 90도 추가)
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 90.f;
	SetActorRotation(CurrentRotation);
}

void ABaseCharacters::OnSideLeftViewPressed()
{
	bUsingKeyboardControl = false;
	 //현재 카메라 붐의 상대 회전을 기준으로 TargetCameraRotation 업데이트 (오른쪽으로 90도 추가)
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw -= 90.f;
	SetActorRotation(CurrentRotation);
}

void ABaseCharacters::UseSkill(int32 SkillID)
{
	// GameInstance에서 USkillManager 서브시스템을 가져옵니다.
	USkillManager* SkillManager = GetGameInstance()->GetSubsystem<USkillManager>();
	if (!SkillManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UseSkill: SkillManager is null."));
		return;
	}

	// SkillManager에서 CachedSkillData에 해당 스킬 ID가 있는지 찾습니다.
	FSkillData* SkillData = SkillManager->CachedSkillData.Find(SkillID);
	if (!SkillData)
	{
		UE_LOG(LogTemp, Error, TEXT("UseSkill: SkillID %d not found."), SkillID);
		return;
	}


	// 애니메이션
	if (!SkillData->AnimationMontage.IsNull())
	{
		// 동기 로드
		UAnimMontage* Montage = SkillData->AnimationMontage.LoadSynchronous();
		if (Montage)
		{
			// 캐릭터의 메쉬 컴포넌트에서 AnimInstance 가져오기
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

	// 2) SpawnDelay 만큼 기다린 뒤 충돌 콜리전 스폰
	float Delay = SkillData->SpawnDelay;  // DataTable에서 읽어온 값
	if (Delay <= 0.f)
	{
		// 즉시 스폰
		SpawnSkillCollision(SkillID);
	}
	else
	{
		FTimerHandle Handle;
		// 람다로 간단하게 바인딩
		GetWorldTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateUObject(this, &ABaseCharacters::SpawnSkillCollision, SkillID),
			Delay,
			false  // 한 번만
		);
	}


//
//	// 스폰 위치 및 회전 설정
//// 예를 들어, 캐릭터 앞쪽 100 유닛 위치에 스폰하도록 함.
//	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
//	FRotator SpawnRotation = GetActorRotation();
//	FTransform SpawnTransform;
//	SpawnTransform.SetLocation(SpawnLocation);
//	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
//	UE_LOG(LogTemp, Warning, TEXT("SpawnTransform Location: %s"), *SpawnTransform.GetLocation().ToString());
//	// USkillManager의 SpawnSkillCollisionActor 함수를 호출해서 스킬 충돌 액터를 스폰합니다.
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
	// 플레이어 캡슐 위치 (중심)
	FVector Center = (GetCapsuleComponent()) ? GetCapsuleComponent()->GetComponentLocation() : GetActorLocation();

	// 캡슐의 절반 높이를 구함
	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Z축을 캡슐 아래쪽(발 위치)로 이동
	// 예: 발바닥 근처에 원을 그리려면, 반높이만큼 내려줌
	Center.Z -= HalfHeight;

	// 원을 그릴 반지름
	float CircleRadius = CollisionRadius2D;

	// 디버그 원 그리기 (바닥에 수평으로)
	//  - X축을 원의 시작 방향,  Z축을 법선(위쪽) 벡터로 설정
	DrawDebugCircle(
		GetWorld(),
		Center,                   // 바닥으로 내린 위치
		CircleRadius,            // 반지름
		32,                      // 세그먼트 수
		FColor::Blue,            // 색상
		false,                   // 지속적 표시 여부
		-1.f,                    // 표시 시간 (-1이면 한 프레임 / persistent)
		0,                       // 우선순위
		2.f,                     // 선 두께
		FVector(1.f, 0.f, 0.f),  // 시작 방향 (X축)
		FVector(0.f, 1.f, 0.f),  // 법선(0,1,0) → XY 평면에 그림
		false
	);
}

void ABaseCharacters::SpawnSkillCollision(int32 SkillID)
{
	USkillManager* SkillManager = GetGameInstance()->GetSubsystem<USkillManager>();
	if (!SkillManager) return;

	FSkillData* Data = SkillManager->CachedSkillData.Find(SkillID);
	if (!Data) return;

	// 스폰 위치/회전 계산 (예시)
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * Data->Range + GetActorForwardVector() * 100.f;
	FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

	// 스킬 콜리전 액터 생성
	ASkill2DCollisionActor* Actor = SkillManager->SpawnSkillCollisionActor(*Data, SpawnTransform);
	if (Actor)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned Skill Collision after %.2f sec"), Data->SpawnDelay);
	}
}





