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
#include "Inventory/Equipment.h" // 장비창
#include "Player/CharacterStatComponent.h"
#include "Widget/HealthBarWidget.h"
#include "TutorialStageClearUI/TutorialStageClearUI.h"





DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// 나중에 플레이어 이동의 경우 Walkable의 bool 값을 조정해서 컨트롤

AProjectKLBPlayerController::AProjectKLBPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	

	// 기본 공격 데미지 설정
	AttackDamage = 20.0f;
}

void AProjectKLBPlayerController::BeginPlay()
{
	Super::BeginPlay();


	// GridManager 설정 (레벨에서 직접 배치했거나 동적으로 찾기)
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	if (!GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager not found!"));
	}
	
	// 입력 매핑 컨텍스트 등록
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

	// CurrentTile 초기화
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		FVector CurrentPosition = ControlledPawn->GetActorLocation();
		CurrentTile = GridManager->GetTileAtLocation(CurrentPosition);
		if (CurrentTile)
		{
			CurrentTile->SetHasPlayer(true); // 초기 위치 타일에 플레이어 설정
			UE_LOG(LogTemp, Log, TEXT("Initial CurrentTile set: Q = %d, R = %d"), CurrentTile->Q, CurrentTile->R);
		}
	}

	if (UWorld* World = GetWorld())
	{
		UDataGameInstance* GameInstance = Cast<UDataGameInstance>(World->GetGameInstance());
		if (GameInstance && CardListWidgetClass)
		{
			// 카드 리스트 위젯 생성 또는 찾기
			if (!CardListWidget) // 기존 위젯이 없으면 생성
			{
				CardListWidget = CreateWidget<UCardListWidget>(World, CardListWidgetClass);
			}

			if (CardListWidget) // 위젯이 존재하면 UI를 갱신
			{
				CardListWidget->UpdateCardUI();
			}
		}
	}

	// 게임 인스턴스에서 아이템 데이터 가져오기
	UDataGameInstance* GameInstance = GetWorld()->GetGameInstance<UDataGameInstance>();
	if (GameInstance)
	{
		Inventory = GameInstance->GetDisplayedItems();
		UE_LOG(LogTemp, Log, TEXT("Inventory initialized with %d items."), Inventory.Num());

		// 인벤토리 변경 델리게이트 방송
		OnInventoryChanged.Broadcast(Inventory);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get GameInstance."));
	}

	// 초기 인벤토리 설정 (예시)


	if (CombatUIClass)  // UPROPERTY(EditDefaultsOnly)로 지정된 CombatUI Blueprint 클래스
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

	// UI에서 사용할 총 슬롯 수 (예: 8 * 5 = 40)
	const int32 TotalSlots = 40;
	Inventory.SetNum(TotalSlots);  // Inventory 배열의 크기를 40으로 고정


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
		// 입력 액션 바인딩
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
	// 카드 UI 표시
	if (CardListWidget)
	{
		CardListWidget->UpdateCardUI(); // UI 갱신
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
		CardListWidget->UpdateCardUI(); // UI 갱신
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
			// 카드 리스트 위젯 생성 또는 찾기
			if (!CardListWidget) // 기존 위젯이 없으면 생성
			{
				CardListWidget = CreateWidget<UCardListWidget>(World, CardListWidgetClass);
				if (CardListWidget)
				{
					// 화면에 위젯 추가
					CardListWidget->AddToViewport();
				}
			}

			if (CardListWidget) // 위젯이 존재하면 UI를 갱신
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

	// 카드 데이터에 따라 이동 가능한 타일 계산
	CurrentCardData = CardData;
	MovePlayerUsingCard(CardData);
	
	// 카드 사용 후 덱 갱신
	if (UDataGameInstance* GameInstance = GetWorld()->GetGameInstance<UDataGameInstance>())
	{
		GameInstance->UseCardAndShuffle(CardData);
	}

	// UI 숨기기
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
		// 클릭한 타일을 향해 플레이어 회전
		ACharacter* ControlledCharacter = GetCharacter();
		if (ControlledCharacter)
		{
			FVector Direction = ClickedTile->GetActorLocation() - ControlledCharacter->GetActorLocation();
			Direction.Z = 0;  // 수평면으로만 회전
			if (!Direction.IsNearlyZero())
			{
				TargetRotation = Direction.Rotation();
				ControlledCharacter->SetActorRotation(TargetRotation);
			}
		}

		// 기존: 경로 계산 및 이동 시작
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


// 가장 가까운 거리 말고 UI로 처리, 사람이 플레이어 클릭 했을 때, 타겟지정 나중에 원거리 공격 기능에 사용할 느낌..?
void AProjectKLBPlayerController::AttackEnemy()
{
	// UI 또는 타겟팅 시스템을 통해 공격할 적(TargetEnemy)을 가져온다고 가정합니다.
	AEnemy* TargetEnemy = nullptr; // 예: 타겟팅 시스템을 통해 할당

	// 현재 소유한 Pawn(플레이어 캐릭터) 가져오기
	AProjectKLBCharacter* MyCharacter = Cast<AProjectKLBCharacter>(GetPawn());

	if (MyCharacter && TargetEnemy)
	{
		// 플레이어의 스탯 컴퍼넌트에서 공격력 값을 가져옵니다.
		float DamageAmount = 0.0f;
		if (MyCharacter->StatComponent)
		{
			DamageAmount = MyCharacter->StatComponent->FinalStats.Attack;
			UE_LOG(LogTemp, Warning, TEXT("is Attack StatComp"));
		}
		else
		{
			// 만약 StatComponent가 없으면, 기본값을 사용할 수 있습니다.
			DamageAmount = AttackDamage; // 예: AttackDamage가 20.0f 등으로 설정되어 있다면.
		}

		// 적에게 데미지 적용 (TargetEnemy->Damage() 함수가 존재한다고 가정)
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
	// TargetTile이 null인지 확인
	if (!TargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetTile is null!"));
		return;
	}

	// 현재 위치한 타일의 bHasPlayer를 false로 설정
	if (CurrentTile)
	{
		CurrentTile->SetHasPlayer(false);
		UE_LOG(LogTemp, Log, TEXT("Set bHasPlayer to false for Tile: Q = %d, R = %d"), CurrentTile->Q, CurrentTile->R);
	}

	// TargetTile의 bHasPlayer를 true로 설정
	TargetTile->SetHasPlayer(true);
	UE_LOG(LogTemp, Log, TEXT("Set bHasPlayer to true for Tile: Q = %d, R = %d"), TargetTile->Q, TargetTile->R);

	// CurrentTile 업데이트
	CurrentTile = TargetTile;

	// 플레이어 이동 처리
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

	// 공격 데미지 처리
	// 현재 소유한 Pawn(플레이어 캐릭터) 가져오기
	AProjectKLBCharacter* MyCharacter = Cast<AProjectKLBCharacter>(GetPawn());

	if (MyCharacter && TargetEnemy)
	{
		// 플레이어의 스탯 컴퍼넌트에서 공격력 값을 가져옵니다.
		float DamageAmount = 0.0f;
		if (MyCharacter->StatComponent)
		{
			DamageAmount = MyCharacter->StatComponent->FinalStats.Attack;
			UE_LOG(LogTemp, Warning, TEXT("is Attack StatComp"));
		}
		else
		{
			// 만약 StatComponent가 없으면, 기본값을 사용할 수 있습니다.
			DamageAmount = AttackDamage; // 예: AttackDamage가 20.0f 등으로 설정되어 있다면.
		}

		// 적에게 데미지 적용 (TargetEnemy->Damage() 함수가 존재한다고 가정)
		TargetEnemy->Damage(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Attacked targeted enemy: %s with damage: %f"), *TargetEnemy->GetName(), DamageAmount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No enemy targeted for attack or character is invalid."));
	}
	 


	// 플레이어 캐릭터 얻기
	APawn* CharPawn = GetPawn();
	if (!CharPawn) return;

	AProjectKLBCharacter* ACharacter = Cast<AProjectKLBCharacter>(CharPawn);
	if (!ACharacter) return;

	// 애니메이션 몽타주 재생
	if (ACharacter->GetMesh() && ACharacter->GetMesh()->GetAnimInstance() && ACharacter->PlayerAttackMontage)
	{
		ACharacter->PlayAnimMontage(ACharacter->PlayerAttackMontage);
	}

	CallEndTurn();
}

// 타일 입력
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

// HUD 연결
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


	// 일정 시간 지연 후 StartTurn 호출
	if (GetWorld())
	{
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AProjectKLBPlayerController::StartTurn, 0.1f, false);
	}

	// 바로 호출하면 문제가 발생할 수 있으므로 0.5초 지연 후에 UpdateEquipmentUI()를 호출합니다.
	if (GetWorld())
	{
		FTimerHandle EquipmentHandle;
		GetWorld()->GetTimerManager().SetTimer(
			EquipmentHandle,
			this,
			&AProjectKLBPlayerController::UpdateEquipmentUI,
			0.1f,    // 0.5초 지연
			false    // 단 한 번만 호출
		);
	}

	
	if (NewCharacter)
	{
		// 새로운 캐릭터의 HealthWidgetComponent에서 위젯을 가져와 업데이트
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
	//bCardUsed = true; // 카드 사용 상태 활성화

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
	CurrentTile = GridManager->GetTileAtLocation(CurrentPosition); // 현재 타일 객체로 설정

	if (!CurrentTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find CurrentTile."));
		return;
	}

	// 현재 (Q,R) 가져오기
	FIntPoint CurrentCoord(CurrentTile->Q, CurrentTile->R);
	TArray<FIntPoint> PossibleTiles;

	// 맵 형태(직사각형 offset vs Axial) 확인
	bool bIsOffset = GridManager->bShapedGrid;
	// (또는 GridManager->GridShape == EGridShape::Rectangular / EGridShape::Hexagonal 식으로)

	// 이동 유형에 따른 분기
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

	// 강조 표시
	EnableCardMovement(PossibleTiles);
}

 //직선이동 1칸이동으로 변경
