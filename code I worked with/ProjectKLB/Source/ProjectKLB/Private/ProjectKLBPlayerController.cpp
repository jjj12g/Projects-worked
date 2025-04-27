// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKLBPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectKLBCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Turn/TurnManager.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "CardDate/CardWidget.h"
#include "Components/WidgetComponent.h"
#include "DataGameInstance.h"
#include "CardDate/CardListWidget.h"
#include "Tile/Tile.h"
#include "Tile/GridManager.h"
#include "HUD/UIHUD.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Player/WolfAnimInstance.h"
#include "ProjectKLBGameMode.h"
#include "Widget/CombatUI.h"
#include "Inventory/Inventory.h"
#include "Inventory/Equipment.h" // ���â
#include "Player/CharacterStatComponent.h"
#include "Widget/HealthBarWidget.h"
#include "TutorialStageClearUI/TutorialStageClearUI.h"





DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// ���߿� �÷��̾� �̵��� ��� Walkable�� bool ���� �����ؼ� ��Ʈ��

AProjectKLBPlayerController::AProjectKLBPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	

	// �⺻ ���� ������ ����
	AttackDamage = 20.0f;
}

void AProjectKLBPlayerController::BeginPlay()
{
	Super::BeginPlay();


	// GridManager ���� (�������� ���� ��ġ�߰ų� �������� ã��)
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	if (!GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager not found!"));
	}
	
	// �Է� ���� ���ؽ�Ʈ ���
	if (APlayerController* PC = Cast<APlayerController>(this))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (Subsystem && DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				UE_LOG(LogTemp, Log, TEXT("DefaultMappingContext added to Subsystem"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Subsystem or DefaultMappingContext is null"));
			}
		}
	}

	// CurrentTile �ʱ�ȭ
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		FVector CurrentPosition = ControlledPawn->GetActorLocation();
		CurrentTile = GridManager->GetTileAtLocation(CurrentPosition);
		if (CurrentTile)
		{
			CurrentTile->SetHasPlayer(true); // �ʱ� ��ġ Ÿ�Ͽ� �÷��̾� ����
			UE_LOG(LogTemp, Log, TEXT("Initial CurrentTile set: Q = %d, R = %d"), CurrentTile->Q, CurrentTile->R);
		}
	}

	if (UWorld* World = GetWorld())
	{
		UDataGameInstance* GameInstance = Cast<UDataGameInstance>(World->GetGameInstance());
		if (GameInstance && CardListWidgetClass)
		{
			// ī�� ����Ʈ ���� ���� �Ǵ� ã��
			if (!CardListWidget) // ���� ������ ������ ����
			{
				CardListWidget = CreateWidget<UCardListWidget>(World, CardListWidgetClass);
			}

			if (CardListWidget) // ������ �����ϸ� UI�� ����
			{
				CardListWidget->UpdateCardUI();
			}
		}
	}

	// ���� �ν��Ͻ����� ������ ������ ��������
	UDataGameInstance* GameInstance = GetWorld()->GetGameInstance<UDataGameInstance>();
	if (GameInstance)
	{
		Inventory = GameInstance->GetDisplayedItems();
		UE_LOG(LogTemp, Log, TEXT("Inventory initialized with %d items."), Inventory.Num());

		// �κ��丮 ���� ��������Ʈ ���
		OnInventoryChanged.Broadcast(Inventory);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get GameInstance."));
	}

	// �ʱ� �κ��丮 ���� (����)


	if (CombatUIClass)  // UPROPERTY(EditDefaultsOnly)�� ������ CombatUI Blueprint Ŭ����
	{
		CombatUI = CreateWidget<UCombatUI>(this, CombatUIClass);
		if (CombatUI)
		{
			CombatUI->AddToViewport();
			if (CombatUI->InventoryWidget)
			{
				InventoryOff();
			}
		}
	}

	// UI���� ����� �� ���� �� (��: 8 * 5 = 40)
	const int32 TotalSlots = 40;
	Inventory.SetNum(TotalSlots);  // Inventory �迭�� ũ�⸦ 40���� ����


}

void AProjectKLBPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AProjectKLBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Log, TEXT("SetupInputComponent called"));

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// �Է� �׼� ���ε�
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectKLBPlayerController::HandleClickAction);
		EnhancedInputComponent->BindAction(TogglePauseAction, ETriggerEvent::Started, this, &AProjectKLBPlayerController::TogglePauseMenu);
		UE_LOG(LogTemp, Log, TEXT("Input Component Setup Complete"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null"));
	}
}


void AProjectKLBPlayerController::StartTurn()
{
	// ī�� UI ǥ��
	if (CardListWidget)
	{
		CardListWidget->UpdateCardUI(); // UI ����
	}

	DisplayCardsOnUI();
	CardListWidget->SetCardVisibility(ESlateVisibility::Visible);

	if (!GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager not initialized."));
		return;
	}
}

void AProjectKLBPlayerController::PlayerTurn()
{
	if (CombatUI)
	{
		CombatUI->AddToViewport();
	}

	if (CardListWidget)
	{
		CardListWidget->UpdateCardUI(); // UI ����
	}
	CardListWidget->SetCardVisibility(ESlateVisibility::Visible);

	UE_LOG(LogTemp, Warning, TEXT("PlayerTurn"));
}

void AProjectKLBPlayerController::DisplayCardsOnUI()
{
	if (UWorld* World = GetWorld())
	{
		UDataGameInstance* GameInstance = Cast<UDataGameInstance>(World->GetGameInstance());
		if (GameInstance && CardListWidgetClass)
		{
			// ī�� ����Ʈ ���� ���� �Ǵ� ã��
			if (!CardListWidget) // ���� ������ ������ ����
			{
				CardListWidget = CreateWidget<UCardListWidget>(World, CardListWidgetClass);
				if (CardListWidget)
				{
					// ȭ�鿡 ���� �߰�
					CardListWidget->AddToViewport();
				}
			}

			if (CardListWidget) // ������ �����ϸ� UI�� ����
			{
				CardListWidget->UpdateCardUI();
			}
		}
	}
}

void AProjectKLBPlayerController::UseCard(const FCardData& CardData)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || !GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn or GridManager is null!"));
		return;
	}

	// ī�� �����Ϳ� ���� �̵� ������ Ÿ�� ���
	CurrentCardData = CardData;
	MovePlayerUsingCard(CardData);
	
	// ī�� ��� �� �� ����
	if (UDataGameInstance* GameInstance = GetWorld()->GetGameInstance<UDataGameInstance>())
	{
		GameInstance->UseCardAndShuffle(CardData);
	}

	// UI �����
	if (CardListWidget)
	{
		CardListWidget->SetCardVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("Hidden"));
	}

	UE_LOG(LogTemp, Log, TEXT("Card used: %s"), *CardData.CardName);
}

