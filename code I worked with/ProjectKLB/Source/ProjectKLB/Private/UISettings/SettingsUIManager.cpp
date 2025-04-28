// Fill out your copyright notice in the Description page of Project Settings.


#include "UISettings/SettingsUIManager.h"
#include "GameFramework/GameUserSettings.h" // 게임 설정 관리
#include "Types/SlateEnums.h" // ESelectInfo를 명시적으로 사용하려면 추가 헤더 포함
#include "Components/ComboBoxString.h" // 콤보 박스를 사용하기 위해 필요
#include "Components/Button.h" // 버튼을 사용하기 위해 필요
#include "Components/TextBlock.h" 
#include "Components/Widget.h" // 위젯 기본 기능
#include "Components/CheckBox.h"      
#include "Kismet/GameplayStatics.h"   
#include "Kismet/KismetSystemLibrary.h"



void USettingsUIManager::NativeConstruct()
{
    Super::NativeConstruct();

    // 체크박스 바인딩
    AutoSaveToggle = Cast<UCheckBox>(GetWidgetFromName(TEXT("AutoSaveToggle")));
    if (AutoSaveToggle)
    {
        AutoSaveToggle->OnCheckStateChanged.AddDynamic(this, &USettingsUIManager::OnAutoSaveToggled);
    }

    DisplayFullBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("DisplayFullBox")));
    if (DisplayFullBox)
    {
        // 체크박스 스타일 정의
        FCheckBoxStyle CheckBoxStyle;

        // 선택되지 않은 상태 
        FSlateBrush UncheckedBrush;
        UncheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Ellipse_3__1_.Ellipse_3__1_'"))); // 이미지 경로를 여기에 설정
        CheckBoxStyle.UncheckedImage = UncheckedBrush;

        // 선택된 상태 
        FSlateBrush CheckedBrush;
        CheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Group_1__2_.Group_1__2_'"))); // 이미지 경로를 여기에 설정
        CheckBoxStyle.CheckedImage = CheckedBrush;

        // 적용
        DisplayFullBox->WidgetStyle = CheckBoxStyle;

        // 상태 변경 이벤트 바인딩
        DisplayFullBox->OnCheckStateChanged.AddDynamic(this, &USettingsUIManager::DisplayFullBoxToggled);
    }

    DisplayWindowBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("DisplayWindowBox")));
    if (DisplayWindowBox)
    {
        // 체크박스 스타일 정의
        FCheckBoxStyle CheckBoxStyle;

        // 선택되지 않은 상태 
        FSlateBrush UncheckedBrush;
        UncheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Ellipse_3__1_.Ellipse_3__1_'"))); // 이미지 경로를 여기에 설정
        CheckBoxStyle.UncheckedImage = UncheckedBrush;

        // 선택된 상태 
        FSlateBrush CheckedBrush;
        CheckedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/test/Widget/SettingsUIWidget/Group_1__2_.Group_1__2_'"))); // 이미지 경로를 여기에 설정
        CheckBoxStyle.CheckedImage = CheckedBrush;

        // 적용
        DisplayWindowBox->WidgetStyle = CheckBoxStyle;

        // 상태 변경 이벤트 바인딩
        DisplayWindowBox->OnCheckStateChanged.AddDynamic(this, &USettingsUIManager::DisplayWindowBoxToggled);
    }

    // 버튼 바인딩
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

    // 콤보박스 바인딩
    if (ResolutionComboBox)
    {
        // 해상도 옵션 추가
        ResolutionComboBox->AddOption(TEXT("1920 x 1080"));
        ResolutionComboBox->AddOption(TEXT("1680 x 900"));
        ResolutionComboBox->AddOption(TEXT("1360 x 760"));
        ResolutionComboBox->AddOption(TEXT("1024 x 768"));

        // 선택 변경 이벤트 바인딩
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &USettingsUIManager::OnResolutionChanged);
    }

    // UserSettings 초기화
    UserSettings = GEngine->GetGameUserSettings();
    UpdateUI();
 
}

void USettingsUIManager::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (UserSettings)
    {
        // 선택된 해상도를 FIntPoint로 변환
        TArray<FString> ResolutionValues;
        SelectedItem.ParseIntoArray(ResolutionValues, TEXT(" x "));

        if (ResolutionValues.Num() == 2)
        {
            int32 Width = FCString::Atoi(*ResolutionValues[0]);
            int32 Height = FCString::Atoi(*ResolutionValues[1]);

            // 해상도 설정 변경
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
        // 현재 해상도 가져오기
        FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
        FString CurrentResolutionString = FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y);

        if (ResolutionComboBox)
        {
            // 현재 해상도 반영
            ResolutionComboBox->SetSelectedOption(CurrentResolutionString);
        }

        // 자동 저장 상태 반영
        if (AutoSaveToggle)
        {
            AutoSaveToggle->SetIsChecked(UserSettings->bUseVSync);
        }
    }
}

void USettingsUIManager::ChangeResolution(FIntPoint NewResolution)
{
    if (!UserSettings) return;

    // 새 해상도로 설정
    UserSettings->SetScreenResolution(NewResolution);
    UserSettings->ApplySettings(false);

    // UI 갱신
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
        // 설정을 즉시 적용
        UserSettings->ApplySettings(false);
    }
}

void USettingsUIManager::OnConfirmClicked()
{
    // 설정 적용 후 메뉴 닫기
    ApplySettings();
    RemoveFromParent(); // UI 닫기
}


void USettingsUIManager::OnAutoSaveToggled(bool IsChecked)
{
    if (UserSettings)
    {
        // 자동 저장 여부 설정 (예: VSync)
        UserSettings->bUseVSync = IsChecked;
    }
}

void USettingsUIManager::DisplayFullBoxToggled(bool bIsfullChecked)
{
    if (UserSettings)
    {
        if (bIsfullChecked)
        {
            // 전체화면으로 설정
            UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
            UserSettings->ApplySettings(false);

            // DisplayWindowBox의 체크 해제
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
            // 창 모드로 설정
            UserSettings->SetFullscreenMode(EWindowMode::Windowed);
            UserSettings->ApplySettings(false);

            // DisplayFullBox의 체크 해제
            if (DisplayFullBox)
            {
                DisplayFullBox->SetIsChecked(false);
            }

            UE_LOG(LogTemp, Warning, TEXT("Windowed mode enabled!"));
        }
    }
}
