// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayer.h"
#include "Math/UnrealMathUtility.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/HeadMountedDisplay/Public/MotionControllerComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Enemy/BaseEnemy.h"
#include "BulletActor.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h> 
#include "PlayerWidget.h"
#include "musicGameModeBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Public/KismetTraceUtils.h>
#include "MainRobeUIActor.h"
#include "gameStartActor.h"
#include "shieldWidget.h"
#include "Enemy/MusicActor.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "musicGameInstance.h"
#include "CustomizeActor.h"
#include "Components/SceneComponent.h"
#include "BulletFXActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>



AGunPlayer::AGunPlayer()
{
 
	PrimaryActorTick.bCanEverTick = true;

	//카메라 회전용 롤 컴포넌트를 생성하고, 루트(캡슐)에 붙인다
	rollComp = CreateDefaultSubobject<USceneComponent>(TEXT("RollComp"));
	rollComp->SetupAttachment(RootComponent);

	// VR 카메라 컴포넌트를 생성하고 롤 컴포넌트에 붙임
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(rollComp);

	// 박스 컴포넌트를 카메라에 붙여서 벽에 닿거나 총알에 맞으면 데미지를 받는다.
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxcomp->SetupAttachment(VRCamera);

	//모션컨트롤러 왼손, 오른손 생성하고 루트에 붙이고 싶다.
	
	MotionLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionLeft"));
	MotionLeft->SetTrackingMotionSource(TEXT("Left"));
	MotionLeft->SetupAttachment(rollComp);
	

	MotionRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionRight"));
	MotionRight->SetTrackingMotionSource(TEXT("Right"));
	MotionRight->SetupAttachment(rollComp);

	// 왼손, 오른손 스켈레탈메시컴포넌트를 만들어서 모션컨트롤러에 붙이고싶다.
	
	MeshLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshLeft"));
	MeshLeft->SetupAttachment(MotionLeft);
	
	MeshRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRight"));
	MeshRight->SetupAttachment(MotionRight);

	// 왼손, 오른손 스켈레탈 메시를로드해서 적용하고싶다.
	
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshLeft(TEXT("/Script/Engine.SkeletalMesh'/Game/assets/pistol-desert-eagle-weapon-model-cs2/DEAGLE.DEAGLE'"));
	
	//// 로드 성공했다면 적용하고싶다.
		if (TempMeshLeft.Succeeded())
			{
			MeshLeft->SetSkeletalMesh(TempMeshLeft.Object);
			MeshLeft->SetWorldLocationAndRotation(FVector((6.7f,-11.25f,1.97f)), FRotator((75.0f,  0.0f,  180.0f)));
			MeshLeft->SetRelativeScale3D(FVector(20.0f));
			}
	

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshRight(TEXT("/Script/Engine.SkeletalMesh'/Game/assets/pistol-desert-eagle-weapon-model-cs2/DEAGLE.DEAGLE'"));
	// 로드 성공했다면 적용하고싶다.
	if (TempMeshRight.Succeeded())
	{
		MeshRight->SetSkeletalMesh(TempMeshRight.Object);
		MeshRight->SetWorldLocationAndRotation(FVector((13.3f, 10.7f, -25.1f)), FRotator((35.0f, 0.0f, 180.0f)));
		MeshRight->SetRelativeScale3D(FVector(20.0f));
	}

	// 오른손과 왼손에 컴포넌트 컬리전을 넣어서 근접공격을 만든다
	LeftHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHitComp"));
	LeftHitComp->SetupAttachment(MeshLeft);
	

	RightHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHitComp"));
	RightHitComp->SetupAttachment(MeshRight);

	
	MeshLeft->SetGenerateOverlapEvents(true); 
	MeshRight->SetGenerateOverlapEvents(true);

	// 플레이어 건 위젯
	
	PlayerGunWidgetLeftComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Left Gun Component"));
	PlayerGunWidgetLeftComp->SetupAttachment(MeshLeft);
	
	PlayerGunWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	PlayerGunWidgetComp->SetupAttachment(MeshRight);

	// 플레이어 쉴드위젯
	PlayerShieldWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Shield Component"));
	PlayerShieldWidgetComp->SetupAttachment(VRCamera);

	
	leftScene = CreateDefaultSubobject<USceneComponent>(TEXT("player Gun Fire Left Target"));
	leftScene->SetupAttachment(MeshLeft);
	leftScene->SetRelativeLocation(FVector(0.25f, 0.0f, 0.58f));
	leftScene->SetRelativeRotation(MeshLeft->GetComponentRotation());
	

	rightScene = CreateDefaultSubobject<USceneComponent>(TEXT("player Gun Fire Target"));
	rightScene->SetupAttachment(MeshRight);
	rightScene->SetRelativeLocation(FVector(0.25f, 0.0f, 0.58f));
	rightScene->SetRelativeRotation(MeshRight->GetComponentRotation());

}