void AProjectKLBPlayerController::HandleTileClick(ATile* ClickedTile)
{
	if (!ClickedTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClickedTile is null!"));
		return;
	}

	FIntPoint ClickedTileCoord(ClickedTile->Q, ClickedTile->R);
	UE_LOG(LogTemp, Log, TEXT("Clicked Tile: Q = %d, R = %d"), ClickedTile->Q, ClickedTile->R);



	if (HighlightedTiles.Contains(ClickedTileCoord))
	{
		// Ŭ���� Ÿ���� ���� �÷��̾� ȸ��
		ACharacter* ControlledCharacter = GetCharacter();
		if (ControlledCharacter)
		{
			FVector Direction = ClickedTile->GetActorLocation() - ControlledCharacter->GetActorLocation();
			Direction.Z = 0;  // ��������θ� ȸ��
			if (!Direction.IsNearlyZero())
			{
				TargetRotation = Direction.Rotation();
				ControlledCharacter->SetActorRotation(TargetRotation);
			}
		}

		// ����: ��� ��� �� �̵� ����
		TArray<ATile*> Path = GridManager->FindPath(CurrentTile, ClickedTile);
		if (Path.Num() > 0)
		{
			MoveCharacterAlongPath(Path,CurrentCardData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No path found to the clicked tile."));
		}

		GridManager->ClearHighlightedTiles();
		HighlightedTiles.Empty();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Clicked Tile is not a valid target!"));
	}
}


// ���� ����� �Ÿ� ���� UI�� ó��, ����� �÷��̾� Ŭ�� ���� ��, Ÿ������ ���߿� ���Ÿ� ���� ��ɿ� ����� ����..?
void AProjectKLBPlayerController::AttackEnemy()
{
	// UI �Ǵ� Ÿ���� �ý����� ���� ������ ��(TargetEnemy)�� �����´ٰ� �����մϴ�.
	AEnemy* TargetEnemy = nullptr; // ��: Ÿ���� �ý����� ���� �Ҵ�

	// ���� ������ Pawn(�÷��̾� ĳ����) ��������
	AProjectKLBCharacter* MyCharacter = Cast<AProjectKLBCharacter>(GetPawn());

	if (MyCharacter && TargetEnemy)
	{
		// �÷��̾��� ���� ���۳�Ʈ���� ���ݷ� ���� �����ɴϴ�.
		float DamageAmount = 0.0f;
		if (MyCharacter->StatComponent)
		{
			DamageAmount = MyCharacter->StatComponent->FinalStats.Attack;
			UE_LOG(LogTemp, Warning, TEXT("is Attack StatComp"));
		}
		else
		{
			// ���� StatComponent�� ������, �⺻���� ����� �� �ֽ��ϴ�.
			DamageAmount = AttackDamage; // ��: AttackDamage�� 20.0f ������ �����Ǿ� �ִٸ�.
		}

		// ������ ������ ���� (TargetEnemy->Damage() �Լ��� �����Ѵٰ� ����)
		TargetEnemy->Damage(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Attacked targeted enemy: %s with damage: %f"), *TargetEnemy->GetName(), DamageAmount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No enemy targeted for attack or character is invalid."));
	}
}


void AProjectKLBPlayerController::MoveCharacterToTile(ATile* TargetTile)
{
	// TargetTile�� null���� Ȯ��
	if (!TargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetTile is null!"));
		return;
	}

	// ���� ��ġ�� Ÿ���� bHasPlayer�� false�� ����
	if (CurrentTile)
	{
		CurrentTile->SetHasPlayer(false);
		UE_LOG(LogTemp, Log, TEXT("Set bHasPlayer to false for Tile: Q = %d, R = %d"), CurrentTile->Q, CurrentTile->R);
	}

	// TargetTile�� bHasPlayer�� true�� ����
	TargetTile->SetHasPlayer(true);
	UE_LOG(LogTemp, Log, TEXT("Set bHasPlayer to true for Tile: Q = %d, R = %d"), TargetTile->Q, TargetTile->R);

	// CurrentTile ������Ʈ
	CurrentTile = TargetTile;

	// �÷��̾� �̵� ó��
	ACharacter* ControlledCharacter = GetCharacter();
	if (ControlledCharacter)
	{
		FVector TargetPosition = TargetTile->GetActorLocation();
		ControlledCharacter->SetActorLocation(TargetPosition);
		UE_LOG(LogTemp, Log, TEXT("Character moved to Tile: Q = %d, R = %d"), TargetTile->Q, TargetTile->R);
	}
}

void AProjectKLBPlayerController::AttackSpecificEnemy(AEnemy* TargetEnemy)
{
	if (!TargetEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("No enemy specified for attack!"));
		return;
	}

	// ���� ������ ó��
	// ���� ������ Pawn(�÷��̾� ĳ����) ��������
	AProjectKLBCharacter* MyCharacter = Cast<AProjectKLBCharacter>(GetPawn());

	if (MyCharacter && TargetEnemy)
	{
		// �÷��̾��� ���� ���۳�Ʈ���� ���ݷ� ���� �����ɴϴ�.
		float DamageAmount = 0.0f;
		if (MyCharacter->StatComponent)
		{
			DamageAmount = MyCharacter->StatComponent->FinalStats.Attack;
			UE_LOG(LogTemp, Warning, TEXT("is Attack StatComp"));
		}
		else
		{
			// ���� StatComponent�� ������, �⺻���� ����� �� �ֽ��ϴ�.
			DamageAmount = AttackDamage; // ��: AttackDamage�� 20.0f ������ �����Ǿ� �ִٸ�.
		}

		// ������ ������ ���� (TargetEnemy->Damage() �Լ��� �����Ѵٰ� ����)
		TargetEnemy->Damage(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Attacked targeted enemy: %s with damage: %f"), *TargetEnemy->GetName(), DamageAmount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No enemy targeted for attack or character is invalid."));
	}
	 


	// �÷��̾� ĳ���� ���
	APawn* CharPawn = GetPawn();
	if (!CharPawn) return;

	AProjectKLBCharacter* ACharacter = Cast<AProjectKLBCharacter>(CharPawn);
	if (!ACharacter) return;

	// �ִϸ��̼� ��Ÿ�� ���
	if (ACharacter->GetMesh() && ACharacter->GetMesh()->GetAnimInstance() && ACharacter->PlayerAttackMontage)
	{
		ACharacter->PlayAnimMontage(ACharacter->PlayerAttackMontage);
	}

	CallEndTurn();
}

