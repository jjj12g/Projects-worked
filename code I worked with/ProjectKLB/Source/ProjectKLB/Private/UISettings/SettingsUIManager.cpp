// Fill out your copyright notice in the Description page of Project Settings.


#include "UISettings/SettingsUIManager.h"
#include "GameFramework/GameUserSettings.h" // ���� ���� ����
#include "Types/SlateEnums.h" // ESelectInfo�� ��������� ����Ϸ��� �߰� ��� ����
#include "Components/ComboBoxString.h" // �޺� �ڽ��� ����ϱ� ���� �ʿ�
#include "Components/Button.h" // ��ư�� ����ϱ� ���� �ʿ�
#include "Components/TextBlock.h" 
#include "Components/Widget.h" // ���� �⺻ ���
#include "Components/CheckBox.h"      
#include "Kismet/GameplayStatics.h"   
#include "Kismet/KismetSystemLibrary.h"



void USettingsUIManager::NativeConstruct()
{
    Super::NativeConstruct();

    // üũ�ڽ� ���ε�
    AutoSaveToggle = Cast<UCheckBox>(GetWidgetFromName(TEXT("AutoSaveToggle")));
    if (AutoSaveToggle)
    {
        AutoSaveToggle->OnCheckStateChanged.AddDynamic(this, &USettingsUIManager::OnAutoSaveToggled);
    }

    DisplayFullBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("DisplayFullBox")));
    if (DisplayFullBox)
    {
        // üũ�ڽ� ��Ÿ�� ����
        FCheckBoxStyle CheckBoxStyle;

        // ���õ��� ���� ���� 
        FSlateBrush UncheckedBrush;
        UncheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Ellipse_3__1_.Ellipse_3__1_'"))); // �̹��� ��θ� ���⿡ ����
        CheckBoxStyle.UncheckedImage = UncheckedBrush;

        // ���õ� ���� 
        FSlateBrush CheckedBrush;
        CheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Group_1__2_.Group_1__2_'"))); // �̹��� ��θ� ���⿡ ����
        CheckBoxStyle.CheckedImage = CheckedBrush;

        // ����
        DisplayFullBox->WidgetStyle = CheckBoxStyle;

        // ���� ���� �̺�Ʈ ���ε�
        DisplayFullBox->OnCheckStateChanged.AddDynamic(this, &USettingsUIManager::DisplayFullBoxToggled);
    }

    DisplayWindowBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("DisplayWindowBox")));
    if (DisplayWindowBox)
    {
        // üũ�ڽ� ��Ÿ�� ����
        FCheckBoxStyle CheckBoxStyle;

        // ���õ��� ���� ���� 
        FSlateBrush UncheckedBrush;
        UncheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Ellipse_3__1_.Ellipse_3__1_'"))); // �̹��� ��θ� ���⿡ ����
        CheckBoxStyle.UncheckedImage = UncheckedBrush;

        // ���õ� ���� 
        FSlateBrush CheckedBrush;
        CheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Group_1__2_.Group_1__2_'"))); // �̹��� ��θ� ���⿡ ����
        CheckBoxStyle.CheckedImage = CheckedBrush;

        // ����
        DisplayWindowBox->WidgetStyle = CheckBoxStyle;

        // ���� ���� �̺�Ʈ ���ε�
        DisplayWindowBox->OnCheckStateChanged.AddDynamic(this, &USettingsUIManager::DisplayWindowBoxToggled);
    }

    // ��ư ���ε�
    if (ResetButton)
    {
        ResetButton->OnClicked.AddDynamic(this, &USettingsUIManager::OnResetClicked);
    }
    if (ApplyButton)
    {
        ApplyButton->OnClicked.AddDynamic(this, &USettingsUIManager::OnApplyClicked);
    }
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &USettingsUIManager::OnConfirmClicked);
    }

    // �޺��ڽ� ���ε�
    if (ResolutionComboBox)
    {
        // �ػ� �ɼ� �߰�
        ResolutionComboBox->AddOption(TEXT("1920 x 1080"));
        ResolutionComboBox->AddOption(TEXT("1680 x 900"));
        ResolutionComboBox->AddOption(TEXT("1360 x 760"));
        ResolutionComboBox->AddOption(TEXT("1024 x 768"));

        // ���� ���� �̺�Ʈ ���ε�
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &USettingsUIManager::OnResolutionChanged);
    }

    // UserSettings �ʱ�ȭ
    UserSettings = GEngine->GetGameUserSettings();
    UpdateUI();
 
}

