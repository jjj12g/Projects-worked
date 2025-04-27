// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StageGameModeBase.generated.h"

/**
 * 
 */

class AGridManager;
class UTurnManager;
class UPlacementWidget;

UCLASS()
class PROJECTKLB_API AStageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	


public:
    AStageGameModeBase();

protected:

    virtual void BeginPlay() override;

public:
    // Reference to the Grid Manager class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Manager")
    TSubclassOf<class AGridManager> GridManagerClass;

    AGridManager* GetGridManager() const;

    //// �÷��̾� ĳ���͸� ������ �� ����� Ŭ����
    UPROPERTY(BlueprintReadOnly)
    TArray<AProjectKLBCharacter*> SpawnedCharacters;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
    TArray<TSubclassOf<class AProjectKLBCharacter>> PlayerCharacterClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<UPlacementWidget> PlacementWidgetClass;

    UPROPERTY()
    UPlacementWidget* PlacementUI;

    UPROPERTY()
    UTurnManager* TurnManagerInstance;

    UFUNCTION(BlueprintCallable)
    void SpawnCharacterAtTile(TSubclassOf<AProjectKLBCharacter> CharacterClass, class ATile* Tile);

    // ��ġ �Ϸ� �� ó��
    UFUNCTION(BlueprintCallable)
    void OnPlacementFinished();

private:
    // ������ GridManager ����
    AGridManager* GridManager;

};
