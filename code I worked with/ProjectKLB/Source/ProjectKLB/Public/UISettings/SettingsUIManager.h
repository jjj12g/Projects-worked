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

    // �ػ� ����
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* AutoSaveToggle;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* DisplayFullBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* DisplayWindowBox;

    // ���߿� URadioButton���� ����? �����غ���
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

    // �ػ� ���� �Լ�
    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnResetClicked();

    UFUNCTION()
    void OnApplyClicked();

    UFUNCTION()
    void OnConfirmClicked();

    // ���� �ʱ�ȭ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateUI();

    // ������ ��� �����ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplySettings();

    // �ػ󵵸� �����ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ChangeResolution(FIntPoint NewResolution);

    // ���� ��ü
    UGameUserSettings* UserSettings;

};
