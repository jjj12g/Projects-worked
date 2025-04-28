// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardDate/CardData.h"  // FCardData 구조체 포함
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

    // 에디터에서 그리드 형태를 선택할 수 있도록 함
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    EGridShape GridShape;

    // 타일이 이동 가능한지 확인
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsTileWalkable(int32 Q, int32 R) const;

    // 타일을 가져오는 함수
    ATile* GetTileAt(int32 Q, int32 R) const;

    // 육각형 타일 직사각형 맵 그리드를 생성
    void GenerateHexGrid();

    // 전체 육각형 맵 그리드를 생성
    void GenerateHexShapedGrid();

    // 그리드를 생성하는 함수 (GridShape에 따라 분기)
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();

    // 타일의 위치를 계산하는 함수
    FVector GetTilePosition(int32 Q, int32 R) const;

    UPROPERTY(EditAnywhere, Category = "Grid")
    TSubclassOf<ATile> TileClass;

    // 그리드의 열과 행 수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 NumCols = 12;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 NumRows = 10;

    UPROPERTY(EditAnywhere, Category = "Grid")
    float HexSize = 100.0f; // 육각형 크기

    // 맵의 "반지름" 크기 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 HexRadius;


    bool bShapedGrid = false;

    // 타일을 저장하는 맵
    UPROPERTY()
    TMap<FIntPoint, ATile*> TileMap;

    // GetTileCoordinatesFromPosition 함수 추가
    FIntPoint GetTileCoordinatesFromPosition(const FVector& Position) const;

    UPROPERTY(EditAnywhere, Category = "Enemy")
    AEnemySpawner* EnemySpawner;

  
    bool IsTileOccupied(int32 Q, int32 R) const;


    ATile* GetTileAtLocation(const FVector& Location) const;


    // 타일 하이라이트 표시
    void HighlightValidTiles(const TArray<FIntPoint>& ValidTiles);

    void ClearHighlightedTiles();


    // AI 길찾기
    TArray<ATile*> FindPath(ATile* StartTile, ATile* TargetTile);
    TArray<ATile*> RetracePath(ATile* StartTile, ATile* EndTile);
    float GetDistance(ATile* TileA, ATile* TileB);
    TArray<ATile*> GetNeighboringTiles(ATile* CurrentTile);

    // 타일 배열
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    TArray<ATile*> AllTiles;

    // 허용 오차
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
    float Tolerance = 50.0f; // 기본값 설정

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
    virtual void Tick(float DeltaTime) override;

   

};
