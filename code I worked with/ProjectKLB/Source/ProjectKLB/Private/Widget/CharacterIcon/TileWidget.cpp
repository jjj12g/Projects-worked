// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CharacterIcon/TileWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "ProjectKLBCharacter.h"
#include "Components/Image.h"



bool UTileWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!InOperation) return false;

    // Payload에 캐릭터 클래스를 담았다고 가정
    if (TSubclassOf<AProjectKLBCharacter> DroppedClass = Cast<UClass>(InOperation->Payload))
    {
        UE_LOG(LogTemp, Warning, TEXT("DroppedClass"));
        if (TileData.bIsOccupied)
        {
            UE_LOG(LogTemp, Warning, TEXT("Tile already occupied! (Q=%d, R=%d)"), TileData.Q, TileData.R);
            // return false; // 교체를 막으려면 이렇게
        }

        // 점유 정보 갱신
        TileData.bIsOccupied = true;
        TileData.OccupiedCharacterClass = DroppedClass;

        UE_LOG(LogTemp, Warning, TEXT("Dropped character on tile (Q=%d, R=%d)"), TileData.Q, TileData.R);

        // 시각적 갱신
        UpdateTileVisual();

        return true;
    }

    return false;
}

void UTileWidget::UpdateTileVisual()
{
    // 타일에 배치된 캐릭터가 있으면, 그 아이콘으로 OccupantImage를 변경
    if (!OccupantImage) return;

    if (TileData.bIsOccupied && TileData.OccupiedCharacterClass)
    {
        AProjectKLBCharacter* DefaultChar = TileData.OccupiedCharacterClass->GetDefaultObject<AProjectKLBCharacter>();
        if (DefaultChar && DefaultChar->CharacterIcon)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(DefaultChar->CharacterIcon);
            Brush.ImageSize = FVector2D(64.f, 64.f); // 아이콘 크기 조절
            OccupantImage->SetBrush(Brush);
            UE_LOG(LogTemp, Log, TEXT("Dropped character on tile (Q=%d, R=%d) with class: %s"),
                TileData.Q, TileData.R,
                TileData.OccupiedCharacterClass ? *TileData.OccupiedCharacterClass->GetName() : TEXT("NULL"));
            return;
        }
    }

    // 만약 비어있다면(혹은 아이콘이 없다면) 이미지 제거
    OccupantImage->SetBrush(FSlateBrush());

}
