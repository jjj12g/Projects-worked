// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacters.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"





ABasePlayerController::ABasePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ABasePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 뷰포트에 고정하지 않음
	InputMode.SetHideCursorDuringCapture(false); // 캡처 시 커서 숨기지 않음
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	APawn* ControlledPawn = GetPawn();
	BasePlayer = Cast<ABaseCharacters>(ControlledPawn);

}

void ABasePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ABasePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ABasePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ABasePlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ABasePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ABasePlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ABasePlayerController::OnTouchReleased);

		// 카메라 시점 변경이동
		EnhancedInputComponent->BindAction(SetCamaraSideRightMoveAction, ETriggerEvent::Started, this, &ABasePlayerController::OnSideRightViewPressed);
		EnhancedInputComponent->BindAction(SetCamaraSideLeftMoveAction, ETriggerEvent::Started, this, &ABasePlayerController::OnSideLeftViewPressed);
		EnhancedInputComponent->BindAction(SetMoveForwardAction, ETriggerEvent::Triggered, this, &ABasePlayerController::PlayerMoveForward);

		// 플레이어 스킬 사용
		EnhancedInputComponent->BindAction(SetQSkillAction, ETriggerEvent::Started, this, &ABasePlayerController::OnQSkillActionPressed);
		EnhancedInputComponent->BindAction(SetWSkillAction, ETriggerEvent::Started, this, &ABasePlayerController::OnWSkillActionPressed);
		EnhancedInputComponent->BindAction(SetESkillAction, ETriggerEvent::Started, this, &ABasePlayerController::OnESkillActionPressed);
		
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NotComponent"));
	}
}

void ABasePlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ABasePlayerController::OnSetDestinationTriggered()
{
	APawn* ControlledPawn = GetPawn();
	BasePlayer->bUsingKeyboardControl = true;
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	//APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ABasePlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ABasePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ABasePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ABasePlayerController::OnSideRightViewPressed()
{
	if (BasePlayer != nullptr)
	{
		BasePlayer->OnSideRightViewPressed();
		//UE_LOG(LogTemp, Warning, TEXT("OnSideViewPressed"));
	}
	//UE_LOG(LogTemp,Warning,TEXT("Player SideView"));
}

void ABasePlayerController::OnSideLeftViewPressed()
{
	if (BasePlayer != nullptr)
	{
		BasePlayer->OnSideLeftViewPressed();
	}
}


void ABasePlayerController::PlayerMoveForward()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		// Pawn의 현재 앞방향(월드 공간)으로 이동 입력을 줍니다.
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), 1.0f);
	}
}


void ABasePlayerController::OnQSkillActionPressed()
{
	if (BasePlayer != nullptr)
	{
		BasePlayer->UseSkill(4);
	}
}


void ABasePlayerController::OnWSkillActionPressed()
{
	if (BasePlayer != nullptr)
	{
		BasePlayer->UseSkill(2);
	}
}

void ABasePlayerController::OnESkillActionPressed()
{
	if (BasePlayer != nullptr)
	{
		BasePlayer->UseSkill(3);
	}
}