// Ÿ�� �Է�
void AProjectKLBPlayerController::HandleClickAction()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		if (ATile* ClickedTile = Cast<ATile>(HitResult.GetActor()))
		{
			HandleTileClick(ClickedTile);
		}
	}

}

// HUD ����
void AProjectKLBPlayerController::TogglePauseMenu()
{
	if (AUIHUD* HUD = Cast<AUIHUD>(GetHUD()))
	{
		SetPause(true);
		HUD->ShowPauseMenu();
	}
}

void AProjectKLBPlayerController::CallEndTurn()
{
	if (UWorld* World = GetWorld())
	{
		if (AProjectKLBGameMode* GM = Cast<AProjectKLBGameMode>(World->GetAuthGameMode()))
		{
			if (GM->TurnManagerInstance)
			{
				GM->TurnManagerInstance->EndTurn();
			}
		}
	}
}

void AProjectKLBPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	AProjectKLBCharacter* NewCharacter = Cast<AProjectKLBCharacter>(InPawn);

	if (NewCharacter)
	{
		if (NewCharacter->CurrentHP <= 0)
		{
			if (NewCharacter->CurrentTile)
			{
				NewCharacter->CurrentTile->SetHasPlayer(false);
				UE_LOG(LogTemp, Log, TEXT("Cleared occupancy on Tile: Q = %d, R = %d"), NewCharacter->CurrentTile->Q, NewCharacter->CurrentTile->R);
				UE_LOG(LogTemp, Log, TEXT("PlayerController possessed NewCharacter: %s"), *NewCharacter->GetName());
				NewCharacter->CurrentTile = nullptr;
			}

			NewCharacter->Destroy();

			if (UWorld* World = GetWorld())
			{
				if (AProjectKLBGameMode* GM = Cast<AProjectKLBGameMode>(World->GetAuthGameMode()))
				{
					if (GM->TurnManagerInstance)
					{
						AProjectKLBCharacter* NextPawn = GM->TurnManagerInstance->SwitchToNextPlayer();
						if (NextPawn)
						{
							Possess(NextPawn);
							UE_LOG(LogTemp, Log, TEXT("PlayerController possessed next Pawn: %s"), *NextPawn->GetName());
							SetViewTargetWithBlend(NextPawn, 0.5f);
							return;
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("No next player found in TurnManager."));
						}
					}
				}
			}
			
		}
	}


	// ���� �ð� ���� �� StartTurn ȣ��
	if (GetWorld())
	{
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AProjectKLBPlayerController::StartTurn, 0.1f, false);
	}

	// �ٷ� ȣ���ϸ� ������ �߻��� �� �����Ƿ� 0.5�� ���� �Ŀ� UpdateEquipmentUI()�� ȣ���մϴ�.
	if (GetWorld())
	{
		FTimerHandle EquipmentHandle;
		GetWorld()->GetTimerManager().SetTimer(
			EquipmentHandle,
			this,
			&AProjectKLBPlayerController::UpdateEquipmentUI,
			0.1f,    // 0.5�� ����
			false    // �� �� ���� ȣ��
		);
	}

	
	if (NewCharacter)
	{
		// ���ο� ĳ������ HealthWidgetComponent���� ������ ������ ������Ʈ
		if (NewCharacter->HealthWidgetComponent)
		{
			UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(NewCharacter->HealthWidgetComponent->GetWidget());
			if (HealthWidget)
			{
				float HealthPercent = (NewCharacter->MaxHP > 0) ? (NewCharacter->CurrentHP / NewCharacter->MaxHP) : 0.0f;
				HealthWidget->UpdateHealthBar(HealthPercent);
				UE_LOG(LogTemp, Log, TEXT("OnPossess: Updated HealthBar for character %s with health percent: %f"),
					*NewCharacter->GetName(), HealthPercent);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("OnPossess: HealthWidget is null for character %s."), *NewCharacter->GetName());
			}
		}
	}

}




void AProjectKLBPlayerController::MovePlayerUsingCard(const FCardData& CardData)
{
	//bCardUsed = true; // ī�� ��� ���� Ȱ��ȭ

	if (!GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager is not initialized."));
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("No controlled pawn found."));
		return;
	}

	FVector CurrentPosition = ControlledPawn->GetActorLocation();
	CurrentTile = GridManager->GetTileAtLocation(CurrentPosition); // ���� Ÿ�� ��ü�� ����

	if (!CurrentTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find CurrentTile."));
		return;
	}

	// ���� (Q,R) ��������
	FIntPoint CurrentCoord(CurrentTile->Q, CurrentTile->R);
	TArray<FIntPoint> PossibleTiles;

	// �� ����(���簢�� offset vs Axial) Ȯ��
	bool bIsOffset = GridManager->bShapedGrid;
	// (�Ǵ� GridManager->GridShape == EGridShape::Rectangular / EGridShape::Hexagonal ������)

	// �̵� ������ ���� �б�
	if (CardData.MoveType == TEXT("HexRange"))
	{
		if (bIsOffset)
		{
			AddHexRangeTiles_Offset(CurrentCoord, PossibleTiles);
		}
		else
		{
			AddHexRangeTiles_Axial(CurrentCoord, PossibleTiles);
		}
	}
	else if (CardData.MoveType == TEXT("Diagonal"))
	{
		if (bIsOffset)
		{
			AddDiagonalTiles_Offset(CurrentCoord, PossibleTiles);
		}
		else
		{
			AddDiagonalTiles_Axial(CurrentCoord, PossibleTiles);
		}
	}
	else if (CardData.MoveType == TEXT("Knight"))
	{
		if (bIsOffset)
		{
			AddKnightMoves_Offset(CurrentCoord, PossibleTiles);
		}
		else
		{
			AddKnightMoves_Axial(CurrentCoord, PossibleTiles);
		}
	}
	else if (CardData.MoveType == TEXT("Lost"))
	{
		if (bIsOffset)
		{
			AddLostMoves_Offset(CurrentCoord, PossibleTiles);
		}
		else
		{
			AddLostMoves_Axial(CurrentCoord, PossibleTiles);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown MoveType: %s"), *CardData.MoveType.ToString());
		return;
	}

	// ���� ǥ��
	EnableCardMovement(PossibleTiles);
}

 //�����̵� 1ĭ�̵����� ����
