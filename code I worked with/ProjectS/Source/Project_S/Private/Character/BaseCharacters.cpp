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
#include "DrawDebugHelpers.h"
#include "SkillCollection/SkillManager.h"
#include "SkillCollection/SkillData.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Engine/GameInstance.h" 



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
	
	//Schema Load (서버데이터 로드)
	//UConfigManager::LoadConfig();
	//UDataManager::LoadData();


	// 만약 StatComponent가 존재하면, PlayerName을 RowName으로 설정 후 데이터 테이블에서 스탯을 로드
	if (StatComponent)
	{
		StatComponent->StatRowName = PlayerName;
		StatComponent->LoadStatsFromDataTable();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StatComponent is not valid on %s"), *GetName());
	}




}

// Called every frame
void ABaseCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!bUsingKeyboardControl)
	{
		FRotator DesiredRelativeRotation = FRotator(-60.f, GetActorRotation().Yaw, 0.f);

		// 카메라 회전 보간
		FRotator CurrentRelativeRotation = CameraBoom->GetRelativeRotation();
		FRotator NewRelativeRotation = FMath::RInterpTo(CurrentRelativeRotation, DesiredRelativeRotation, DeltaTime, CameraRotationInterpSpeed);
		CameraBoom->SetRelativeRotation(NewRelativeRotation);
	}


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
	 //현재 카메라 붐의 상대 회전을 기준으로 TargetCameraRotation 업데이트 (왼쪽으로 90도 추가)
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw -= 90.f;
	SetActorRotation(CurrentRotation);
}

void ABaseCharacters::UseSkill(int32 SkillID)
{
	// GameInstance에서 USkillManager를 가져옴
	USkillManager* SkillManager = GetGameInstance()->GetSubsystem<USkillManager>();
	if (!SkillManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UseSkill: SkillManager is null."));
		return;
	}

	// SkillManager에서 CachedSkillData에 해당 스킬 ID가 있는지 탐색
	FSkillData* SkillData = SkillManager->CachedSkillData.Find(SkillID);
	if (!SkillData)
	{
		UE_LOG(LogTemp, Error, TEXT("UseSkill: SkillID %d not found."), SkillID);
		return;
	}


	// 애니메이션
	if (!SkillData->AnimationMontage.IsNull())
	{
		UAnimMontage* Montage = SkillData->AnimationMontage.LoadSynchronous();
		if (Montage)
		{
			// 캐릭터의 메쉬 컴포넌트에서 AnimInstance 가져오기
			if (USkeletalMeshComponent* MeshComp = GetMesh())
			{
				if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
				{
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
			false  
		);
	}
}

void ABaseCharacters::CircleDebug()
{
	// 플레이어 캡슐 위치의 중심
	FVector Center = (GetCapsuleComponent()) ? GetCapsuleComponent()->GetComponentLocation() : GetActorLocation();

	// 캡슐의 절반 높이를 구함
	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Z축을 캡슐 아래쪽인 발 위치로 이동
	Center.Z -= HalfHeight;

	// 원을 그릴 반지름
	float CircleRadius = CollisionRadius2D;

	// 디버그 원 그리기 (바닥 수평)
	DrawDebugCircle(
		GetWorld(),
		Center,                  
		CircleRadius,            
		32,                      
		FColor::Blue,            
		false,                   
		-1.f,                   
		0,                     
		2.f,                    
		FVector(1.f, 0.f, 0.f), 
		FVector(0.f, 1.f, 0.f),  
		false
	);
}

void ABaseCharacters::SpawnSkillCollision(int32 SkillID)
{
	USkillManager* SkillManager = GetGameInstance()->GetSubsystem<USkillManager>();
	if (!SkillManager) return;

	FSkillData* Data = SkillManager->CachedSkillData.Find(SkillID);
	if (!Data) return;

	// 스폰 위치/회전 계산
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * Data->Range + GetActorForwardVector() * 100.f;
	FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

	// 스킬 콜리전 액터 생성
	ASkill2DCollisionActor* Actor = SkillManager->SpawnSkillCollisionActor(*Data, SpawnTransform);
	if (Actor)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned Skill Collision after %.2f sec"), Data->SpawnDelay);
	}
}