void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 연결
	PlayerLeftWidget  = Cast<UPlayerWidget>(PlayerGunWidgetLeftComp->GetWidget());
	PlayerWidget = Cast<UPlayerWidget>(PlayerGunWidgetComp->GetWidget());

	shieldWidget = Cast<UshieldWidget>(PlayerShieldWidgetComp->GetWidget());
	gi = Cast<UmusicGameInstance>(GetGameInstance());

	LeftGunToggle(gi->bIsWeapon);

	SetStartLoc();

	moveDist = FVector::Dist(startLoc, targetLoc)/endTime;

	// 플레이어 이동속도
	if (GetWorld()->GetMapName().Contains(FString("LoadingLevel"))) {
		if (shieldWidget != nullptr)
		{
			shieldWidget->startShield();
		}
	}
	else if (GetWorld()->GetMapName().Contains(FString("TextLevel"))) {
		targetLoc = GetActorLocation() + FVector(1000, 0, 0);
		endTime = 3.0f;
	}
	else if (GetWorld()->GetMapName().Contains(FString("startMap")) || GetWorld()->GetMapName().Contains(FString("gameStartMap"))) {
		targetLoc = GetActorLocation();
		endTime = 1.0f;
	}
	
	leftbulletFactory = 15;
	bulletFactory = 15;

	//사격 이펙트 액터를 생성하고 배열에 넣어놓는다.
	FXarray.Empty();
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FXarray.Add(GetWorld()->SpawnActor<ABulletFXActor>(bulletFX, FVector::ZeroVector, FRotator::ZeroRotator, params));
	FXarray.Add(GetWorld()->SpawnActor<ABulletFXActor>(bulletFX, FVector::ZeroVector, FRotator::ZeroRotator, params));

	// 인풋
	pc = Cast<APlayerController>(Controller);
	
	if (pc != nullptr)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(IMC_GunPlayer, 0);
		}
		pc->SetShowMouseCursor(true);
	}

	// 펀치공격
	LeftHitComp->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap);
	
	RightHitComp->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap); 
}

void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStart && moveTime < endTime)
	{
		if (!bIsCastingDone) {
			for (TActorIterator<AMusicActor> iter(GetWorld()); iter; ++iter) {
				musicActor = Cast<AMusicActor>(*iter);
			}
			if (musicActor != nullptr) {
				bIsCastingDone = true;
			}
		}

		moveTime += DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("%f / %f"), moveTime, endTime);
		SetActorLocation(FMath::Lerp(startLoc, targetLoc, (moveTime/endTime)), true);
	}
	
	if (bLeft)
	{
		if (FVector::Dist(VRCamera->GetComponentLocation(), MeshLeft->GetComponentLocation()) > 70)
		{
			ONLeftReroad();
		}
	}

	if (FVector::Dist(VRCamera->GetComponentLocation(), MeshRight->GetComponentLocation()) > 70)
	{
		ONReroad();
	}
}

void AGunPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		// 함수를 인풋 컴포넌트에 연결한다.
		
		enhancedInputComponent->BindAction(IA_LeftFire, ETriggerEvent::Started, this, &AGunPlayer::ONLeftFire);
		enhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &AGunPlayer::ONFire);

		enhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AGunPlayer::ONTurn);

		// 총 회전
		enhancedInputComponent->BindAction(IA_LeftSpinGun, ETriggerEvent::Triggered, this, &AGunPlayer::ONLeftSpinGun);
		enhancedInputComponent->BindAction(IA_LeftSpinGun, ETriggerEvent::Completed, this, &AGunPlayer::ONLeftSpinGun);

		enhancedInputComponent->BindAction(IA_SpinGun, ETriggerEvent::Triggered, this, &AGunPlayer::ONRightSpinGun);
		enhancedInputComponent->BindAction(IA_SpinGun, ETriggerEvent::Completed, this, &AGunPlayer::ONRightSpinGun);
	}
}

//플레이어 스타트 위치 지정
void AGunPlayer::SetStartLoc()
{
	gi = Cast<UmusicGameInstance>(GetGameInstance());
	startLoc = GetActorLocation();
	
	if (gi->bIsPlaingBBKK){
		targetLoc = GetActorLocation() + FVector(31400.0f, 0.0f, 0.0f);
		endTime = 136.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), endTime);
	}
	else {
		targetLoc = GetActorLocation() + FVector(31400.0f, 0.0f, 0.0f);
		endTime = 130.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), endTime);
	}
	//UE_LOG(LogTemp, Warning, TEXT("start : %f, %f, %f"), startLoc.X, startLoc.Y, startLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("end : %f, %f, %f"), targetLoc.X, targetLoc.Y, targetLoc.Z);

}

