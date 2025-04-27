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

// 아이콘 드래그앤 드랍 해야함
// 타일위젯 생성 해야함
// -- 완
// 이제 드래그 앤 드랍한 곳에 그 캐릭터가 생성되어야 함
// 월드에 배치된 그리드 매니저한테서 어떤 맵이 만들어 질지랑 타일값 같은거를 받아와서 위젯을 그려주어야 함.
// 나중에 아이콘 크기 수정해주기


void UPlacementWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // GameMode, GridManager 참조 획득
    UWorld* World = GetWorld();
    if (World)
    {
        StageGameModeRef = Cast<AStageGameModeBase>(UGameplayStatics::GetGameMode(World));
        GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(World, AGridManager::StaticClass()));
    }

    // 버튼 바인딩
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UPlacementWidget::OnStartGameButtonClicked);
    }

    // 타일 위젯 생성
    CreateTileWidgets();

    // 캐릭터 아이콘 생성
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

    // (A) HexRadius를 기준으로 Axial 좌표 (x,z) 순회
    for (int32 x = -HexRadius; x <= HexRadius; x++)
    {
        for (int32 y = -HexRadius; y <= HexRadius; y++)
        {
            int32 z = -x - y;
            if (FMath::Abs(z) > HexRadius)
            {
                // 육각형 범위 밖
                continue;
            }

            // (B) 타일 위젯 생성
            UTileWidget* NewTile = CreateWidget<UTileWidget>(this, TileWidgetBPClass);
            if (!NewTile)
                continue;

            // (C) 타일의 Q,R(=x,z) 저장 (UI에서 OnDrop 등에 사용)
            NewTile->TileData.Q = x;
            NewTile->TileData.R = z;
            NewTile->TileData.bIsOccupied = false;
            NewTile->TileData.OccupiedCharacterClass = nullptr;

            // (D) CanvasPanel에 AddChild → UCanvasPanelSlot 반환
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TileCanvasPanel->AddChild(NewTile));
            if (CanvasSlot)
            {
                // 헥스 좌표를 2D 픽셀 좌표로 변환
                FVector2D UIPos = CalculateHexTileUIPosition(x, z);
                CanvasSlot->SetPosition(UIPos);
                CanvasSlot->SetSize(FVector2D(60.f, 60.f)); // 원하는 타일 크기
            }

            TileWidgets.Add(NewTile);
            UE_LOG(LogTemp, Warning, TEXT("Created tile widget at Q=%d, R=%d"), x, z);

        }
    }
}


FVector2D UPlacementWidget::CalculateHexTileUIPosition(int32 Q, int32 R)
{
   // // Pointy-Top Axial → Pixel 변환 예시
   //// (3D에서 사용한 공식과 일치하게 or 원하는 형태에 맞게 조정)

   //// 타일 한 변의 픽셀 크기
   // float Size = 30.f;
   // // 실제로는 2D 이미지(육각형) 크기에 맞춰 조정

   // // Pointy-Top Axial (q,r) 변환
   // float width = FMath::Sqrt(3.f) * Size;
   // float height = 2.f * Size * 0.75f; // 1.5 * Size

   // float x = width * (Q + R * 0.5f);
   // float y = height * R;

   // // 필요하면 맵 전체를 화면 중앙으로 옮기기 위한 오프셋 추가
   // x += 300.f;
   // y += 300.f;

   // return FVector2D(x, y);

  // (1) 기본 Axial -> (x, y)
    float size = 30.f;
    float x = FMath::Sqrt(3.f) * size * (Q + R * 0.5f);
    float y = 1.5f * size * R;

    // (2) 180도 회전 적용: (x, y) -> (-x, -y)
    float rotatedX = -x;
    float rotatedY = -y;

    // (3) 원하는 화면 중심 오프셋
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
        // for문 바깥이 아니라, 안에서 위젯 생성
        for (int32 i = 0; i < AvailableCharacters.Num(); i++)
        {
            UCharacterIconWidget* NewIcon = CreateWidget<UCharacterIconWidget>(this, CharacterIconWidgetBPClass);
            if (NewIcon)
            {
                // 캐릭터 클래스를 넘겨준다
                NewIcon->CharacterClass = AvailableCharacters[i];

                // UniformGridPanel에 (행, 열)로 추가
                int32 Row = i / 3;
                int32 Col = i % 3;
                CharacterIconPanel->AddChildToUniformGrid(NewIcon, Row, Col);

                // 배열에 저장
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



    // 타일 위젯 배열을 순회하면서, 배치된 캐릭터가 있다면 실제 타일을 찾아 스폰 요청
    for (UTileWidget* TileWidget : TileWidgets)
    {
        if (!TileWidget)
            continue;
           
        FPlacementTileData& Data = TileWidget->TileData;
        UE_LOG(LogTemp, Warning, TEXT("TileWidget at Q=%d, R=%d, Occupied: %s"),
            Data.Q, Data.R, Data.bIsOccupied ? TEXT("true") : TEXT("false"));
        if (Data.bIsOccupied && Data.OccupiedCharacterClass)
        {
            // GridManager에서 UI에 저장된 Q, R 좌표와 동일한 실제 타일을 가져옵니다.
            ATile* RealTile = GridManagerRef->GetTileAt(Data.Q, Data.R);
            if (RealTile)
            {
                // 실제 게임 월드에 캐릭터 스폰
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

    // 배치 완료 후 UI 제거 및 게임 시작 신호 전달
    RemoveFromParent();
    StageGameModeRef->OnPlacementFinished();
}
