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
    // �� Ÿ���� ��ġ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    FPlacementTileData TileData;

protected:
    // Ÿ�� ���� ���ο� ��ġ�� Image
    UPROPERTY(meta = (BindWidget))
    UImage* OccupantImage;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    // ��� �ĳ� ���� ��ȭ ��, �������� �����ϴ� �Լ�
    void UpdateTileVisual();

};