void AGunPlayer::ONLeftFire(const FInputActionValue& value)
{
	if (bLeft)
	{ 
		FVector StartFire = MeshLeft->GetComponentLocation();

		if (leftbulletFactory > 0)
		{
			leftbulletFactory -= 1;
			if (PlayerLeftWidget != nullptr)
			{
				PlayerLeftWidget->remainBullet(-1);
			}
			if (explosionFX != nullptr && NI_Arrow != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, leftScene->GetComponentLocation(), leftScene->GetComponentRotation() + FRotator(90, 0, 0));

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Arrow, leftScene->GetComponentLocation(), leftScene->GetComponentRotation());
			}

			FHitResult hitInfo;
			FVector start = leftScene->GetComponentLocation();
			FVector end = start + leftScene->GetForwardVector() * 100000;
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);  
			params.AddIgnoredComponent(MeshLeft); 

			FQuat startRot = FRotator(0, 0, 0).Quaternion();

			// Sphere Trace
			bool bResult = GetWorld()->SweepSingleByChannel(hitInfo, start, end, startRot, ECC_Visibility, FCollisionShape::MakeSphere(60), params);

			// Sound
			if (fireSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
			}

			if (bResult)
			{
				enemy = Cast<ABaseEnemy>(hitInfo.GetActor());
				widgetLevel = Cast<AMainRobeUIActor>(hitInfo.GetActor());
				startActor = Cast<AgameStartActor>(hitInfo.GetActor());
				if (enemy != nullptr)
				{
					PlayFX(hitInfo.ImpactPoint);
					bool bIsEnemyDead = enemy->Hit(false, CurrentXEnemy);
					if (bIsEnemyDead)
					{
						CurrentXNumber++;
						SetCurrentXNumber();
						UE_LOG(LogTemp, Warning, TEXT("1"));
						if (!bshield)
						{
							shieldrecovery();
						}
					}
					return;
				}
				else if (widgetLevel != nullptr)
				{
					widgetLevel->MoveLevel();
				}
				else if (startActor != nullptr)
				{
					if (startActor->bPlayStart)
					{
						UGameplayStatics::OpenLevel(this, "startMap");
					}
				}
			}
		}
	}
}

// 총알 발사 인풋
void AGunPlayer::ONFire(const FInputActionValue& value)
{
	FVector StartFire = MeshRight->GetComponentLocation();
	
	if (bulletFactory > 0)
	{ 
		bulletFactory -= 1;
		if (PlayerWidget != nullptr)
		{
			PlayerWidget->remainBullet(-1);
		}
		if (explosionFX != nullptr && NI_Arrow != nullptr)
		{
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, rightScene->GetComponentLocation(), rightScene->GetComponentRotation()+ FRotator(90,0,0));
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Arrow, rightScene->GetComponentLocation(), rightScene->GetComponentRotation());
		}

		FHitResult hitInfo;
		FVector start = rightScene->GetComponentLocation();
		FVector end = start + rightScene->GetForwardVector() * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this); 
		params.AddIgnoredComponent(MeshRight);

		FQuat startRot = FRotator(0, 0, 0).Quaternion();

		// Sphere Trace
		bool bResult = GetWorld()->SweepSingleByChannel(hitInfo, start, end, startRot, ECC_Visibility, FCollisionShape::MakeSphere(60), params);

		// Sound
		if (fireSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
		}

		if (bResult)
		{
			enemy = Cast<ABaseEnemy>(hitInfo.GetActor());
			widgetLevel = Cast<AMainRobeUIActor>(hitInfo.GetActor());
			startActor = Cast<AgameStartActor>(hitInfo.GetActor());
			WeaponActor = Cast<ACustomizeActor>(hitInfo.GetActor());

			if (enemy != nullptr)
			{
				PlayFX(hitInfo.ImpactPoint);
				bool bIsEnemyDead = enemy->Hit(false, CurrentXEnemy);
				if (bIsEnemyDead)
				{
					CurrentXNumber++;
					SetCurrentXNumber();
					UE_LOG(LogTemp , Warning, TEXT("1"));
					if(!bshield)
					{
						shieldrecovery();
					}
				}
				return;
			}
			else if (widgetLevel != nullptr)
			{
				widgetLevel->MoveLevel();
			}
			else if (startActor != nullptr)
			{
				if (startActor->bPlayStart)
				{
					UGameplayStatics::OpenLevel(this, "startMap");
				}
			}
			else if (WeaponActor != nullptr)
			{
				if (hitInfo.GetComponent() == WeaponActor->OneboxComp)
				{
					UE_LOG(LogTemp,Warning,TEXT("leftflasse"));
					LeftGunToggle(false);
				}
				else if (hitInfo.GetComponent() == WeaponActor->dobleboxComp)
				{
					UE_LOG(LogTemp, Warning, TEXT("true"));
					LeftGunToggle(true);
				}
			}
		}
	}
}

