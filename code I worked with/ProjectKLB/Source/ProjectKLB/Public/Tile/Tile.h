// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class UTextRenderComponent;
class UStaticMeshComponent;
class AEnemy;

UCLASS()
class PROJECTKLB_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 타일 메시
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	bool bIsWalkable;  // 타일이 이동 가능한지 여부

	// 타일의 위치 좌표 (Q, R)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Q;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 R;

	// 타일 초기화
	void InitializeTile(int32 InQ, int32 InR, bool bWalkable);

	// 타일 회전
	bool IsPointyTop = true;

	UFUNCTION()
	void OnTileClicked(AActor* TouchedActor, FKey ButtonPressed);

	// 타일이 이동 가능한지 여부를 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Tile")
	bool IsWalkable() const { return bIsWalkable; }

	// 텍스트 렌더 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	UTextRenderComponent* TextComponent;

	// 텍스트 업데이트 함수
	void UpdateTileText();

	// 큐브 좌표 출력
	FString PrintTileCoordinates() const;
	
	// 에너미 위치 확인
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	AEnemy* OccupyingEnemy = nullptr;  // 해당 타일에 있는 에너미
	
	void SetOccupyingEnemy(AEnemy* Enemy);


	// 이동 가능 머터리얼 강조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	UMaterialInterface* HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	UMaterialInterface* DefaultMaterial;

	void Highlight(bool bEnable);

	// AI A*알고리즘 추가요소
	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	float GCost; // 시작 타일에서 현재 타일까지의 거리

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	float HCost; // 현재 타일에서 목표 타일까지의 추정 거리

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	float FCost; // GCost + HCost

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	ATile* ParentTile; // 경로를 역추적하기 위한 부모 타일


	
	// 플레이어가 타일에 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bHasPlayer = false;

	// 플레이어가 타일에 있는지 여부 함수
	void SetHasPlayer(bool bValue);

	// 에너미가 타일에 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bHasEnemy = false;

	AEnemy* GetOccupyingEnemy();
	AEnemy* OccupyingActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	bool bIsHighlighted;

};
