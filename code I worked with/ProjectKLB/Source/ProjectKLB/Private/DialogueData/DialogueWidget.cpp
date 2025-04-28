// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueData/DialogueWidget.h"
#include "Village/NPC/FavorabilityBaseNpcCharacter.h"
#include "Player/CharacterStatComponent.h"
#include "Components/Button.h"
#include "Village/VillageCharacter.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"


void UDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 호감도 버튼 델리게이트
    if (FavorabilityButton)
    {
        FavorabilityButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnFavorabilityButtonClicked);
    }

    if (CheckButtonBorder)
    {
        // 현재 브러시 컬러를 가져온 후, 알파값을 0 (안 보임)으로 설정
        FLinearColor NewColor = CheckButtonBorder->BrushColor;
        NewColor.A = 0.0f;
        CheckButtonBorder->SetBrushColor(NewColor);
    }

    if (CheckButton)
    {
        CheckButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnCheckButtonClicked);
        // 버튼 Hover 이벤트 바인딩
        CheckButton->OnHovered.AddDynamic(this, &UDialogueWidget::OnCheckButtonHovered);
        CheckButton->OnUnhovered.AddDynamic(this, &UDialogueWidget::OnCheckButtonUnhovered);

       
    }
}


void UDialogueWidget::OnFavorabilityButtonClicked()
{
    APlayer = Cast<AVillageCharacter>(GetOwningPlayer());
    if (NPCRef && NPCRef->StatComponent)
    {
        // 호감도 10 증가
        NPCRef->StatComponent->IncreaseFavorability(10);
        if (!APlayer)
        {
            NPCRef->Interact(APlayer);
        }

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFavorabilityButtonClicked: NPCRef or StatComponent is null."));
    }

    if (NPCRef)
    {
        //NPC의 대사 결정 함수를 호출합니다.
        FText NewDialogue = NPCRef->GetDialogueForPlayer(nullptr);
        UpdateDialogueText(NewDialogue);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFavorabilityButtonClicked: NPCRef is null."));
    }
}

void UDialogueWidget::UpdateDialogueText(const FText& NewDialogue)
{
    if (DialogueTextUpdated)
    {
        DialogueTextUpdated->SetText(NewDialogue);
    }
    if (DialogueSwitcher)
    {
        // 슬롯 1 (업데이트된 대사) 활성화
        DialogueSwitcher->SetActiveWidgetIndex(1);
    }
    UE_LOG(LogTemp, Log, TEXT("Dialogue updated: %s"), *NewDialogue.ToString());
}

void UDialogueWidget::OnCheckButtonClicked()
{
    if (DialogueSwitcher)
    {
        // 슬롯 0 (업데이트된 대사) 활성화
        DialogueSwitcher->SetActiveWidgetIndex(0);
    }
}

void UDialogueWidget::OnCheckButtonHovered()
{
    UE_LOG(LogTemp, Log, TEXT("CheckButton Hovered!"));
    if (CheckButtonBorder)
    {
        // 현재 브러시 컬러를 가져온 후, 알파값 1로 설정.
        FLinearColor NewColor = CheckButtonBorder->BrushColor;
        NewColor.A = 1.0f;
        CheckButtonBorder->SetBrushColor(NewColor);
    }
}

void UDialogueWidget::OnCheckButtonUnhovered()
{
    UE_LOG(LogTemp, Log, TEXT("CheckButton Unhovered!"));
    if (CheckButtonBorder)
    {
        // 현재 브러시 컬러를 가져온 후, 알파값 0으로 설정.
        FLinearColor NewColor = CheckButtonBorder->BrushColor;
        NewColor.A = 0.0f;
        CheckButtonBorder->SetBrushColor(NewColor);
    }
}