void AProjectKLBPlayerController::AddHexRangeTiles_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// 육각형 1칸 이동 오프셋 정의 (홀수 열) (5,5 기준)
	TArray<FIntPoint> EvenColumnOffsets = {
		{0, -1},   // 좌 (5,4)
		{0, 1},   // 우 (5,6)
		{1, 0}, // 왼위대각 (6,5)
		{1, 1},  // 오른위대각 (6,6)
		{-1, 0},  // 왼아래대각 (4,5)
		{-1, 1}   // 오른아래대각 (4,6)

	};

	// 육각형 1칸 이동 오프셋 정의 (짝수 열) (6,5 기준)
	TArray<FIntPoint> OddColumnOffsets = {
		{0, 1},   // 좌 (6,4)
		{0, -1},   // 우 (6,6)
		{1, -1}, // 왼위대각 (7,4)
		{1, 0},  // 오른위대각 (7,5)
		{-1, -1},  // 왼아래대각 (5,4)
		{-1, 0}   // 오른아래대각 (5,5)
	};

	// 플레이어의 열(Q값)이 짝수인지 홀수인지 확인
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// 각 방향으로 나이트 이동 타일 계산
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// 타일이 존재하고 유효한 경우 추가
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

// 대각선 이동 3칸이동
void AProjectKLBPlayerController::AddDiagonalTiles_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// 육각형 3칸 이동 오프셋 정의 (홀수 열) (5,5 기준)
	TArray<FIntPoint> EvenColumnOffsets = {
		{0, -3},   // 좌 (5,2)
		{0, 3},   // 우 (5,8)
		{3, -1}, // 왼위대각 (8,4)
		{3, 2},  // 오른위대각 (8,7)
		{-3, -1},  // 왼아래대각 (2,4)
		{-3, 2}   // 오른아래대각 (2,7)

	};

	// 육각형 3칸 이동 오프셋 정의 (짝수 열) (6,5 기준)
	TArray<FIntPoint> OddColumnOffsets = {
		{0, -3},   // 좌 (6,2)
		{0, 3},   // 우 (6,8)
		{3, -2}, // 왼위대각 (9,3)
		{3, 1},  // 오른위대각 (9,6)
		{-3, -2},  // 왼아래대각 (3,3)
		{-3, 1}   // 오른아래대각 (3,6)
	};

	// 플레이어의 열(Q값)이 짝수인지 홀수인지 확인
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// 각 방향으로 나이트 이동 타일 계산
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// 타일이 존재하고 유효한 경우 추가
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

