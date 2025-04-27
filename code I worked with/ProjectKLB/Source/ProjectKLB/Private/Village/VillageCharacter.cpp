// Fill out your copyright notice in the Description page of Project Settings.


#include "Village/VillageCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Village/VillageController.h"
#include "Village/NPC/FavorabilityBaseNpcCharacter.h"
#include "DialogueData/DialogueWidget.h"    // 대화 위젯 헤더
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Village/Mobile/VirtualJoystickWidget.h"




// Sets default values
AVillageCharacter::AVillageCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bIsSmoothMoving = false;
	MoveSpeed = 5.0f;
	RotateSpeed = 5.0f;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AVillageCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (Controller)
	{
		AVillagePC = Cast<AVillageController>(Controller);
	}
	
	/*if (VirtualJoystickWidgetClass)
	{
		UVirtualJoystickWidget* VirtualJoystickWidget = CreateWidget<UVirtualJoystickWidget>(GetWorld(), VirtualJoystickWidgetClass);
		if (VirtualJoystickWidget)
		{
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
			VirtualJoystickWidget->AddToViewport();
			VirtualJoystickWidget->OnMovementUpdated.AddDynamic(this, &AVillageCharacter::OnVirtualJoystickMoved);
		}
	}*/

}

void AVillageCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSmoothMoving)
	{
		// 현재 위치와 목표 위치를 선형 보간 (Lerp)
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, MoveSpeed);
		SetActorLocation(NewLocation);

		// 현재 회전과 목표 회전을 선형 보간 (RInterp)
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, RotateSpeed);
		SetActorRotation(NewRotation);

		// 목표에 충분히 가까워지면 이동 종료
		if (FVector::Dist(NewLocation, TargetLocation) < 1.0f &&
			NewRotation.Equals(TargetRotation, 1.0f))
		{
			bIsSmoothMoving = false;
			UE_LOG(LogTemp, Log, TEXT("Smooth move finished."));
		}
	}

}



//////////////////////////////////////////////////////////////////////////
// Input

void AVillageCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			/*Subsystem->AddMappingContext(VillageMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("UEnhancedInputLocalPlayerSubsystem"));*/
			
			#if PLATFORM_IOS || PLATFORM_ANDROID
			Subsystem->AddMappingContext(MobileMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("Mobile Input Mapping Context Added"));
			#else
			Subsystem->AddMappingContext(VillageMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("PC Input Mapping Context Added"));
			#endif
		}
	}
}

void AVillageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVillageCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVillageCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AVillageCharacter::Interact);

		#if PLATFORM_IOS || PLATFORM_ANDROID
		// 모바일 전용 입력 바인딩 (예: 터치 입력)
		EnhancedInputComponent->BindAction(TouchPressedAction, ETriggerEvent::Started, this, &AVillageCharacter::TouchPressed);
		EnhancedInputComponent->BindAction(TouchReleasedAction, ETriggerEvent::Completed, this, &AVillageCharacter::TouchReleased);
		#endif
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not using EnhancedInputComponent"));
	}
}

void AVillageCharacter::StartSmoothMove(const FVector& InTargetLocation, const FRotator& InTargetRotation)
{
	TargetLocation = InTargetLocation;
	TargetRotation = InTargetRotation;
	bIsSmoothMoving = true;

}

void AVillageCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();


	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveNotConreoller"));
	}
}

void AVillageCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LookNotConreoller"));
	}
}

