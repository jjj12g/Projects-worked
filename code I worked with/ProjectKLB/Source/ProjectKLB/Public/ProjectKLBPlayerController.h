// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "CardDate/CardData.h"  // FCardData ����ü ����
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
 * - �÷��̾� �Է� ó��
 * - ĳ���� �̵� �� ���� ó��
 * - UI���� ��ȣ�ۿ� ����
 */


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Dynamic Multicast Delegate ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FItemData>&, UpdatedInventory);

UCLASS()
class AProjectKLBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectKLBPlayerController();

    /** BeginPlay ȣ�� �� �ʱ�ȭ */
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    /** �Է� ���� ���� */
    virtual void SetupInputComponent() override;

    /** �÷��̾� �� ���� */
    UFUNCTION(BlueprintCallable, Category = "Turn Management")
    void StartTurn();

    UFUNCTION(BlueprintCallable, Category = "Turn Management")
    void PlayerTurn();

    /** ī�带 UI�� ǥ�� */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void DisplayCardsOnUI();

    /** ī�� ��� */
    UFUNCTION(BlueprintCallable, Category = "Card")
    void UseCard(const FCardData& CardData);

    /** Ÿ�� Ŭ�� ó�� */
    UFUNCTION()
    void HandleTileClick(ATile* ClickedTile);

    /** ĳ���͸� Ÿ�Ϸ� �̵� */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveCharacterToTile(ATile* TargetTile);

    /** ���� ���� */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackEnemy();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackSpecificEnemy(AEnemy* TargetEnemy);

    /** ���콺 Ŭ�� ó�� */
    void HandleClickAction();

    /** ESC ����â ó�� */
    void TogglePauseMenu();

    //AI��Ʈ�ѷ�
    AEnemyAIController* AIController;


    void CallEndTurn();

    virtual void OnPossess(APawn* InPawn) override;


    // ���� ���̶���Ʈ�� Ÿ�� ��ǥ�� ����
    TArray<FIntPoint> HighlightedTiles;

    // ī�� �̵�
    void MovePlayerUsingCard(const FCardData& CardData);

    void AddHexRangeTiles_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);
    void AddDiagonalTiles_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);
    void AddKnightMoves_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);
    void AddLostMoves_Offset(const FIntPoint& CurrentTile, TArray<FIntPoint>& PossibleTiles);

    //////////////////////////////////////////////////////
    // Axial ���� ���� Ÿ�� �̵� ����
    //////////////////////////////////////////////////////

    void AddHexRangeTiles_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);
    void AddDiagonalTiles_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);
    void AddKnightMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);
    void AddLostMoves_Axial(const FIntPoint& CurrentTileCoord, TArray<FIntPoint>& PossibleTiles);





    void MoveToFirstValidTile(const TArray<FIntPoint>& PossibleTiles, APawn* ControlledPawn);
    void EnableCardMovement(const TArray<FIntPoint>& PossibleTiles);
    void ResetCardMovement();

    // --- �׸��� ���� ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    AGridManager* GridManager;

    // AI�� �����ν�
    void CheckSurroundingTilesForEnemyAndAttack();

    // �÷��̾ ���� ��ġ�� Ÿ��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
    ATile* CurrentTile = nullptr;


    // ================================================ �κ��丮
    // �κ��丮 �迭
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FItemData> Inventory;

    // �κ��丮 ���� ��������Ʈ
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryChanged OnInventoryChanged;

    

    // �κ��丮 ������ ǥ���ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "UI")
    void InventoryOn();

    // �κ��丮 ������ ����� �Լ� (��� ����� ����)
    UFUNCTION(BlueprintCallable, Category = "UI")
    void InventoryOff();


    // �κ��丮 ��ȯ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SwapInventoryItems(int32 FromIndex, int32 ToIndex);


    // �÷��̾� ���â
    UFUNCTION(BlueprintCallable, Category = "Turn Equipment")
    void UpdateEquipmentUI();

    // �巡�� �� ����� ���� ������ ���ִ� �Լ�
    void RemoveInventoryItem(int32 Index);

    // �� ���� ã�� �Լ�
    int32 FindEmptyInventorySlot() const;

    // ===== ���߿� �̸�  �Լ��� �������ֱ�
    // �κ��丮 �ʱ�ȭ �Լ�
    void PCUpdateInventoryUI();

    // ���â �ʱ�ȭ �Լ�
    void PCUpdateEquipmentUI();

    void PlayerHPUI();

    // ���� �÷��̾� Tile ó�� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Tile")
    void ClearCurrentTile(AProjectKLBCharacter* DeadPawn);


    void UpdateTutorialStageClearUI();


private:
    // --- �Է� ���� ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SetDestinationClickAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* TogglePauseAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    float ShortPressThreshold = 0.5f;

    // --- ���־� ����Ʈ ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UNiagaraSystem* FXCursor;


    // �̵� ���� ����
    TArray<ATile*> MovePath;        // ���� �̵� ���
    int32 CurrentPathIndex = 0;     // ���� ��λ��� �ε���
    FTimerHandle MoveTimerHandle;   // �̵� Ÿ�̸� �ڵ�
    float StepDelay = 0.5f;         // �� ĭ ���� �̵� ���� �ð� (��)

    // �ε巯�� �̵� ���� ����
    float MoveProgress = 0.0f;           // ���� ���� ���൵ (0.0 ~ 1.0)
    float MoveUpdateInterval = 0.02f;    // ��ġ ���� ���� (��)
    FVector StartLocation;               // ���� �̵� ���� ��ġ
    FVector EndLocation;                 // ���� �̵� ��ǥ ��ġ

    // ȸ�� ���� ����
    float InitialYaw = 0.0f;             // �̵� ���� �� �ʱ� Yaw
    TArray<float> StepRotations;         // �� �̵� �ܰ躰 ��ǥ Yaw
    int32 CurrentRotationStep = 0;       // ���� ȸ�� �ܰ� �ε���
    FRotator TargetRotation;             // ��ǥ ȸ�� ����
    FTimerHandle RotationTimerHandle;    // ȸ�� Ÿ�̸� �ڵ�
    float RotationSpeed = 180.0f;        // ȸ�� �ӵ� (��/��)


    // �̵� �� ȸ�� �Լ�
    void MoveCharacterAlongPath(const TArray<ATile*>& Path, const FCardData& CardData);
    void MoveOneStep();
    void SmoothMoveUpdate();
    void StartRotation(float TargetYaw);
    void RotateUpdate();


    ATile* FindNeighborInRelativeDirection(ERelativeDirection RelDir);
    void MoveOneRelativeStep(ERelativeDirection RelDir);
    TArray<ERelativeDirection> DirectionSequence;
    int32 CurrentSequenceIndex = 0;
    // �̵�����
    void StartKnightMovement();
    void StartHexRangeMovement();
    void StartDiagonalMovement();
    void StartLostMovement();
    void MoveNextInSequence();

    // --- ī�� ���� ---
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


    // --- ���� ���� ---
    UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float AttackDamage = 20.0f;

    FCardData CurrentCardData;

    bool bCardUsed = false; // ī�� ��� ����
    TArray<ATile*> ValidTiles; // ī��� �̵� ������ Ÿ�� ���

    // --- ��Ÿ ---
    FVector CachedDestination;
    bool bIsTouch = false;
    float FollowTime = 0.0f;
};


