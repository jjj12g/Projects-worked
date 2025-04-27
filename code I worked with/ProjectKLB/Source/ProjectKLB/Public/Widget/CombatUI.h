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


	// �κ��丮 ����
	UPROPERTY(meta = (BindWidget))
	class UInventory* InventoryWidget;

	// �κ��丮 ����
	UPROPERTY(meta = (BindWidget))
	class UEquipment* EquipmentWidget;




protected:
	// PlayerController ����
	class AProjectKLBPlayerController* PlayerControllerRef;

	// �κ��丮 UI ������Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateInventoryUI();

	// "����" ��ư�� ���� UPROPERTY
	UPROPERTY(meta = (BindWidget))
	UButton* InventoryButton;




	// �κ��丮 ��ư Ŭ��
	UFUNCTION()
	void OnInventoryButtonClicked();


	// �κ��丮 ���� ��������Ʈ �ڵ鷯
	UFUNCTION()
	void HandleInventoryChanged(const TArray<FItemData>& UpdatedInventory);



	//CharacterListVerticalBox

};
