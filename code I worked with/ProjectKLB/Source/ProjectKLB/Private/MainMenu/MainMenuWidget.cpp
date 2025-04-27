// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UISettings/SettingsUIManager.h"
#include "SaveGame/SaveWidget.h"



void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ư�� ���������� ���ε��Ǿ����� üũ �� Ŭ�� �̺�Ʈ�� ��������Ʈ �߰�
    if (GameStartButton)
    {
        GameStartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnGameStartButtonClicked);
    }

    // ExitButton�� ���������� ���ε��Ǿ����� üũ �� Ŭ�� �̺�Ʈ�� ��������Ʈ �߰�
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsButtonClicked);
    }

}

void UMainMenuWidget::OnGameStartButtonClicked()
{
    if (SaveWidgetClass)
    {
        USaveWidget* SaveWidget = CreateWidget<USaveWidget>(GetWorld(), SaveWidgetClass);
        if (SaveWidget)
        {
            SaveWidget->AddToViewport();
            // �ʿ��, ���� �޴� ������ ȭ�鿡�� ������ �� �ֽ��ϴ�.
            RemoveFromParent();
        }
    }
}

void UMainMenuWidget::OnExitButtonClicked()
{
    // GetOwningPlayer()�� ���� ���� �÷��̾� ��Ʈ�ѷ��� ������ ��, QuitGame �Լ� ȣ��
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
}

void UMainMenuWidget::OnSettingsButtonClicked()
{
    if (SettingsWidgetClass)
    {
        // SettingsWidgetClass�� �����Ϳ��� WBP_SettingsWidget ������ �����մϴ�.
        USettingsUIManager* SettingsWidget = CreateWidget<USettingsUIManager>(GetWorld(), SettingsWidgetClass);
        if (SettingsWidget)
        {
            // ���� ������ ȭ�鿡 ǥ��
            SettingsWidget->AddToViewport();

            // ���� ������ �����Ͽ� ������ �ʰ� ��
            SetVisibility(ESlateVisibility::Hidden);
        }
    }
    

}


