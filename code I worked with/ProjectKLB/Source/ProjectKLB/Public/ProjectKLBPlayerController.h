// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "CardDate/CardData.h"  // FCardData 구조체 포함
#include "Inventory/ItemData.h"
#include "GameFramework/PlayerController.h"
#include "ProjectKLBPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UTurnManager;
class AEnemy;
class UCardWidget;
class UDataGameInstance;
class UCardListWidget;
class ATile;
class AGridManager;
class AUIHUD;
class AEnemyAIController;
class UBlackboardComponent;
class UCombatUI;
class UInventory;
class AProjectKLBCharacter;
class UTutorialStageClearUI;


enum class ERelativeDirection { TwoOClock, TwelveOClock, TenOClock };

/**
 * AProjectKLBPlayerController
 * - 플레이어 입력 처리
 * - 캐릭터 이동 및 공격 처리
 * - UI와의 상호작용 관리
 */


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Dynamic Multicast Delegate 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FItemData>&, UpdatedInventory);

UCLASS()
class AProjectKLBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectKLBPlayerController();

    /** BeginPlay 호출 시 초기화 */
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    /** 입력 매핑 설정 */
    virtual void SetupInputComponent() override;

    /** 플레이어 턴 시작 */
    UFUNCTION(BlueprintCallable, Category = "Turn Management")
    void StartTurn();

    UFUNCTION(BlueprintCallable, Category = "Turn Management")
    void PlayerTurn();

    /** 카드를 UI에 표시 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void DisplayCardsOnUI();

    /** 카드 사용 */
    UFUNCTION(BlueprintCallable, Category = "Card")
    void UseCard(const FCardData& CardData);

    /** 타일 클릭 처리 */
    UFUNCTION()
    void HandleTileClick(ATile* ClickedTile);

    /** 캐릭터를 타일로 이동 */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveCharacterToTile(ATile* TargetTile);

    /** 적을 공격 */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackEnemy();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackSpecificEnemy(AEnemy* TargetEnemy);

    /** 마우스 클릭 처리 */
    void HandleClickAction();

    /** ESC 설정창 처리 */
    void TogglePauseMenu();

    //AI컨트롤러
    AEnemyAIController* AIController;


    void CallEndTurn();

    virtual void OnPossess(APawn* InPawn) override;


    // 현재 하이라이트된 타일 좌표를 저장
    TArray<FIntPoint> HighlightedTiles;

    // 카드 이동
    void MovePlayerUsingCard(const FCardData& CardData);

    void AddHexRangeTiles_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);
    void AddDiagonalTiles_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);
    void AddKnightMoves_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);
    void AddLostMoves_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);

    //////////////////////////////////////////////////////
    // Axial 전용 육각 타일 이동 로직
    //////////////////////////////////////////////////////

    void AddHexRangeTiles_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);
    void AddDiagonalTiles_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);
    void AddKnightMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);
    void AddLostMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);





    void MoveToFirstValidTile(const TArray<FIntPoint>& PossibleTiles, APawn* ControlledPawn);
    void EnableCardMovement(const TArray<FIntPoint>& PossibleTiles);
    void ResetCardMovement();

    // --- 그리드 관리 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    AGridManager* GridManager;

    // AI가 공격인식
    void CheckSurroundingTilesForEnemyAndAttack();

    // 플레이어가 현재 위치한 타일
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
    ATile* CurrentTile = nullptr;


    // ================================================ 인벤토리
    // 인벤토리 배열
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FItemData> Inventory;

    // 인벤토리 변경 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryChanged OnInventoryChanged;

    

    // 인벤토리 위젯을 표시하는 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void InventoryOn();

    // 인벤토리 위젯을 숨기는 함수 (토글 기능을 위해)
    UFUNCTION(BlueprintCallable, Category = "UI")
    void InventoryOff();


    // 인벤토리 교환 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SwapInventoryItems(int32 FromIndex, int32 ToIndex);


    // 플레이어 장비창
    UFUNCTION(BlueprintCallable, Category = "Turn Equipment")
    void UpdateEquipmentUI();

    // 드래그 앤 드랍시 이전 아이템 없애는 함수
    void RemoveInventoryItem(int32 Index);

    // 빈 슬롯 찾는 함수
    int32 FindEmptyInventorySlot() const;

    // ===== 나중에 이름  함수명 변경해주기
    // 인벤토리 초기화 함수
    void PCUpdateInventoryUI();

    // 장비창 초기화 함수
    void PCUpdateEquipmentUI();

    void PlayerHPUI();

    // 죽은 플레이어 Tile 처리 함수
    UFUNCTION(BlueprintCallable, Category = "Tile")
    void ClearCurrentTile(AProjectKLBCharacter* DeadPawn);


    void UpdateTutorialStageClearUI();


