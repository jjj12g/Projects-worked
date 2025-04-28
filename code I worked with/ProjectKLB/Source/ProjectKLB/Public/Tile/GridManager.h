// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardDate/CardData.h"  // FCardData ����ü ����
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"


class ATile;
class AEnemySpawner;


UENUM(BlueprintType)
enum class EGridShape : uint8
{
    Rectangular UMETA(DisplayName = "Rectangular"),
    Hexagonal   UMETA(DisplayName = "Hexagonal")
};


UCLASS()
class PROJECTKLB_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

    // �����Ϳ��� �׸��� ���¸� ������ �� �ֵ��� ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    EGridShape GridShape;

    // Ÿ���� �̵� �������� Ȯ��
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsTileWalkable(int32 Q, int32 R) const;

    // Ÿ���� �������� �Լ�
    ATile* GetTileAt(int32 Q, int32 R) const;

    // ������ Ÿ�� ���簢�� �� �׸��带 ����
    void GenerateHexGrid();

    // ��ü ������ �� �׸��带 ����
    void GenerateHexShapedGrid();

    // �׸��带 �����ϴ� �Լ� (GridShape�� ���� �б�)
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();

    // Ÿ���� ��ġ�� ����ϴ� �Լ�
    FVector GetTilePosition(int32 Q, int32 R) const;

    UPROPERTY(EditAnywhere, Category = "Grid")
    TSubclassOf<ATile> TileClass;

    // �׸����� ���� �� ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 NumCols = 12;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 NumRows = 10;

    UPROPERTY(EditAnywhere, Category = "Grid")
    float HexSize = 100.0f; // ������ ũ��

    // ���� "������" ũ�� 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 HexRadius;


    bool bShapedGrid = false;

    // Ÿ���� �����ϴ� ��
    UPROPERTY()
    TMap<FIntPoint, ATile*> TileMap;

    // GetTileCoordinatesFromPosition �Լ� �߰�
    FIntPoint GetTileCoordinatesFromPosition(const FVector& Position) const;

    UPROPERTY(EditAnywhere, Category = "Enemy")
    AEnemySpawner* EnemySpawner;

  
    bool IsTileOccupied(int32 Q, int32 R) const;


    ATile* GetTileAtLocation(const FVector& Location) const;


    // Ÿ�� ���̶���Ʈ ǥ��
    void HighlightValidTiles(const TArray<FIntPoint>& ValidTiles);

    void ClearHighlightedTiles();


    // AI ��ã��
    TArray<ATile*> FindPath(ATile* StartTile, ATile* TargetTile);
    TArray<ATile*> RetracePath(ATile* StartTile, ATile* EndTile);
    float GetDistance(ATile* TileA, ATile* TileB);
    TArray<ATile*> GetNeighboringTiles(ATile* CurrentTile);

    // Ÿ�� �迭
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    TArray<ATile*> AllTiles;

    // ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
    float Tolerance = 50.0f; // �⺻�� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
    virtual void Tick(float DeltaTime) override;

   

};