void AProjectKLBPlayerController::AddHexRangeTiles_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// ������ 1ĭ �̵� ������ ���� (Ȧ�� ��) (5,5 ����)
	TArray<FIntPoint> EvenColumnOffsets = {
		{0, -1},   // �� (5,4)
		{0, 1},   // �� (5,6)
		{1, 0}, // �����밢 (6,5)
		{1, 1},  // �������밢 (6,6)
		{-1, 0},  // �޾Ʒ��밢 (4,5)
		{-1, 1}   // �����Ʒ��밢 (4,6)

	};

	// ������ 1ĭ �̵� ������ ���� (¦�� ��) (6,5 ����)
	TArray<FIntPoint> OddColumnOffsets = {
		{0, 1},   // �� (6,4)
		{0, -1},   // �� (6,6)
		{1, -1}, // �����밢 (7,4)
		{1, 0},  // �������밢 (7,5)
		{-1, -1},  // �޾Ʒ��밢 (5,4)
		{-1, 0}   // �����Ʒ��밢 (5,5)
	};

	// �÷��̾��� ��(Q��)�� ¦������ Ȧ������ Ȯ��
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// �� �������� ����Ʈ �̵� Ÿ�� ���
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// Ÿ���� �����ϰ� ��ȿ�� ��� �߰�
		ATile* Tile = GridManager->GetTileAt(TargetQ, TargetR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(TargetQ, TargetR));
			UE_LOG(LogTemp, Log, TEXT("Knight Tile Added: Q = %d, R = %d"), TargetQ, TargetR);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid or Non-Walkable Knight Tile: Q = %d, R = %d"), TargetQ, TargetR);
		}
	}
}

// �밢�� �̵� 3ĭ�̵�
void AProjectKLBPlayerController::AddDiagonalTiles_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// ������ 3ĭ �̵� ������ ���� (Ȧ�� ��) (5,5 ����)
	TArray<FIntPoint> EvenColumnOffsets = {
		{0, -3},   // �� (5,2)
		{0, 3},   // �� (5,8)
		{3, -1}, // �����밢 (8,4)
		{3, 2},  // �������밢 (8,7)
		{-3, -1},  // �޾Ʒ��밢 (2,4)
		{-3, 2}   // �����Ʒ��밢 (2,7)

	};

	// ������ 3ĭ �̵� ������ ���� (¦�� ��) (6,5 ����)
	TArray<FIntPoint> OddColumnOffsets = {
		{0, -3},   // �� (6,2)
		{0, 3},   // �� (6,8)
		{3, -2}, // �����밢 (9,3)
		{3, 1},  // �������밢 (9,6)
		{-3, -2},  // �޾Ʒ��밢 (3,3)
		{-3, 1}   // �����Ʒ��밢 (3,6)
	};

	// �÷��̾��� ��(Q��)�� ¦������ Ȧ������ Ȯ��
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// �� �������� ����Ʈ �̵� Ÿ�� ���
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// Ÿ���� �����ϰ� ��ȿ�� ��� �߰�
		ATile* Tile = GridManager->GetTileAt(TargetQ, TargetR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(TargetQ, TargetR));
			UE_LOG(LogTemp, Log, TEXT("Knight Tile Added: Q = %d, R = %d"), TargetQ, TargetR);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid or Non-Walkable Knight Tile: Q = %d, R = %d"), TargetQ, TargetR);
		}
	}
}

// ����Ʈó�� �̵�
void AProjectKLBPlayerController::AddKnightMoves_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// ������ ����Ʈ ���� ������ ���� (Ȧ�� ��) (5,5 ����)
	TArray<FIntPoint> EvenColumnOffsets = {
		{3, 1},   // 12�� (8,6)
		{1, 3},   // 1�� (6,8)
		{-1, -2}, // 9�� (4,3)
		{2, -2},  // 11�� (7,3)
		{-3, 0},  // 7�� (2,5)
		{-2, 2}   // 5�� (3,7)
		
	};
	
	// ������ ����Ʈ ���� ������ ���� (¦�� ��) (6,5 ����)
	TArray<FIntPoint> OddColumnOffsets = {
		{3, 0},   // 12�� (9,5)
		{1, 2},   // 1�� (7,7)
		{-1, -3}, // 9�� (5,2)
		{2, -2},  // 11�� (8,3)
		{-3, -1},  // 7�� (3,4)
		{-2, 2}   // 5�� (4,7)
	};

	// �÷��̾��� ��(Q��)�� ¦������ Ȧ������ Ȯ��
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// �� �������� ����Ʈ �̵� Ÿ�� ���
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// Ÿ���� �����ϰ� ��ȿ�� ��� �߰�
		ATile* Tile = GridManager->GetTileAt(TargetQ, TargetR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(TargetQ, TargetR));
			UE_LOG(LogTemp, Log, TEXT("Knight Tile Added: Q = %d, R = %d"), TargetQ, TargetR);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid or Non-Walkable Knight Tile: Q = %d, R = %d"), TargetQ, TargetR);
		}
	}
}

// �ν�Ʈ �̵�
void AProjectKLBPlayerController::AddLostMoves_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// ������ �ν�Ʈ ���� ������ ���� (Ȧ�� ��) (5,5 ����)
	TArray<FIntPoint> EvenColumnOffsets = {
		{1, -1},   //  ���� (6,4)
		{-1, -1},   //   �� �Ʒ� (4,4)
		{2, 0}, //   12�� (7,5)
		{-2, 0},  //   6�� (3,5)
		{-1, 2},  //   ���� �Ʒ� (4,7)
		{1, 2}   //   ���� �� (6,7)

	};

	// ������ �ν�Ʈ ���� ������ ���� (¦�� ��) (6,5 ����)
	TArray<FIntPoint> OddColumnOffsets = {
		{1, -2},   //   ���� (7,3)
		{-1, -2},   //   �� �Ʒ� (5,3)
		{2, 0}, //   12�� (8,5)
		{-2, 0},  //   6�� (4,5)
		{-1, 1},  //   ���� �Ʒ� (5,6)
		{1, 1}   //   ���� �� (7,6)
	};

	// �÷��̾��� ��(Q��)�� ¦������ Ȧ������ Ȯ��
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// �� �������� ����Ʈ �̵� Ÿ�� ���
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// Ÿ���� �����ϰ� ��ȿ�� ��� �߰�
		ATile* Tile = GridManager->GetTileAt(TargetQ, TargetR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(TargetQ, TargetR));
			UE_LOG(LogTemp, Log, TEXT("Knight Tile Added: Q = %d, R = %d"), TargetQ, TargetR);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid or Non-Walkable Knight Tile: Q = %d, R = %d"), TargetQ, TargetR);
		}
	}
}

