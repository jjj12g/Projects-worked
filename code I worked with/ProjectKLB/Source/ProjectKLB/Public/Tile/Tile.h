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

	// Ÿ�� �޽�
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	bool bIsWalkable;  // Ÿ���� �̵� �������� ����

	// Ÿ���� ��ġ ��ǥ (Q, R)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Q;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 R;

	// Ÿ�� �ʱ�ȭ
	void InitializeTile(int32 InQ, int32 InR, bool bWalkable);

	// Ÿ�� ȸ��
	bool IsPointyTop = true;

	UFUNCTION()
	void OnTileClicked(AActor* TouchedActor, FKey ButtonPressed);

	// Ÿ���� �̵� �������� ���θ� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Tile")
	bool IsWalkable() const { return bIsWalkable; }

	// �ؽ�Ʈ ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	UTextRenderComponent* TextComponent;

	// �ؽ�Ʈ ������Ʈ �Լ�
	void UpdateTileText();

	// ť�� ��ǥ ���
	FString PrintTileCoordinates() const;
	
	// ���ʹ� ��ġ Ȯ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	AEnemy* OccupyingEnemy = nullptr;  // �ش� Ÿ�Ͽ� �ִ� ���ʹ�
	
	void SetOccupyingEnemy(AEnemy* Enemy);


	// �̵� ���� ���͸��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	UMaterialInterface* HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	UMaterialInterface* DefaultMaterial;

	void Highlight(bool bEnable);

	// AI A*�˰��� �߰����
	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	float GCost; // ���� Ÿ�Ͽ��� ���� Ÿ�ϱ����� �Ÿ�

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	float HCost; // ���� Ÿ�Ͽ��� ��ǥ Ÿ�ϱ����� ���� �Ÿ�

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	float FCost; // GCost + HCost

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	ATile* ParentTile; // ��θ� �������ϱ� ���� �θ� Ÿ��


	
	// �÷��̾ Ÿ�Ͽ� �ִ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bHasPlayer = false;

	// �÷��̾ Ÿ�Ͽ� �ִ��� ���� �Լ�
	void SetHasPlayer(bool bValue);

	// ���ʹ̰� Ÿ�Ͽ� �ִ��� ����
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
