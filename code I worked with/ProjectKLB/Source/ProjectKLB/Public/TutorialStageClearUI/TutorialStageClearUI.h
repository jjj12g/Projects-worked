// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialStageClearUI.generated.h"

/**
 * 
 */

class UButton;


UCLASS()
class PROJECTKLB_API UTutorialStageClearUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* NextLevelButton;

	
	UFUNCTION()
	void OnNextLevelButtonClicked();



};
