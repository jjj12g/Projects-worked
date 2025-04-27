// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsUIManager.generated.h"

/**
 * 
 */
class UComboBoxString;
class UGameUserSettings;
class UButton;
class UCheckBox;
class UTextBlock;






UCLASS()
class PROJECTKLB_API USettingsUIManager : public UUserWidget
{
	GENERATED_BODY()
	

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* ResetButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ConfirmButton;

    // 해상도 설정
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* AutoSaveToggle;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* DisplayFullBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* DisplayWindowBox;

    // 나중에 URadioButton으로 변경? 생각해보기
    UPROPERTY(meta = (BindWidget))
    UButton* FullscreenRadioButton;

    UPROPERTY(meta = (BindWidget))
    UButton* WindowedRadioButton;


private:
    UFUNCTION()
    void OnAutoSaveToggled(bool IsChecked);

    UFUNCTION()
    void DisplayFullBoxToggled(bool bIsfullChecked);

    UFUNCTION()
    void DisplayWindowBoxToggled(bool bIsWindowChecked);

    // 해상도 변경 함수
    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnResetClicked();

    UFUNCTION()
    void OnApplyClicked();

    UFUNCTION()
    void OnConfirmClicked();

    // 설정 초기화 함수
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateUI();

    // 설정을 즉시 적용하는 함수
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplySettings();

    // 해상도를 변경하는 함수
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ChangeResolution(FIntPoint NewResolution);

    // 설정 객체
    UGameUserSettings* UserSettings;

};