// 나이트처럼 이동
void AProjectKLBPlayerController::AddKnightMoves_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// 육각형 나이트 방향 오프셋 정의 (홀수 열) (5,5 기준)
	TArray<FIntPoint> EvenColumnOffsets = {
		{3, 1},   // 12시 (8,6)
		{1, 3},   // 1시 (6,8)
		{-1, -2}, // 9시 (4,3)
		{2, -2},  // 11시 (7,3)
		{-3, 0},  // 7시 (2,5)
		{-2, 2}   // 5시 (3,7)
		
	};
	
	// 육각형 나이트 방향 오프셋 정의 (짝수 열) (6,5 기준)
	TArray<FIntPoint> OddColumnOffsets = {
		{3, 0},   // 12시 (9,5)
		{1, 2},   // 1시 (7,7)
		{-1, -3}, // 9시 (5,2)
		{2, -2},  // 11시 (8,3)
		{-3, -1},  // 7시 (3,4)
		{-2, 2}   // 5시 (4,7)
	};

	// 플레이어의 열(Q값)이 짝수인지 홀수인지 확인
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// 각 방향으로 나이트 이동 타일 계산
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// 타일이 존재하고 유효한 경우 추가
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

// 로스트 이동
void AProjectKLBPlayerController::AddLostMoves_Offset(const FIntPoint& Current_Tile, TArray<FIntPoint>& PossibleTiles)
{
	// 육각형 로스트 방향 오프셋 정의 (홀수 열) (5,5 기준)
	TArray<FIntPoint> EvenColumnOffsets = {
		{1, -1},   //  왼위 (6,4)
		{-1, -1},   //   왼 아래 (4,4)
		{2, 0}, //   12시 (7,5)
		{-2, 0},  //   6시 (3,5)
		{-1, 2},  //   오른 아래 (4,7)
		{1, 2}   //   오른 위 (6,7)

	};

	// 육각형 로스트 방향 오프셋 정의 (짝수 열) (6,5 기준)
	TArray<FIntPoint> OddColumnOffsets = {
		{1, -2},   //   왼위 (7,3)
		{-1, -2},   //   왼 아래 (5,3)
		{2, 0}, //   12시 (8,5)
		{-2, 0},  //   6시 (4,5)
		{-1, 1},  //   오른 아래 (5,6)
		{1, 1}   //   오른 위 (7,6)
	};

	// 플레이어의 열(Q값)이 짝수인지 홀수인지 확인
	const TArray<FIntPoint>& DiagonalOffsets = (Current_Tile.X % 2 == 0) ? OddColumnOffsets : EvenColumnOffsets;

	// 각 방향으로 나이트 이동 타일 계산
	for (const FIntPoint& Offset : DiagonalOffsets)
	{
		int32 TargetQ = Current_Tile.X + Offset.X;
		int32 TargetR = Current_Tile.Y + Offset.Y;

		// 타일이 존재하고 유효한 경우 추가
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
	// Axial(Pointy-Top)에서의 6방향 오프셋
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

		// 그리드 매니저에서 (newQ, newR)에 해당하는 타일 가져오기
		ATile* Tile = GridManager->GetTileAt(newQ, newR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(newQ, newR));
		}
	}
}