void AProjectKLBPlayerController::AddHexRangeTiles_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles)
{
	// Axial(Pointy-Top)������ 6���� ������
	// (q+1, r), (q-1, r), (q, r+1), (q, r-1), (q+1, r-1), (q-1, r+1)
	static TArray<FIntPoint> AxialOffsets = {
		{+1,  0},
		{-1,  0},
		{ 0, +1},
		{ 0, -1},
		{+1, -1},
		{-1, +1}
	};

	int32 q = CurrentTileCoord.X;
	int32 r = CurrentTileCoord.Y;

	for (auto& Off : AxialOffsets)
	{
		int32 newQ = q + Off.X;
		int32 newR = r + Off.Y;

		// �׸��� �Ŵ������� (newQ, newR)�� �ش��ϴ� Ÿ�� ��������
		ATile* Tile = GridManager->GetTileAt(newQ, newR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(newQ, newR));
		}
	}
}

// (2) Diagonal(3ĭ �̵�) �� Axial���� 6���� * 3
void AProjectKLBPlayerController::AddDiagonalTiles_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles)
{
	static TArray<FIntPoint> AxialDirections = {
		{+1,  0},
		{-1,  0},
		{ 0, +1},
		{ 0, -1},
		{+1, -1},
		{-1, +1}
	};

	int32 q = CurrentTileCoord.X;
	int32 r = CurrentTileCoord.Y;

	for (auto& Dir : AxialDirections)
	{
		// 3ĭ �̵�
		int32 newQ = q + 3 * Dir.X;
		int32 newR = r + 3 * Dir.Y;

		ATile* Tile = GridManager->GetTileAt(newQ, newR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(newQ, newR));
		}
	}
}

// (3) KnightMoves (Axial ����)
//   - ��: (��2, ��1), (��1, ��2) ������ ������ �� ����
void AProjectKLBPlayerController::AddKnightMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles)
{
	// ����: Axial���� "����Ʈ"ó�� �̵��ϴ� 12����(Ȥ�� 6����) ���� (���� ��Ģ�� ���� ����)
	// ���⼭�� (��2, ��1), (��1, ��2) ������ ��� �־��ٰ� ����
	static TArray<FIntPoint> KnightOffsetsAxial = {
		{1, 2},   //  ���� (1,2)
		{-2, 3},   //   �� �Ʒ� (-2,3)
		{3, -1}, //   12�� (3,-1)
		{2, -3},  //   6�� (2,-3)
		{-1, -2},  //   ���� �Ʒ� (-1,-2)
		{-3, 1}   //   ���� �� (-3,1)
		// �ʿ��ϴٸ� �߰�...
	};

	int32 q = CurrentTileCoord.X;
	int32 r = CurrentTileCoord.Y;

	for (auto& Off : KnightOffsetsAxial)
	{
		int32 newQ = q + Off.X;
		int32 newR = r + Off.Y;

		ATile* Tile = GridManager->GetTileAt(newQ, newR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(newQ, newR));
		}
	}
}

// (4) LostMoves (Axial ����) �� ��Ģ�� ���� ����
void AProjectKLBPlayerController::AddLostMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles)
{
	// "Lost" �̵� ����(��: 2��, 10��...)�� �������� ��Ȯ�� �𸣳�,
	// ���⼭�� ���Ƿ� 2ĭ �̵� ����� Axial�� �����صдٰ� ����
	static TArray<FIntPoint> LostOffsetsAxial = {
		// ��: 2ĭ ��/�Ʒ�, 1ĭ �밢 ���
		{-1, -1},  { -2, 1},
		{+1, +1}, { 2, -1},
		{1, -2}, { -1, 2}
		// �ʿ��ϸ� �߰�...
	};

	int32 q = CurrentTileCoord.X;
	int32 r = CurrentTileCoord.Y;

	for (auto& Off : LostOffsetsAxial)
	{
		int32 newQ = q + Off.X;
		int32 newR = r + Off.Y;

		ATile* Tile = GridManager->GetTileAt(newQ, newR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(newQ, newR));
		}
	}
}



void AProjectKLBPlayerController::SmoothMoveUpdate()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter) return;


	// �ִ� �ν��Ͻ��� ������ Speed ����
	if (UWolfAnimInstance* AnimBP = Cast<UWolfAnimInstance>(ControlledCharacter->GetMesh()->GetAnimInstance()))
	{
		AnimBP->bShouldMove = true;
		// �̵� �߿��� ���� �ӵ��� ���� (��: 200.0f)
		AnimBP->Speed = 500.0f;
		
	}

	MoveProgress += MoveUpdateInterval / StepDelay;

	if (MoveProgress >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		ControlledCharacter->SetActorLocation(EndLocation);
		MoveProgress = 0.0f;


		// �̵� �Ϸ� �� Speed 0���� ����
		if (UWolfAnimInstance* AnimBP = Cast<UWolfAnimInstance>(ControlledCharacter->GetMesh()->GetAnimInstance()))
		{
			AnimBP->Speed = 0.0f;
			AnimBP->bShouldMove = false;
		}

		// ���� �̵� �ܰ� ����
		MoveNextInSequence();
	}
	else
	{
		FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, MoveProgress);
		NewLocation.Z = StartLocation.Z;
		ControlledCharacter->SetActorLocation(NewLocation);
	}
}

// ��ǥ Yaw�� �÷��̾ ȸ����Ű�� �����ϴ� �Լ�
void AProjectKLBPlayerController::StartRotation(float TargetYaw)
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter) return;

	FRotator CurrentRotation = ControlledCharacter->GetActorRotation();
	TargetRotation = FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll);

	GetWorld()->GetTimerManager().SetTimer(
		RotationTimerHandle,
		this,
		&AProjectKLBPlayerController::RotateUpdate,
		MoveUpdateInterval,
		true
	);
}

