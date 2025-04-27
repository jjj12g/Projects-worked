// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterIconWidget.generated.h"

/**
 * 
 */

 class UUniformGridPanel;
 class UImage;

UCLASS()
class PROJECTKLB_API UCharacterIconWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    virtual void NativeConstruct() override;

    // 어떤 캐릭터 클래스를 나타내는지 (늑대, 토끼 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    TSubclassOf<class AProjectKLBCharacter> CharacterClass;

    // UMG에서 아이콘 이미지를 표시할 UImage. 블루프린트에서 BindWidget으로 연결
    UPROPERTY(meta = (BindWidget))
    UImage* IconImage;

protected:
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