void AVillageCharacter::Interact(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("InputSpaceBar"));

	// 원하는 인터랙션 범위 (300 cm)
	const float InteractionRange = 300.0f;

	// 상호작용 최대범위
	const float MaxInteractionRange = 450.0f;

	// 가장 가까운 NPC를 검색합니다.
	TArray<AActor*> FoundNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFavorabilityBaseNpcCharacter::StaticClass(), FoundNPCs);

	AFavorabilityBaseNpcCharacter* TargetNPC = nullptr;
	float ClosestDistance = FLT_MAX;
	for (AActor* Actor : FoundNPCs)
	{
		AFavorabilityBaseNpcCharacter* NPC = Cast<AFavorabilityBaseNpcCharacter>(Actor);
		if (NPC)
		{
			float Distance = FVector::Dist(NPC->GetActorLocation(), GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				TargetNPC = NPC;
			}
		}
	}

	// 만약 가장 가까운 NPC가 있다면
	if (TargetNPC)
	{
		// 플레이어와 NPC 사이의 거리가 InteractionRange보다 크면
		if (ClosestDistance > InteractionRange && ClosestDistance < MaxInteractionRange)
		{
			// NPC와 플레이어 사이의 방향 벡터 계산
			FVector Direction = (TargetNPC->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			// NPC를 기준으로 원하는 거리(300cm)만큼 떨어진 위치 계산
			FVector TargetPosition = TargetNPC->GetActorLocation() - Direction * InteractionRange;
			// 플레이어를 해당 위치로 즉시 이동 (부드러운 이동을 원하면 Lerp나 Timeline 사용 가능)
			SetActorLocation(TargetPosition);
			UE_LOG(LogTemp, Warning, TEXT("Player moved to interaction range at: %s"), *TargetPosition.ToString());

			// NPC와 상호작용: NPC의 Interact() 함수를 호출하여 대화 처리
			TargetNPC->Interact(this);

			// 대화 위젯 생성 예시 (DialogueWidgetClass가 설정되어 있어야 함)
			if (DialogueWidgetClass)
			{
				UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);
				if (DialogueWidget)
				{
					DialogueWidget->NPCRef = TargetNPC;
					DialogueWidget->AddToViewport();
					AVillagePC->GameAndUIInputMode();
					UE_LOG(LogTemp, Log, TEXT("Dialogue widget created for NPC: %s"), *TargetNPC->GetName());
				}
			}
		}

		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No NPC within interaction range."));
	}

	// 디버그: 현재 플레이어 위치에 녹색 디버그 스피어 표시
	DrawDebugSphere(GetWorld(), GetActorLocation(), InteractionRange, 32, FColor::Green, false, 2.0f, 0, 2.0f);


	DrawDebugSphere(GetWorld(), GetActorLocation(), MaxInteractionRange, 32, FColor::Red, false, 2.0f, 0, 2.0f);

	//UE_LOG(LogTemp, Warning, TEXT("InputSpaceBar"));


	//// 300 범위 안 상호작용
	//const float InteractionRange = 300.0f;

	//// GetActorLocation()을 중심으로 녹색 디버그 스피어를 영구적으로 표시합니다.
	//DrawDebugSphere(GetWorld(), GetActorLocation(), InteractionRange, 32, FColor::Green, true, -1.0f, 0, 2.0f);

	//// 가장 가까운 NPC를 검색합니다.
	//TArray<AActor*> FoundNPCs;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFavorabilityBaseNpcCharacter::StaticClass(), FoundNPCs);

	//AFavorabilityBaseNpcCharacter* TargetNPC = nullptr;
	//for (AActor* Actor : FoundNPCs)
	//{
	//	AFavorabilityBaseNpcCharacter* NPC = Cast<AFavorabilityBaseNpcCharacter>(Actor);
	//	if (NPC)
	//	{
	//		float Distance = FVector::Dist(NPC->GetActorLocation(), GetActorLocation());
	//		if (Distance <= InteractionRange)
	//		{
	//			TargetNPC = NPC;
	//			break; // 가까운 NPC를 찾으면 종료 (또는 조건에 따라 가장 가까운 NPC 선택)
	//		}
	//	}
	//}

	//if (TargetNPC)
	//{
	//	// NPC의 Interact 함수를 호출하면 그 안에서 GetDialogueForPlayer()가 실행됩니다.
	//	TargetNPC->Interact(this);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No NPC in interaction range."));
	//}

	//if (TargetNPC)
	//{
	//	// 만약 대화 위젯 클래스가 설정되어 있다면 대화 위젯 생성
	//	if (DialogueWidgetClass)
	//	{
	//		UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);
	//		if (DialogueWidget)
	//		{
	//			// 대화 위젯에 해당 NPC를 할당합니다.
	//			DialogueWidget->NPCRef = TargetNPC;
	//			DialogueWidget->AddToViewport();

	//			AVillagePC->GameAndUIInputMode();

	//			UE_LOG(LogTemp, Log, TEXT("Dialogue widget created for NPC: %s"), *TargetNPC->GetName());
	//		}
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No NPC within %f cm to interact."), InteractionRange);
	//}

}

void AVillageCharacter::TouchPressed(const FInputActionValue& Value)
{
	// 터치 시작 시 입력값은 보통 화면상의 좌표 (Vector2D)로 전달됩니다.
	FVector2D TouchLocation = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Touch Pressed at: %s"), *TouchLocation.ToString());

	// 예: 터치 위치를 월드 좌표로 변환 후 라인 트레이스 등을 통해 오브젝트 선택/이동 처리 가능

}

void AVillageCharacter::TouchReleased(const FInputActionValue& Value)
{
	FVector2D TouchLocation = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Touch Released at: %s"), *TouchLocation.ToString());

	// 터치 종료 후 처리할 로직 구현 (예: 터치 드래그 끝내기 등)

}

void AVillageCharacter::OnVirtualJoystickMoved(FVector2D MovementVector)
{
	if (Controller)
	{
		// 현재 컨트롤러의 회전을 기준으로 이동 방향 계산
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 2D 벡터의 Y값은 전후, X값은 좌우 이동
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

