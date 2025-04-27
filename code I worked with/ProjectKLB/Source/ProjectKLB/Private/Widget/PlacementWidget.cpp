// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlacementWidget.h"
#include "Widget/CharacterIcon/TileWidget.h"
#include "Widget/CharacterIcon/CharacterIconWidget.h"
#include "ProjectKLBCharacter.h"
#include "Stage/StageGameModeBase.h"
#include "Tile/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Tile/Tile.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

// ������ �巡�׾� ��� �ؾ���
// Ÿ������ ���� �ؾ���
// -- ��
// ���� �巡�� �� ����� ���� �� ĳ���Ͱ� �����Ǿ�� ��
// ���忡 ��ġ�� �׸��� �Ŵ������׼� � ���� ����� ������ Ÿ�ϰ� �����Ÿ� �޾ƿͼ� ������ �׷��־�� ��.
// ���߿� ������ ũ�� �������ֱ�


void UPlacementWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // GameMode, GridManager ���� ȹ��
    UWorld* World = GetWorld();
    if (World)
    {
        StageGameModeRef = Cast<AStageGameModeBase>(UGameplayStatics::GetGameMode(World));
        GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(World, AGridManager::StaticClass()));
    }

    // ��ư ���ε�
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UPlacementWidget::OnStartGameButtonClicked);
    }

    // Ÿ�� ���� ����
    CreateTileWidgets();

    // ĳ���� ������ ����
    CreateCharacterIcons();
}

void UPlacementWidget::CreateTileWidgets()
{
    if (!TileCanvasPanel || !TileWidgetBPClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateHexTileWidgets: CanvasPanel or TileWidgetBPClass is null"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Creating hex tile widgets..."));

    // (A) HexRadius�� �������� Axial ��ǥ (x,z) ��ȸ
    for (int32 x = -HexRadius; x <= HexRadius; x++)
    {
        for (int32 y = -HexRadius; y <= HexRadius; y++)
        {
            int32 z = -x - y;
            if (FMath::Abs(z) > HexRadius)
            {
                // ������ ���� ��
                continue;
            }

            // (B) Ÿ�� ���� ����
            UTileWidget* NewTile = CreateWidget<UTileWidget>(this, TileWidgetBPClass);
            if (!NewTile)
                continue;

            // (C) Ÿ���� Q,R(=x,z) ���� (UI���� OnDrop � ���)
            NewTile->TileData.Q = x;
            NewTile->TileData.R = z;
            NewTile->TileData.bIsOccupied = false;
            NewTile->TileData.OccupiedCharacterClass = nullptr;

            // (D) CanvasPanel�� AddChild �� UCanvasPanelSlot ��ȯ
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TileCanvasPanel->AddChild(NewTile));
            if (CanvasSlot)
            {
                // �� ��ǥ�� 2D �ȼ� ��ǥ�� ��ȯ
                FVector2D UIPos = CalculateHexTileUIPosition(x, z);
                CanvasSlot->SetPosition(UIPos);
                CanvasSlot->SetSize(FVector2D(60.f, 60.f)); // ���ϴ� Ÿ�� ũ��
            }

            TileWidgets.Add(NewTile);
            UE_LOG(LogTemp, Warning, TEXT("Created tile widget at Q=%d, R=%d"), x, z);

        }
    }
}