// ȸ���� ������Ʈ�ϴ� �Լ� (Ÿ�̸� �ݹ�)
void AProjectKLBPlayerController::RotateUpdate()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter) return;

	float DeltaYaw = RotationSpeed * MoveUpdateInterval;
	FRotator CurrentRotation = ControlledCharacter->GetActorRotation();
	float YawDifference = FMath::Abs(FRotator::NormalizeAxis(TargetRotation.Yaw - CurrentRotation.Yaw));

	if (YawDifference < DeltaYaw)
	{
		ControlledCharacter->SetActorRotation(TargetRotation);
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);

		// ȸ�� �Ϸ� �� �ܰ� �ε��� ����
		CurrentRotationStep++;

		// ȸ�� �Ϸ� �� ù ��° �̵� �ܰ� ����
		MoveOneStep();
	}
	else
	{
		float YawStep = FMath::Clamp(FRotator::NormalizeAxis(TargetRotation.Yaw - CurrentRotation.Yaw), -DeltaYaw, DeltaYaw);
		FRotator NewRotation = CurrentRotation + FRotator(0, YawStep, 0);
		ControlledCharacter->SetActorRotation(NewRotation);
	}
}

ATile* AProjectKLBPlayerController::FindNeighborInRelativeDirection(ERelativeDirection RelDir)
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter || !CurrentTile || !GridManager) return nullptr;

	// �÷��̾��� ����� ���� �� ������ ���� ���
	FVector Forward = ControlledCharacter->GetActorForwardVector();
	Forward.Z = 0;
	Forward.Normalize();
	FVector Up = FVector::UpVector;
	FVector Right = FVector::CrossProduct(Up, Forward).GetSafeNormal();

	// ��� ���� ���� ���
	FVector DesiredDir;
	switch (RelDir)
	{
	case ERelativeDirection::TwoOClock:
		DesiredDir = (Forward + Right).GetSafeNormal();
		break;
	case ERelativeDirection::TwelveOClock:
		DesiredDir = Forward;
		break;
	case ERelativeDirection::TenOClock:
		DesiredDir = (Forward - Right).GetSafeNormal();
		break;
	default:
		DesiredDir = Forward;
		break;
	}

	// ���� Ÿ���� ���� Ÿ�ϵ� ��������
	TArray<ATile*> Neighbors = GridManager->GetNeighboringTiles(CurrentTile);
	ATile* BestTile = nullptr;
	float SmallestAngle = FLT_MAX;
	FVector PlayerLocation = ControlledCharacter->GetActorLocation();

	for (ATile* Tile : Neighbors)
	{
		if (!Tile) continue;
		FVector ToTile = Tile->GetActorLocation() - PlayerLocation;
		ToTile.Z = 0;
		ToTile.Normalize();
		float Angle = FMath::Acos(FVector::DotProduct(DesiredDir, ToTile));
		if (Angle < SmallestAngle)
		{
			SmallestAngle = Angle;
			BestTile = Tile;
		}
	}
	return BestTile;
}

void AProjectKLBPlayerController::MoveOneRelativeStep(ERelativeDirection RelDir)
{
	ATile* NextTile = FindNeighborInRelativeDirection(RelDir);
	if (!NextTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("No neighbor found in the desired direction."));
		return;
	}

	// ���� Ÿ�Ͽ� �����ϴ��� Ȯ��
	if (NextTile->bHasEnemy)
	{
		UE_LOG(LogTemp, Log, TEXT("NextTile has enemy."));
		AEnemy* Enemy = NextTile->GetOccupyingEnemy();
		if (Enemy)
		{
			UE_LOG(LogTemp, Log, TEXT("Attacking enemy: %s"), *Enemy->GetName());
			AttackSpecificEnemy(Enemy);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy pointer is null even though bHasEnemy is true."));
		}

		// �� �߰� �� �̵� �ߴ�
		DirectionSequence.Empty();
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("NextTile does not have an enemy."));
	}

	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter) return;

	AProjectKLBCharacter* NewCharacter = Cast<AProjectKLBCharacter>(ControlledCharacter);
	UE_LOG(LogTemp, Log, TEXT("ControlledCharacter NewCharacter: %s"), *NewCharacter->GetName());


	// ���� Ÿ�� ���� ������Ʈ
	if (CurrentTile)
		CurrentTile->SetHasPlayer(false);

	NextTile->SetHasPlayer(true);
	CurrentTile = NextTile;

	NewCharacter->CurrentTile = NextTile;

	// �ε巯�� �̵��� ���� �ʱⰪ ����
	StartLocation = ControlledCharacter->GetActorLocation();
	EndLocation = NextTile->GetActorLocation();

	float FixedZ = StartLocation.Z;
	StartLocation.Z = FixedZ;
	EndLocation.Z = FixedZ;

	MoveProgress = 0.0f;

	// �ε巯�� �̵� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&AProjectKLBPlayerController::SmoothMoveUpdate,
		MoveUpdateInterval,
		true
	);
}

void AProjectKLBPlayerController::StartKnightMovement()
{
	// ����Ʈ �̵� ���� ������ ����: 2��, 2��, 10��
	DirectionSequence = { ERelativeDirection::TwoOClock, ERelativeDirection::TwoOClock, ERelativeDirection::TenOClock };
	CurrentSequenceIndex = 0;
	MoveNextInSequence();
}

void AProjectKLBPlayerController::StartHexRangeMovement()
{
	// 12�� ������ �÷��̾��� ������ �ǹ��ϹǷ�,
   // ERelativeDirection::TwelveOClock�� ����Ͽ� �� ĭ �̵�
	MoveOneRelativeStep(ERelativeDirection::TwelveOClock);
}

void AProjectKLBPlayerController::StartDiagonalMovement()
{
	// 12�� �������� 3ĭ �̵��� ���� ������ ����
	DirectionSequence.Empty();
	for (int i = 0; i < 3; ++i)
	{
		DirectionSequence.Add(ERelativeDirection::TwelveOClock);
	}
	CurrentSequenceIndex = 0;

	// �������� �����Ͽ� ù ��° �̵� ����
	MoveNextInSequence();
}

void AProjectKLBPlayerController::StartLostMovement()
{
	// �ν�Ʈ �̵� ���� ������ ����: 2��, 10��
	DirectionSequence = { ERelativeDirection::TwoOClock, ERelativeDirection::TenOClock };
	CurrentSequenceIndex = 0;
	MoveNextInSequence();
}

void AProjectKLBPlayerController::MoveNextInSequence()
{
	if (CurrentSequenceIndex >= DirectionSequence.Num())
	{
		// ��� �̵� �Ϸ�
		CallEndTurn();

		return;
	}

	ERelativeDirection RelDir = DirectionSequence[CurrentSequenceIndex];
	CurrentSequenceIndex++;
	MoveOneRelativeStep(RelDir);
}

