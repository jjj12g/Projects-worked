// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tile/PlacementTileData.h"
#include "TileWidget.generated.h"

/**
 * 
 */

 class UImage;

UCLASS()
class PROJECTKLB_API UTileWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    // 이 타일의 배치 정보
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    FPlacementTileData TileData;

protected:
    // 타일 위젯 내부에 배치된 Image
    UPROPERTY(meta = (BindWidget))
    UImage* OccupantImage;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // 드롭 후나 상태 변화 시, 아이콘을 갱신하는 함수
    void UpdateTileVisual();

};