FVector2D UPlacementWidget::CalculateHexTileUIPosition(int32 Q, int32 R)
{
   // // Pointy-Top Axial �� Pixel ��ȯ ����
   //// (3D���� ����� ���İ� ��ġ�ϰ� or ���ϴ� ���¿� �°� ����)

   //// Ÿ�� �� ���� �ȼ� ũ��
   // float Size = 30.f;
   // // �����δ� 2D �̹���(������) ũ�⿡ ���� ����

   // // Pointy-Top Axial (q,r) ��ȯ
   // float width = FMath::Sqrt(3.f) * Size;
   // float height = 2.f * Size * 0.75f; // 1.5 * Size

   // float x = width * (Q + R * 0.5f);
   // float y = height * R;

   // // �ʿ��ϸ� �� ��ü�� ȭ�� �߾����� �ű�� ���� ������ �߰�
   // x += 300.f;
   // y += 300.f;

   // return FVector2D(x, y);

  // (1) �⺻ Axial -> (x, y)
    float size = 30.f;
    float x = FMath::Sqrt(3.f) * size * (Q + R * 0.5f);
    float y = 1.5f * size * R;

    // (2) 180�� ȸ�� ����: (x, y) -> (-x, -y)
    float rotatedX = -x;
    float rotatedY = -y;

    // (3) ���ϴ� ȭ�� �߽� ������
    rotatedX += 300.f;
    rotatedY += 300.f;

    return FVector2D(rotatedX, rotatedY);
}

void UPlacementWidget::CreateCharacterIcons()
{
    //UE_LOG(LogTemp, Warning, TEXT("NotCreateCharacterIcons"));
    if (!CharacterIconPanel)
        return;

    if (CharacterIconWidgetBPClass)
    {
        // for�� �ٱ��� �ƴ϶�, �ȿ��� ���� ����
        for (int32 i = 0; i < AvailableCharacters.Num(); i++)
        {
            UCharacterIconWidget* NewIcon = CreateWidget<UCharacterIconWidget>(this, CharacterIconWidgetBPClass);
            if (NewIcon)
            {
                // ĳ���� Ŭ������ �Ѱ��ش�
                NewIcon->CharacterClass = AvailableCharacters[i];

                // UniformGridPanel�� (��, ��)�� �߰�
                int32 Row = i / 3;
                int32 Col = i % 3;
                CharacterIconPanel->AddChildToUniformGrid(NewIcon, Row, Col);

                // �迭�� ����
                CharacterIcons.Add(NewIcon);

                //UE_LOG(LogTemp, Warning, TEXT("CreateCharacterIcons index: %d"), i);
            }
        }
    }
}

void UPlacementWidget::OnStartGameButtonClicked()
{
    if (!StageGameModeRef || !GridManagerRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnStartGameButtonClicked: StageGameModeRef or GridManagerRef is null"));
        return;
    }



    // Ÿ�� ���� �迭�� ��ȸ�ϸ鼭, ��ġ�� ĳ���Ͱ� �ִٸ� ���� Ÿ���� ã�� ���� ��û
    for (UTileWidget* TileWidget : TileWidgets)
    {
        if (!TileWidget)
            continue;
           
        FPlacementTileData& Data = TileWidget->TileData;
        UE_LOG(LogTemp, Warning, TEXT("TileWidget at Q=%d, R=%d, Occupied: %s"),
            Data.Q, Data.R, Data.bIsOccupied ? TEXT("true") : TEXT("false"));
        if (Data.bIsOccupied && Data.OccupiedCharacterClass)
        {
            // GridManager���� UI�� ����� Q, R ��ǥ�� ������ ���� Ÿ���� �����ɴϴ�.
            ATile* RealTile = GridManagerRef->GetTileAt(Data.Q, Data.R);
            if (RealTile)
            {
                // ���� ���� ���忡 ĳ���� ����
                StageGameModeRef->SpawnCharacterAtTile(Data.OccupiedCharacterClass, RealTile);
                UE_LOG(LogTemp, Warning, TEXT("Spawn request for tile (Q=%d, R=%d) with character %s"),
                    Data.Q, Data.R, *Data.OccupiedCharacterClass->GetName());
                UE_LOG(LogTemp, Warning, TEXT("Found real tile at Q=%d, R=%d at location %s"),
                    RealTile->Q, RealTile->R, *RealTile->GetActorLocation().ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No real tile found for Q=%d, R=%d"), Data.Q, Data.R);
            }
        }
    }

    // ��ġ �Ϸ� �� UI ���� �� ���� ���� ��ȣ ����
    RemoveFromParent();
    StageGameModeRef->OnPlacementFinished();
}