// (2) Diagonal(3칸 이동) → Axial에서 6방향 * 3
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
		// 3칸 이동
		int32 newQ = q + 3 * Dir.X;
		int32 newR = r + 3 * Dir.Y;

		ATile* Tile = GridManager->GetTileAt(newQ, newR);
		if (Tile && Tile->IsWalkable())
		{
			PossibleTiles.Add(FIntPoint(newQ, newR));
		}
	}
}

// (3) KnightMoves (Axial 버전)
//   - 예: (±2, ±1), (±1, ±2) 식으로 정의할 수 있음
void AProjectKLBPlayerController::AddKnightMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles)
{
	// 예시: Axial에서 "나이트"처럼 이동하는 12가지(혹은 6가지) 정의 (게임 규칙에 따라 조정)
	// 여기서는 (±2, ±1), (±1, ±2) 조합을 모두 넣었다고 가정
	static TArray<FIntPoint> KnightOffsetsAxial = {
		{1, 2},   //  왼위 (1,2)
		{-2, 3},   //   왼 아래 (-2,3)
		{3, -1}, //   12시 (3,-1)
		{2, -3},  //   6시 (2,-3)
		{-1, -2},  //   오른 아래 (-1,-2)
		{-3, 1}   //   오른 위 (-3,1)
		// 필요하다면 추가...
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

// (4) LostMoves (Axial 버전) → 규칙에 따라 정의
void AProjectKLBPlayerController::AddLostMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles)
{
	// "Lost" 이동 방향(예: 2시, 10시...)이 무엇인지 정확히 모르나,
	// 여기서는 임의로 2칸 이동 등등을 Axial로 정의해둔다고 가정
	static TArray<FIntPoint> LostOffsetsAxial = {
		// 예: 2칸 위/아래, 1칸 대각 등등
		{-1, -1},  { -2, 1},
		{+1, +1}, { 2, -1},
		{1, -2}, { -1, 2}
		// 필요하면 추가...
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


	// 애님 인스턴스를 가져와 Speed 설정
	if (UWolfAnimInstance* AnimBP = Cast<UWolfAnimInstance>(ControlledCharacter->GetMesh()->GetAnimInstance()))
	{
		AnimBP->bShouldMove = true;
		// 이동 중에는 일정 속도로 설정 (예: 200.0f)
		AnimBP->Speed = 500.0f;
		
	}

	MoveProgress += MoveUpdateInterval / StepDelay;

	if (MoveProgress >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		ControlledCharacter->SetActorLocation(EndLocation);
		MoveProgress = 0.0f;


		// 이동 완료 후 Speed 0으로 설정
		if (UWolfAnimInstance* AnimBP = Cast<UWolfAnimInstance>(ControlledCharacter->GetMesh()->GetAnimInstance()))
		{
			AnimBP->Speed = 0.0f;
			AnimBP->bShouldMove = false;
		}

		// 다음 이동 단계 시작
		MoveNextInSequence();
	}
	else
	{
		FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, MoveProgress);
		NewLocation.Z = StartLocation.Z;
		ControlledCharacter->SetActorLocation(NewLocation);
	}
}

// 목표 Yaw로 플레이어를 회전시키기 시작하는 함수
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

// 회전을 업데이트하는 함수 (타이머 콜백)
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

		// 회전 완료 시 단계 인덱스 증가
		CurrentRotationStep++;

		// 회전 완료 후 첫 번째 이동 단계 시작
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

	// 플레이어의 수평면 전방 및 오른쪽 벡터 계산
	FVector Forward = ControlledCharacter->GetActorForwardVector();
	Forward.Z = 0;
	Forward.Normalize();
	FVector Up = FVector::UpVector;
	FVector Right = FVector::CrossProduct(Up, Forward).GetSafeNormal();

	// 상대 방향 벡터 계산
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

	// 현재 타일의 인접 타일들 가져오기
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

	// 적이 타일에 존재하는지 확인
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

		// 적 발견 시 이동 중단
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


	// 현재 타일 상태 업데이트
	if (CurrentTile)
		CurrentTile->SetHasPlayer(false);

	NextTile->SetHasPlayer(true);
	CurrentTile = NextTile;

	NewCharacter->CurrentTile = NextTile;

	// 부드러운 이동을 위한 초기값 설정
	StartLocation = ControlledCharacter->GetActorLocation();
	EndLocation = NextTile->GetActorLocation();

	float FixedZ = StartLocation.Z;
	StartLocation.Z = FixedZ;
	EndLocation.Z = FixedZ;

	MoveProgress = 0.0f;

	// 부드러운 이동 타이머 설정
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
	// 나이트 이동 방향 시퀀스 설정: 2시, 2시, 10시
	DirectionSequence = { ERelativeDirection::TwoOClock, ERelativeDirection::TwoOClock, ERelativeDirection::TenOClock };
	CurrentSequenceIndex = 0;
	MoveNextInSequence();
}

