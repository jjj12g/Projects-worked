// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectKLBGameMode.generated.h"

class AGridManager;
class UTurnManager;

UCLASS(minimalapi)
class AProjectKLBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectKLBGameMode();

protected:
  
    virtual void BeginPlay() override;

public:
    // Reference to the Grid Manager class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Manager")
    TSubclassOf<class AGridManager> GridManagerClass;

    AGridManager* GetGridManager() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
    TArray<TSubclassOf<class AProjectKLBCharacter>> PlayerCharacterClasses;

    UPROPERTY()
    UTurnManager* TurnManagerInstance;

private:
    // 생성된 GridManager 참조
    AGridManager* GridManager;


};