// �÷��̾ ������ Ÿ�ϱ��� ��θ� ���� �̵��� �����ϴ� �Լ�
void AProjectKLBPlayerController::MoveCharacterAlongPath(const TArray<ATile*>& Path, const FCardData& CardData)
{
	if (Path.Num() == 0) return;

	MovePath = Path;

	// ī�� Ÿ�Կ� ���� �б�
	if (CardData.MoveType == TEXT("Knight"))
	{
		StartKnightMovement();
	}
	else if (CardData.MoveType == TEXT("HexRange"))
	{
		StartHexRangeMovement();
	}
	else if (CardData.MoveType == TEXT("Diagonal"))
	{
		StartDiagonalMovement();
	}
	else if (CardData.MoveType == TEXT("Lost"))
	{
		StartLostMovement();
	}
	// �ٸ� ī�� Ÿ�Կ� ���� ó�� �߰�...
	else
	{
		return;
		// �⺻ �̵� ó�� �Ǵ� ���� ó��
	}
}

// �� �ܰ��� �̵� �� ȸ���� ó���ϴ� �Լ�
void AProjectKLBPlayerController::MoveOneStep()
{
	if (CurrentPathIndex >= MovePath.Num())
	{
		// ��� �̵� �Ϸ�
		return;
	}

	ATile* NextTile = MovePath[CurrentPathIndex];
	if (!NextTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("NextTile is null!"));
		return;
	}

	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter) return;

	// �÷��̾ ���� Ÿ���� �ٶ󺸵��� ȸ�� ����
	{
		FVector Direction = NextTile->GetActorLocation() - ControlledCharacter->GetActorLocation();
		Direction.Z = 0; // ���� ���⸸ ���
		if (!Direction.IsNearlyZero())
		{
			TargetRotation = Direction.Rotation();
			ControlledCharacter->SetActorRotation(TargetRotation);
		}
	}

	// Ÿ�� ���� ���� ������Ʈ
	if (CurrentTile)
	{
		CurrentTile->SetHasPlayer(false);
	}
	NextTile->SetHasPlayer(true);
	CurrentTile = NextTile;

	// �ε巯�� �̵��� ���� �ʱⰪ ����
	StartLocation = ControlledCharacter->GetActorLocation();
	EndLocation = NextTile->GetActorLocation();

	// Z�� �� ���� (�ٴ��� ���� �ʵ���)
	float FixedZ = StartLocation.Z;
	StartLocation.Z = FixedZ;
	EndLocation.Z = FixedZ;

	MoveProgress = 0.0f;

	// �ε巯�� �̵� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&AProjectKLBPlayerController::SmoothMoveUpdate,
		MoveUpdateInterval,
		true
	);

	// ���� ��� Ÿ�Ϸ� �̵� �غ�
	CurrentPathIndex++;
}