void AProjectKLBPlayerController::StartHexRangeMovement()
{
	// 12시 방향은 플레이어의 전방을 의미하므로,
   // ERelativeDirection::TwelveOClock을 사용하여 한 칸 이동
	MoveOneRelativeStep(ERelativeDirection::TwelveOClock);
}

void AProjectKLBPlayerController::StartDiagonalMovement()
{
	// 12시 방향으로 3칸 이동을 위한 시퀀스 설정
	DirectionSequence.Empty();
	for (int i = 0; i < 3; ++i)
	{
		DirectionSequence.Add(ERelativeDirection::TwelveOClock);
	}
	CurrentSequenceIndex = 0;

	// 시퀀스를 시작하여 첫 번째 이동 실행
	MoveNextInSequence();
}

void AProjectKLBPlayerController::StartLostMovement()
{
	// 로스트 이동 방향 시퀀스 설정: 2시, 10시
	DirectionSequence = { ERelativeDirection::TwoOClock, ERelativeDirection::TenOClock };
	CurrentSequenceIndex = 0;
	MoveNextInSequence();
}

void AProjectKLBPlayerController::MoveNextInSequence()
{
	if (CurrentSequenceIndex >= DirectionSequence.Num())
	{
		// 모든 이동 완료
		CallEndTurn();

		return;
	}

	ERelativeDirection RelDir = DirectionSequence[CurrentSequenceIndex];
	CurrentSequenceIndex++;
	MoveOneRelativeStep(RelDir);
}

