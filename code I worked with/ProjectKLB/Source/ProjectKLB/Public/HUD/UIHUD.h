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
    // UI 생성 함수
    virtual void BeginPlay() override;

    // Pause Menu Widget 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

    // Settings UI Widget 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USettingsUIManager> SettingsWidgetClass;

    // Pause Menu Widget 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UPauseWidget* PauseWidget;

    // 위젯 인스턴스
    UPROPERTY()
    USettingsUIManager* SettingsWidget;


    // Pause Menu와 Settings UI 간 전환 함수
    void ShowPauseMenu();
    void ShowSettingsMenu();

    

};
