// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIHUD.generated.h"

/**
 * 
 */

class USettingsUIManager;
class UUserWidget;
class UPauseWidget;


UCLASS()
class PROJECTKLB_API AUIHUD : public AHUD
{
	GENERATED_BODY()
	

public:
    // UI ���� �Լ�
    virtual void BeginPlay() override;

    // Pause Menu Widget Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

    // Settings UI Widget Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USettingsUIManager> SettingsWidgetClass;

    // Pause Menu Widget Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UPauseWidget* PauseWidget;

    // ���� �ν��Ͻ�
    UPROPERTY()
    USettingsUIManager* SettingsWidget;


    // Pause Menu�� Settings UI �� ��ȯ �Լ�
    void ShowPauseMenu();
    void ShowSettingsMenu();

    

};
