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
#include "DialogueData/DialogueWidget.h"    // ��ȭ ���� ���
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


}

void AVillageCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (Controller)
	{
		AVillagePC = Cast<AVillageController>(Controller);
	}
	

}

void AVillageCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSmoothMoving)
	{
		// ���� ��ġ�� ��ǥ ��ġ�� ���� ���� (Lerp)
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, MoveSpeed);
		SetActorLocation(NewLocation);

		// ���� ȸ���� ��ǥ ȸ���� ���� ���� (RInterp)
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, RotateSpeed);
		SetActorRotation(NewRotation);

		// ��ǥ�� ����� ��������� �̵� ����
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
		// ����� ���� �Է� ���ε� (��: ��ġ �Է�)
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

	// ���ϴ� ���ͷ��� ����
	const float InteractionRange = 300.0f;

	// ��ȣ�ۿ� �ִ����
	const float MaxInteractionRange = 450.0f;

	// ���� ����� NPC�� �˻�
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

	// ���� ���� ����� NPC�� �ִٸ�
	if (TargetNPC)
	{
		// �÷��̾�� NPC ������ �Ÿ��� InteractionRange���� ũ��
		if (ClosestDistance > InteractionRange && ClosestDistance < MaxInteractionRange)
		{
			// NPC�� �÷��̾� ������ ���� ���� ���
			FVector Direction = (TargetNPC->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			// NPC�� �������� ���ϴ� �Ÿ� ��ŭ ������ ��ġ ���
			FVector TargetPosition = TargetNPC->GetActorLocation() - Direction * InteractionRange;
			// �÷��̾ �ش� ��ġ�� ��� �̵�
			SetActorLocation(TargetPosition);
			UE_LOG(LogTemp, Warning, TEXT("Player moved to interaction range at: %s"), *TargetPosition.ToString());

			// NPC�� ��ȣ�ۿ�: NPC�� Interact() �Լ��� ȣ���Ͽ� ��ȭ ó��
			TargetNPC->Interact(this);

			// ��ȭ ���� ����
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
}

void AVillageCharacter::TouchPressed(const FInputActionValue& Value)
{
	// ��ġ ���� �� �Է°��� ���� ȭ����� ��ǥ (Vector2D)�� ���޵˴ϴ�.
	FVector2D TouchLocation = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Touch Pressed at: %s"), *TouchLocation.ToString());

}

void AVillageCharacter::TouchReleased(const FInputActionValue& Value)
{
	FVector2D TouchLocation = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Touch Released at: %s"), *TouchLocation.ToString());

	// ��ġ ���� �� ó���� ���� ����

}

void AVillageCharacter::OnVirtualJoystickMoved(FVector2D MovementVector)
{
	if (Controller)
	{
		// ���� ��Ʈ�ѷ��� ȸ���� �������� �̵� ���� ���
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 2D ������ Y���� ����, X���� �¿� �̵�
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