// 플레이어가 선택한 타일까지 경로를 따라 이동을 시작하는 함수
void AProjectKLBPlayerController::MoveCharacterAlongPath(const TArray<ATile*>& Path, const FCardData& CardData)
{
	if (Path.Num() == 0) return;

	MovePath = Path;

	// 카드 타입에 따른 분기
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
	// 다른 카드 타입에 대한 처리 추가...
	else
	{
		return;
		// 기본 이동 처리 또는 오류 처리
	}
}

// 한 단계의 이동 및 회전을 처리하는 함수
void AProjectKLBPlayerController::MoveOneStep()
{
	if (CurrentPathIndex >= MovePath.Num())
	{
		// 모든 이동 완료
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

	// 플레이어가 다음 타일을 바라보도록 회전 설정
	{
		FVector Direction = NextTile->GetActorLocation() - ControlledCharacter->GetActorLocation();
		Direction.Z = 0; // 수평 방향만 고려
		if (!Direction.IsNearlyZero())
		{
			TargetRotation = Direction.Rotation();
			ControlledCharacter->SetActorRotation(TargetRotation);
		}
	}

	// 타일 관련 상태 업데이트
	if (CurrentTile)
	{
		CurrentTile->SetHasPlayer(false);
	}
	NextTile->SetHasPlayer(true);
	CurrentTile = NextTile;

	// 부드러운 이동을 위한 초기값 설정
	StartLocation = ControlledCharacter->GetActorLocation();
	EndLocation = NextTile->GetActorLocation();

	// Z축 값 고정 (바닥을 뚫지 않도록)
	float FixedZ = StartLocation.Z;
	StartLocation.Z = FixedZ;
	EndLocation.Z = FixedZ;

	MoveProgress = 0.0f;

	// 부드러운 이동 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&AProjectKLBPlayerController::SmoothMoveUpdate,
		MoveUpdateInterval,
		true
	);

	// 다음 경로 타일로 이동 준비
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
	HighlightedTiles.Empty(); // 추가: 강조된 타일 좌표 초기화

	for (const FIntPoint& TileCoord : PossibleTiles)
	{
		ATile* Tile = GridManager->GetTileAt(TileCoord.X, TileCoord.Y);
		if (Tile && Tile->IsWalkable())
		{
			ValidTiles.Add(Tile);
			HighlightedTiles.Add(TileCoord); // 강조된 타일 좌표 추가
			Tile->Highlight(true);          // 타일 강조 표시
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
	//CardUsed = false; // 카드 사용 상태 비활성화
	for (ATile* Tile : ValidTiles)
	{
		if (Tile)
		{
			Tile->Highlight(false); // 강조 해제
		}
	}
	ValidTiles.Empty(); // 목록 초기화
}



void AProjectKLBPlayerController::CheckSurroundingTilesForEnemyAndAttack()
{
	if (!GridManager || !GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager or Pawn is null"));
		return;
	}

	// 현재 플레이어가 위치한 타일 가져오기
	FVector PlayerLocation = GetPawn()->GetActorLocation();
	CurrentTile = GridManager->GetTileAtLocation(PlayerLocation);

	if (!CurrentTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentTile is null"));
		return;
	}

	// 주변 6타일 가져오기
	TArray<ATile*> NeighborTiles = GridManager->GetNeighboringTiles(CurrentTile);

	// 주변 6타일에서 적 검색
	for (ATile* Tile : NeighborTiles)
	{
		if (Tile && Tile->bHasEnemy) // bHasEnemy는 타일에 적이 있는지 나타내는 불리언 값
		{
			AEnemy* Enemy = Tile->GetOccupyingEnemy(); // 적 Actor를 가져오는 함수
			if (Enemy)
			{
				// 공격 수행
				AttackSpecificEnemy(Enemy);
				UE_LOG(LogTemp, Log, TEXT("Attacked enemy at Tile: Q=%d, R=%d"), Tile->Q, Tile->R);

				return; // 공격 후 종료
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
		// 빈 슬롯이면, 단순히 아이템 이동
		ToItem = FromItem;
		// From 슬롯은 빈 슬롯으로 초기화
		FromItem.ItemName = "";
		FromItem.ItemDescription = "";
		FromItem.ItemIcon = nullptr;
		FromItem.Quantity = 0;
	}
	else
	{
		// 일반적인 스왑
		Swap(FromItem, ToItem);
		UE_LOG(LogTemp, Log, TEXT("Swapped slot %d with slot %d"), FromIndex, ToIndex);
	}

	
	// 인벤토리 배열 상태 출력
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Slot %d: %s, Quantity: %d"), i, *Inventory[i].ItemName, Inventory[i].Quantity);
	}

	// 인벤토리 변경 델리게이트 방송
	OnInventoryChanged.Broadcast(Inventory);
	UE_LOG(LogTemp, Log, TEXT("OnInventoryChanged Broadcasted."));
}

void AProjectKLBPlayerController::UpdateEquipmentUI()
{
	// 현재 Possess된 Pawn을 캐스팅해서 캐릭터로 가져옵니다.
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
	// Inventory 배열은 UI 슬롯 수(예: 40)와 고정되어 있다고 가정
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		// 빈 슬롯 조건: Quantity가 0이거나 ItemName이 비어있으면
		if (Inventory[i].Quantity <= 0 || Inventory[i].ItemName.IsEmpty())
		{
			return i;
		}
	}
	return -1;
}

void AProjectKLBPlayerController::PCUpdateInventoryUI()
{
	// UWBP_Inventory 위젯이 있다면 재초기화(예: InitializeInventory 호출)
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
		UE_LOG(LogTemp, Warning, TEXT("UpdateEquipmentUI: 캐릭터나 EquipmentWidget이 유효하지 않습니다."));
	}
}

void AProjectKLBPlayerController::PlayerHPUI()
{
	// 새로운 Pawn(플레이어 캐릭터)로 전환되면 HealthWidget을 갱신합니다.
	AProjectKLBCharacter* NewCharacter = Cast<AProjectKLBCharacter>(GetPawn());
	if (NewCharacter)
	{
		// 새로운 캐릭터의 HealthWidgetComponent에서 위젯을 가져와 업데이트
		if (NewCharacter->HealthWidgetComponent)
		{
			UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(NewCharacter->HealthWidgetComponent->GetWidget());
			if (HealthWidget)
			{
				//NewCharacter->UpdateHealthBar();
				
				 //새 캐릭터의 체력으로 체력바를 초기화
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
		// 화면에 위젯 추가
		TutorialStageClearUI->AddToViewport();
	}

}