private:
    // --- 입력 관련 ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SetDestinationClickAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* TogglePauseAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    float ShortPressThreshold = 0.5f;

    // --- 비주얼 이펙트 ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UNiagaraSystem* FXCursor;


    // 이동 관련 변수
    TArray<ATile*> MovePath;        // 계산된 이동 경로
    int32 CurrentPathIndex = 0;     // 현재 경로상의 인덱스
    FTimerHandle MoveTimerHandle;   // 이동 타이머 핸들
    float StepDelay = 0.5f;         // 각 칸 사이 이동 지연 시간 (초)

    // 부드러운 이동 관련 변수
    float MoveProgress = 0.0f;           // 현재 보간 진행도 (0.0 ~ 1.0)
    float MoveUpdateInterval = 0.02f;    // 위치 갱신 간격 (초)
    FVector StartLocation;               // 현재 이동 시작 위치
    FVector EndLocation;                 // 현재 이동 목표 위치

    // 회전 관련 변수
    float InitialYaw = 0.0f;             // 이동 시작 시 초기 Yaw
    TArray<float> StepRotations;         // 각 이동 단계별 목표 Yaw
    int32 CurrentRotationStep = 0;       // 현재 회전 단계 인덱스
    FRotator TargetRotation;             // 목표 회전 각도
    FTimerHandle RotationTimerHandle;    // 회전 타이머 핸들
    float RotationSpeed = 180.0f;        // 회전 속도 (도/초)


    // 이동 및 회전 함수
    void MoveCharacterAlongPath(const TArray<ATile*>& Path, const FCardData& CardData);
    void MoveOneStep();
    void SmoothMoveUpdate();
    void StartRotation(float TargetYaw);
    void RotateUpdate();


    ATile* FindNeighborInRelativeDirection(ERelativeDirection RelDir);
    void MoveOneRelativeStep(ERelativeDirection RelDir);
    TArray<ERelativeDirection> DirectionSequence;
    int32 CurrentSequenceIndex = 0;
    // 이동시작
    void StartKnightMovement();
    void StartHexRangeMovement();
    void StartDiagonalMovement();
    void StartLostMovement();
    void MoveNextInSequence();

    // --- 카드 관리 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UCardListWidget> CardListWidgetClass;

    UPROPERTY()
    UCardListWidget* CardListWidget = nullptr;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UCombatUI> CombatUIClass;

    UPROPERTY()
    UCombatUI* CombatUI = nullptr;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UTutorialStageClearUI> TutorialStageClearUIClass;

    UPROPERTY()
    UTutorialStageClearUI* TutorialStageClearUI;


    // --- 공격 관련 ---
    UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float AttackDamage = 20.0f;

    FCardData CurrentCardData;

    bool bCardUsed = false; // 카드 사용 여부
    TArray<ATile*> ValidTiles; // 카드로 이동 가능한 타일 목록

    // --- 기타 ---
    FVector CachedDestination;
    bool bIsTouch = false;
    float FollowTime = 0.0f;
};