void AProjectKLBPlayerController::MoveToFirstValidTile(const TArray<FIntPoint>& PossibleTiles, APawn* ControlledPawn)
{
	for (const FIntPoint& Tile : PossibleTiles)
	{
		ATile* TargetTile = GridManager->GetTileAt(Tile.X, Tile.Y);
		if (TargetTile && TargetTile->IsWalkable())
		{
			FVector TargetPosition = TargetTile->GetActorLocation();
			ControlledPawn->SetActorLocation(TargetPosition);
			
			UE_LOG(LogTemp, Log, TEXT("Moved to Tile: Q=%d, R=%d"), Tile.X, Tile.Y);
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No valid tile found for movement."));
}

void AProjectKLBPlayerController::EnableCardMovement(const TArray<FIntPoint>& PossibleTiles)
{
	ValidTiles.Empty();
	HighlightedTiles.Empty(); // �߰�: ������ Ÿ�� ��ǥ �ʱ�ȭ

	for (const FIntPoint& TileCoord : PossibleTiles)
	{
		ATile* Tile = GridManager->GetTileAt(TileCoord.X, TileCoord.Y);
		if (Tile && Tile->IsWalkable())
		{
			ValidTiles.Add(Tile);
			HighlightedTiles.Add(TileCoord); // ������ Ÿ�� ��ǥ �߰�
			Tile->Highlight(true);          // Ÿ�� ���� ǥ��
			UE_LOG(LogTemp, Log, TEXT("Highlighted Tile: Q = %d, R = %d"), TileCoord.X, TileCoord.Y);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile Not Highlighted: Q = %d, R = %d"), TileCoord.X, TileCoord.Y);
		}
	}

	if (ValidTiles.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid tiles found for movement."));
	}

}

void AProjectKLBPlayerController::ResetCardMovement()
{
	//CardUsed = false; // ī�� ��� ���� ��Ȱ��ȭ
	for (ATile* Tile : ValidTiles)
	{
		if (Tile)
		{
			Tile->Highlight(false); // ���� ����
		}
	}
	ValidTiles.Empty(); // ��� �ʱ�ȭ
}



void AProjectKLBPlayerController::CheckSurroundingTilesForEnemyAndAttack()
{
	if (!GridManager || !GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager or Pawn is null"));
		return;
	}

	// ���� �÷��̾ ��ġ�� Ÿ�� ��������
	FVector PlayerLocation = GetPawn()->GetActorLocation();
	CurrentTile = GridManager->GetTileAtLocation(PlayerLocation);

	if (!CurrentTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentTile is null"));
		return;
	}

	// �ֺ� 6Ÿ�� ��������
	TArray<ATile*> NeighborTiles = GridManager->GetNeighboringTiles(CurrentTile);

	// �ֺ� 6Ÿ�Ͽ��� �� �˻�
	for (ATile* Tile : NeighborTiles)
	{
		if (Tile && Tile->bHasEnemy) // bHasEnemy�� Ÿ�Ͽ� ���� �ִ��� ��Ÿ���� �Ҹ��� ��
		{
			AEnemy* Enemy = Tile->GetOccupyingEnemy(); // �� Actor�� �������� �Լ�
			if (Enemy)
			{
				// ���� ����
				AttackSpecificEnemy(Enemy);
				UE_LOG(LogTemp, Log, TEXT("Attacked enemy at Tile: Q=%d, R=%d"), Tile->Q, Tile->R);

				return; // ���� �� ����
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("No enemies found in surrounding tiles"));
}

void AProjectKLBPlayerController::InventoryOn()
{
	if (CombatUI->InventoryWidget)
	{
		CombatUI->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (CombatUI->EquipmentWidget)
	{
		CombatUI->EquipmentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AProjectKLBPlayerController::InventoryOff()
{
	if (CombatUI->InventoryWidget)
	{
		CombatUI->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (CombatUI->EquipmentWidget)
	{
		CombatUI->EquipmentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AProjectKLBPlayerController::SwapInventoryItems(int32 FromIndex, int32 ToIndex)
{
	if (FromIndex < 0 || FromIndex >= Inventory.Num() || ToIndex < 0 || ToIndex >= Inventory.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapInventoryItems: Invalid indices."));
		return;
	}

	if (FromIndex == ToIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapInventoryItems: FromIndex and ToIndex are the same."));
		return;
	}

	FItemData& FromItem = Inventory[FromIndex];
	FItemData& ToItem = Inventory[ToIndex];

	bool bToEmpty = ToItem.Quantity <= 0 || ToItem.ItemName.IsEmpty();

	if (bToEmpty)
	{
		// �� �����̸�, �ܼ��� ������ �̵�
		ToItem = FromItem;
		// From ������ �� �������� �ʱ�ȭ
		FromItem.ItemName = "";
		FromItem.ItemDescription = "";
		FromItem.ItemIcon = nullptr;
		FromItem.Quantity = 0;
	}
	else
	{
		// �Ϲ����� ����
		Swap(FromItem, ToItem);
		UE_LOG(LogTemp, Log, TEXT("Swapped slot %d with slot %d"), FromIndex, ToIndex);
	}

	
	// �κ��丮 �迭 ���� ���
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Slot %d: %s, Quantity: %d"), i, *Inventory[i].ItemName, Inventory[i].Quantity);
	}

	// �κ��丮 ���� ��������Ʈ ���
	OnInventoryChanged.Broadcast(Inventory);
	UE_LOG(LogTemp, Log, TEXT("OnInventoryChanged Broadcasted."));
}

void AProjectKLBPlayerController::UpdateEquipmentUI()
{
	// ���� Possess�� Pawn�� ĳ�����ؼ� ĳ���ͷ� �����ɴϴ�.
	AProjectKLBCharacter* CurrentCharacter = Cast<AProjectKLBCharacter>(GetPawn());
	if (CurrentCharacter && CombatUI && CombatUI->EquipmentWidget)
	{
		CombatUI->EquipmentWidget->InitializeEquipment(CurrentCharacter);
		UE_LOG(LogTemp, Log, TEXT("Equipment UI updated for character: %s"), *CurrentCharacter->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateEquipmentUI: Character EquipmentWidget Not."));
	}
}

void AProjectKLBPlayerController::RemoveInventoryItem(int32 Index)
{
	if (Inventory.IsValidIndex(Index))
	{
		FItemData EmptyItem;
		EmptyItem.ItemName = "";
		EmptyItem.ItemDescription = "";
		EmptyItem.ItemIcon = nullptr;
		EmptyItem.Quantity = 0;
		EmptyItem.MaxQuantity = 0;
		Inventory[Index] = EmptyItem;
		OnInventoryChanged.Broadcast(Inventory);
		UE_LOG(LogTemp, Log, TEXT("Removed item from inventory slot %d"), Index);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveInventoryItem: Invalid index %d"), Index);
	}
}

int32 AProjectKLBPlayerController::FindEmptyInventorySlot() const
{
	// Inventory �迭�� UI ���� ��(��: 40)�� �����Ǿ� �ִٰ� ����
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		// �� ���� ����: Quantity�� 0�̰ų� ItemName�� ���������
		if (Inventory[i].Quantity <= 0 || Inventory[i].ItemName.IsEmpty())
		{
			return i;
		}
	}
	return -1;
}

void AProjectKLBPlayerController::PCUpdateInventoryUI()
{
	// UWBP_Inventory ������ �ִٸ� ���ʱ�ȭ(��: InitializeInventory ȣ��)
	if (CombatUI->InventoryWidget)
	{
		CombatUI->InventoryWidget->InitializeInventory(Inventory);
		UE_LOG(LogTemp, Log, TEXT("InventoryWidget updated with %d items."), Inventory.Num());
	}
}

void AProjectKLBPlayerController::PCUpdateEquipmentUI()
{
	AProjectKLBCharacter* CurrentCharacter = Cast<AProjectKLBCharacter>(GetPawn());
	if (CurrentCharacter && CombatUI && CombatUI->EquipmentWidget)
	{
		CombatUI->EquipmentWidget->InitializeEquipment(CurrentCharacter);
		UE_LOG(LogTemp, Log, TEXT("Equipment UI updated for character: %s"), *CurrentCharacter->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateEquipmentUI: ĳ���ͳ� EquipmentWidget�� ��ȿ���� �ʽ��ϴ�."));
	}
}

void AProjectKLBPlayerController::PlayerHPUI()
{
	// ���ο� Pawn(�÷��̾� ĳ����)�� ��ȯ�Ǹ� HealthWidget�� �����մϴ�.
	AProjectKLBCharacter* NewCharacter = Cast<AProjectKLBCharacter>(GetPawn());
	if (NewCharacter)
	{
		// ���ο� ĳ������ HealthWidgetComponent���� ������ ������ ������Ʈ
		if (NewCharacter->HealthWidgetComponent)
		{
			UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(NewCharacter->HealthWidgetComponent->GetWidget());
			if (HealthWidget)
			{
				//NewCharacter->UpdateHealthBar();
				
				 //�� ĳ������ ü������ ü�¹ٸ� �ʱ�ȭ
			float HealthPercent = (NewCharacter->MaxHP > 0) ? (NewCharacter->CurrentHP / NewCharacter->MaxHP) : 0.0f;
				HealthWidget->UpdateHealthBar(HealthPercent);
				UE_LOG(LogTemp, Log, TEXT("OnPossess: Updated HealthBar for character %s with health percent: %f"),
					*NewCharacter->GetName(), HealthPercent);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("OnPossess: HealthWidget is null."));
			}
		}
	}

}

void AProjectKLBPlayerController::ClearCurrentTile(AProjectKLBCharacter* DeadPawn)
{
	if (CurrentTile)
	{
		CurrentTile->SetHasPlayer(false);
		UE_LOG(LogTemp, Warning, TEXT("Cleared occupancy on Tile: Q = %d, R = %d"), CurrentTile->Q, CurrentTile->R);
		CurrentTile = nullptr;


		
		DeadPawn->Destroy();
	}
}

void AProjectKLBPlayerController::UpdateTutorialStageClearUI()
{
	TutorialStageClearUI = CreateWidget<UTutorialStageClearUI>(this, TutorialStageClearUIClass);
	if (TutorialStageClearUI)
	{
		// ȭ�鿡 ���� �߰�
		TutorialStageClearUI->AddToViewport();
	}

}







