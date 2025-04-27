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

    // ȣ���� ��ư ��������Ʈ
    if (FavorabilityButton)
    {
        FavorabilityButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnFavorabilityButtonClicked);
    }

    if (CheckButtonBorder)
    {
        // ���� �귯�� �÷��� ������ ��, ���İ��� 0 (�� ����)���� �����մϴ�.
        FLinearColor NewColor = CheckButtonBorder->BrushColor;
        NewColor.A = 0.0f;
        CheckButtonBorder->SetBrushColor(NewColor);
    }

    if (CheckButton)
    {
        CheckButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnCheckButtonClicked);
        // ��ư Hover �̺�Ʈ ���ε�
        CheckButton->OnHovered.AddDynamic(this, &UDialogueWidget::OnCheckButtonHovered);
        CheckButton->OnUnhovered.AddDynamic(this, &UDialogueWidget::OnCheckButtonUnhovered);

       
    }
}


void UDialogueWidget::OnFavorabilityButtonClicked()
{
    APlayer = Cast<AVillageCharacter>(GetOwningPlayer());
    if (NPCRef && NPCRef->StatComponent)
    {
        // ��: ȣ������ 10 ����
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
        // ����: NPC�� ��� ���� �Լ��� ȣ���մϴ�.
        // (�����δ� �÷��̾� ĳ���͸� ���ڷ� ������ �� ������, ���⼭�� �ܼ� ���÷� nullptr�� ����)
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
        // ���� 1 (������Ʈ�� ���)�� Ȱ��ȭ�մϴ�.
        DialogueSwitcher->SetActiveWidgetIndex(1);
    }
    UE_LOG(LogTemp, Log, TEXT("Dialogue updated: %s"), *NewDialogue.ToString());
}

void UDialogueWidget::OnCheckButtonClicked()
{
    if (DialogueSwitcher)
    {
        // ���� 0 (������Ʈ�� ���)�� Ȱ��ȭ�մϴ�.
        DialogueSwitcher->SetActiveWidgetIndex(0);
    }
}

void UDialogueWidget::OnCheckButtonHovered()
{
    UE_LOG(LogTemp, Log, TEXT("CheckButton Hovered!"));
    if (CheckButtonBorder)
    {
        // ���� �귯�� �÷��� ������ ��, ���İ��� 1 (����)�� �����մϴ�.
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
        // ���� �귯�� �÷��� ������ ��, ���İ��� 0 (�� ����)���� �����մϴ�.
        FLinearColor NewColor = CheckButtonBorder->BrushColor;
        NewColor.A = 0.0f;
        CheckButtonBorder->SetBrushColor(NewColor);
    }
}
