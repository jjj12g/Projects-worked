// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CharacterIcon/TileWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "ProjectKLBCharacter.h"
#include "Components/Image.h"



bool UTileWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!InOperation) return false;

    // Payload�� ĳ���� Ŭ������ ��Ҵٰ� ����
    if (TSubclassOf<AProjectKLBCharacter> DroppedClass = Cast<UClass>(InOperation->Payload))
    {
        UE_LOG(LogTemp, Warning, TEXT("DroppedClass"));
        if (TileData.bIsOccupied)
        {
            UE_LOG(LogTemp, Warning, TEXT("Tile already occupied! (Q=%d, R=%d)"), TileData.Q, TileData.R);
            // return false; // ��ü�� �������� �̷���
        }

        // ���� ���� ����
        TileData.bIsOccupied = true;
        TileData.OccupiedCharacterClass = DroppedClass;

        UE_LOG(LogTemp, Warning, TEXT("Dropped character on tile (Q=%d, R=%d)"), TileData.Q, TileData.R);

        // �ð��� ����
        UpdateTileVisual();

        return true;
    }

    return false;
}

void UTileWidget::UpdateTileVisual()
{
    // Ÿ�Ͽ� ��ġ�� ĳ���Ͱ� ������, �� ���������� OccupantImage�� ����
    if (!OccupantImage) return;

    if (TileData.bIsOccupied && TileData.OccupiedCharacterClass)
    {
        AProjectKLBCharacter* DefaultChar = TileData.OccupiedCharacterClass->GetDefaultObject<AProjectKLBCharacter>();
        if (DefaultChar && DefaultChar->CharacterIcon)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(DefaultChar->CharacterIcon);
            Brush.ImageSize = FVector2D(64.f, 64.f); // ������ ũ�� ����
            OccupantImage->SetBrush(Brush);
            UE_LOG(LogTemp, Log, TEXT("Dropped character on tile (Q=%d, R=%d) with class: %s"),
                TileData.Q, TileData.R,
                TileData.OccupiedCharacterClass ? *TileData.OccupiedCharacterClass->GetName() : TEXT("NULL"));
            return;
        }
    }

    // ���� ����ִٸ�(Ȥ�� �������� ���ٸ�) �̹��� ����
    OccupantImage->SetBrush(FSlateBrush());

}