void AGunPlayer::ONTurn(const FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerYawInput(v);
}

void AGunPlayer::ONLeftSpinGun(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{ 
		float dt = GetWorld()->GetDeltaSeconds();
		auto newRotation = MeshLeft->GetRelativeRotation().Quaternion() * FRotator(-360 * 4 * dt, 0, 0).Quaternion();
		MeshLeft->SetRelativeRotation(newRotation);
	}
	else
	{
		MeshLeft->SetRelativeRotation(FRotator(5, 0, 180));
	}
}

void AGunPlayer::ONRightSpinGun(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		float dt = GetWorld()->GetDeltaSeconds();
		auto newRotation = MeshRight->GetRelativeRotation().Quaternion() * FRotator(-360 * 4 * dt, 0, 0).Quaternion();
		MeshRight->SetRelativeRotation(newRotation);
	}
	else
	{
		MeshRight->SetRelativeRotation(FRotator(5,0,180));
	}
}


void AGunPlayer::ONReroad()
{
	bulletFactory = 15;
	PlayerWidget->remainBullet(15);
}

void AGunPlayer::ONLeftReroad()
{
	leftbulletFactory = 15;
	PlayerLeftWidget->remainBullet(15);
}

void AGunPlayer::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	enemy = Cast<ABaseEnemy>(OtherActor);
	if (OtherActor->IsA<ABaseEnemy>())
	{
		if (enemy->Hit(true, CurrentXEnemy) && bshield == false)
		{
			shieldrecovery();
		}
		CurrentXNumber++;
		SetCurrentXNumber();
		UE_LOG(LogTemp, Warning, TEXT("2"));
	}
}

// 공격당하면
void AGunPlayer::OnDamaged()
{
	pc = GetController<APlayerController>();

	
	if (bshield == true)
	{
		CurrentXEnemy = 1;
		CurrentXNumber = 0;
		shieldWidget->CurrentX(0);
		// 페이드 인 효과를 준다.
		if (pc != nullptr)
		{
			pc->PlayerCameraManager->StartCameraFade(1, 0, 1.0f, FLinearColor::Red);
			 // 불값으로 틱에전달해서 델타세컨드를 통해 음악소리를 조절한다.
			musicActor->VolumeHitReact(false);
		}
		
		// 쉴드위젯을 히트쉴드위젯으로 변경한다.
		if (shieldWidget != nullptr)
		{
			shieldWidget->hitShield();
			if (hitSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
			}
			bshield = false;
		}
	}
	
	else if (bshield == false)
	{
		Destroy();
	}
}


void AGunPlayer::shieldrecovery()
{
	shieldWidget->removeShield(-1);
	recovery += 1;
	if (recovery == 4)
	{
		if (shieldWidget != nullptr)
		{
		 // 회복 페이드아웃 효과를 준다.
			pc->PlayerCameraManager->StartCameraFade(1, 0, 1.0f, FLinearColor::Green);
			shieldWidget->produceShield();
			bshield = true;
			shieldWidget->removeShield(4);
			recovery = 0;
		}
	}
}

void AGunPlayer::SetCurrentXNumber()
{
	// 점수
	if (CurrentXNumber == 2)
	{
		shieldWidget->CurrentX(1);
		CurrentXEnemy += 1;
		UE_LOG(LogTemp, Warning, TEXT("Num1"));
	}
	else if (CurrentXNumber == 6)
	{
		shieldWidget->CurrentX(2);
		CurrentXEnemy += 2;
		UE_LOG(LogTemp, Warning, TEXT("Num2"));
	}
	else if (CurrentXNumber == 14)
	{
		shieldWidget->CurrentX(4);
		CurrentXEnemy += 4;
		UE_LOG(LogTemp, Warning, TEXT("Num3"));
	}
}

//왼손총 최적화
void AGunPlayer::LeftGunToggle(bool value)
{
	if (value) {
		bLeft = true;
		gi->bIsWeapon = true;
		MotionLeft->SetVisibility(true, true);
		leftbulletFactory = 15;
		PlayerLeftWidget->remainBullet(15);
	}
	else {
		bLeft = false;
		gi->bIsWeapon = false;
		MotionLeft->SetVisibility(false, true);

	}
}

void AGunPlayer::PlayFX(FVector playLoc)
{
	int32 i = FXAddress % 2;
	FXAddress++;
	FXarray[i]->SetActorLocation(playLoc);
	FXarray[i]->PlayFX();
}


	