void USettingsUIManager::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (UserSettings)
    {
        // ���õ� �ػ󵵸� FIntPoint�� ��ȯ
        TArray<FString> ResolutionValues;
        SelectedItem.ParseIntoArray(ResolutionValues, TEXT(" x "));

        if (ResolutionValues.Num() == 2)
        {
            int32 Width = FCString::Atoi(*ResolutionValues[0]);
            int32 Height = FCString::Atoi(*ResolutionValues[1]);

            // �ػ� ���� ����
            UserSettings->SetScreenResolution(FIntPoint(Width, Height));
            UserSettings->ApplySettings(false);

            UE_LOG(LogTemp, Log, TEXT("Resolution changed to: %d x %d"), Width, Height);
        }
    }
}

void USettingsUIManager::UpdateUI()
{
    if (UserSettings)
    {
        // ���� �ػ� ��������
        FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
        FString CurrentResolutionString = FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y);

        if (ResolutionComboBox)
        {
            // ���� �ػ� �ݿ�
            ResolutionComboBox->SetSelectedOption(CurrentResolutionString);
        }

        // �ڵ� ���� ���� �ݿ�
        if (AutoSaveToggle)
        {
            AutoSaveToggle->SetIsChecked(UserSettings->bUseVSync);
        }
    }
}

void USettingsUIManager::ChangeResolution(FIntPoint NewResolution)
{
    if (!UserSettings) return;

    // �� �ػ󵵷� ����
    UserSettings->SetScreenResolution(NewResolution);
    UserSettings->ApplySettings(false);

    // UI ����
    UpdateUI();
}




void USettingsUIManager::OnResetClicked()
{
    if (UserSettings)
    {
        UserSettings->ResetToCurrentSettings();
        UserSettings->ApplySettings(false);

    }
}

void USettingsUIManager::OnApplyClicked()
{
    ApplySettings();
}

void USettingsUIManager::ApplySettings()
{
    if (UserSettings)
    {
        // ������ ��� ����
        UserSettings->ApplySettings(false);
    }
}

void USettingsUIManager::OnConfirmClicked()
{
    // ���� ���� �� �޴� �ݱ�
    ApplySettings();
    RemoveFromParent(); // UI �ݱ�
}


void USettingsUIManager::OnAutoSaveToggled(bool IsChecked)
{
    if (UserSettings)
    {
        // �ڵ� ���� ���� ���� (��: VSync)
        UserSettings->bUseVSync = IsChecked;
    }
}

void USettingsUIManager::DisplayFullBoxToggled(bool bIsfullChecked)
{
    if (UserSettings)
    {
        if (bIsfullChecked)
        {
            // ��üȭ������ ����
            UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
            UserSettings->ApplySettings(false);

            // DisplayWindowBox�� üũ ����
            if (DisplayWindowBox)
            {
                DisplayWindowBox->SetIsChecked(false);
            }

            UE_LOG(LogTemp, Warning, TEXT("Fullscreen mode enabled!"));
        }
    }
}


void USettingsUIManager::DisplayWindowBoxToggled(bool bIsWindowChecked)
{
    if (UserSettings)
    {
        if (bIsWindowChecked)
        {
            // â ���� ����
            UserSettings->SetFullscreenMode(EWindowMode::Windowed);
            UserSettings->ApplySettings(false);

            // DisplayFullBox�� üũ ����
            if (DisplayFullBox)
            {
                DisplayFullBox->SetIsChecked(false);
            }

            UE_LOG(LogTemp, Warning, TEXT("Windowed mode enabled!"));
        }
    }
}
