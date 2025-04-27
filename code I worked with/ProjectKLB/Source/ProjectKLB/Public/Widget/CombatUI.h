// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemData.h"
#include "CombatUI.generated.h"

/**
 * 
 */

 class UButton;
 class AProjectKLBCharacter;

UCLASS()
class PROJECTKLB_API UCombatUI : public UUserWidget
{
	GENERATED_BODY()
	



public:
	virtual void NativeConstruct() override;

	UCombatUI(const FObjectInitializer& ObjectInitializer);

	virtual ~UCombatUI();



	void UpdateCharacterList(const TArray<AProjectKLBCharacter*>& Characters);


	// 인벤토리 위젯
	UPROPERTY(meta = (BindWidget))
	class UInventory* InventoryWidget;

	// 인벤토리 위젯
	UPROPERTY(meta = (BindWidget))
	class UEquipment* EquipmentWidget;




protected:
	// PlayerController 참조
	class AProjectKLBPlayerController* PlayerControllerRef;

	// 인벤토리 UI 업데이트 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateInventoryUI();

	// "설정" 버튼에 대한 UPROPERTY
	UPROPERTY(meta = (BindWidget))
	UButton* InventoryButton;




	// 인벤토리 버튼 클릭
	UFUNCTION()
	void OnInventoryButtonClicked();


	// 인벤토리 변경 델리게이트 핸들러
	UFUNCTION()
	void HandleInventoryChanged(const TArray<FItemData>& UpdatedInventory);



	//CharacterListVerticalBox

};
