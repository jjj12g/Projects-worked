// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlacementWidget.generated.h"

/**
 * 
 */

 class UTileWidget;
 class UCharacterIconWidget;
 class AProjectKLBCharacter;
 class UCanvasPanel;

UCLASS()
class PROJECTKLB_API UPlacementWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // (1) �� �ݰ� (3D�� �����ϰ� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid")
    int32 HexRadius = 5;

    // (3) Ÿ���� ���� CanvasPanel (�����̳ʿ��� BindWidget)
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* TileCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* CharacterIconPanel;

    // ��ġ �Ϸ� ��ư
    UPROPERTY(meta = (BindWidget))
    class UButton* StartGameButton;

    // ���� Ÿ�� ���� ����
    UPROPERTY()
    TArray<UTileWidget*> TileWidgets;

    // ������ ������ ������ ������ �迭
    UPROPERTY()
    TArray<UCharacterIconWidget*> CharacterIcons;

    // ��ġ�� ĳ���� ����Ʈ (ex: ����, �䳢 ��) - GameMode�� GameInstance���� �޾ƿ´ٰ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    TArray<TSubclassOf<AProjectKLBCharacter>> AvailableCharacters;

    // GridManager ����
    UPROPERTY()
    class AGridManager* GridManagerRef;

    // ���� �� ���� ������ GameMode�� �˸��� ����
    UPROPERTY()
    class AStageGameModeBase* StageGameModeRef;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UCharacterIconWidget> CharacterIconWidgetBPClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UTileWidget> TileWidgetBPClass;



protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnStartGameButtonClicked();

    // Ÿ�� ���� ���� �Լ�
    void CreateTileWidgets();

    // ĳ���� ������ ���� �Լ�
    void CreateCharacterIcons();

    // �� ��ǥ �� 2D �ȼ� ��ǥ ��ȯ �Լ�
    FVector2D CalculateHexTileUIPosition(int32 Q, int32 R);

};
