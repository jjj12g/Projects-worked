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
    // (1) 헥스 반경 (3D와 동일하게 설정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid")
    int32 HexRadius = 5;

    // (3) 타일을 담을 CanvasPanel (디자이너에서 BindWidget)
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* TileCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* CharacterIconPanel;

    // 배치 완료 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* StartGameButton;

    // 실제 타일 위젯 보관
    UPROPERTY()
    TArray<UTileWidget*> TileWidgets;

    // 생성된 아이콘 위젯을 보관할 배열
    UPROPERTY()
    TArray<UCharacterIconWidget*> CharacterIcons;

    // 배치할 캐릭터 리스트 (ex: 늑대, 토끼 등) - GameMode나 GameInstance에서 받아온다고 가정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    TArray<TSubclassOf<AProjectKLBCharacter>> AvailableCharacters;

    // GridManager 참조
    UPROPERTY()
    class AGridManager* GridManagerRef;

    // 스폰 후 게임 시작을 GameMode에 알리기 위해
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

    // 타일 위젯 생성 함수
    void CreateTileWidgets();

    // 캐릭터 아이콘 생성 함수
    void CreateCharacterIcons();

    // 헥스 좌표 → 2D 픽셀 좌표 변환 함수
    FVector2D CalculateHexTileUIPosition(int32 Q, int32 R);

};
