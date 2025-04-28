// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CombatUI.h"
#include "Components/Button.h"
#include "ProjectKLBPlayerController.h"
#include "Inventory/Inventory.h"
#include "ProjectKLBCharacter.h"


void UCombatUI::NativeConstruct()
{
    Super::NativeConstruct();

    // PlayerController 참조 가져오기
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer)
    {
        PlayerControllerRef = Cast<AProjectKLBPlayerController>(OwningPlayer);
        if (PlayerControllerRef)
        {
            UpdateInventoryUI();

            // 인벤토리 변경 델리게이트에 핸들러 바인딩
            PlayerControllerRef->OnInventoryChanged.AddDynamic(this, &UCombatUI::HandleInventoryChanged);

            UE_LOG(LogTemp, Log, TEXT("CombatUI constructed and InventoryChanged delegate bound."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to cast OwningPlayer to ProjectKLBPlayerController."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OwningPlayer is null."));
    }

    // 버튼 이벤트 바인딩
    if (InventoryButton)
    {
        InventoryButton->OnClicked.AddDynamic(this, &UCombatUI::OnInventoryButtonClicked);
        UE_LOG(LogTemp, Log, TEXT("InventoryButton bound to OnInventoryButtonClicked."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryButton is null."));
    }

}

UCombatUI::UCombatUI(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    UE_LOG(LogTemp, Log, TEXT("CombatUI instance created."));

}



void UCombatUI::UpdateInventoryUI()
{
    UE_LOG(LogTemp, Log, TEXT("UpdateInventoryUI called."));
    if (InventoryWidget && PlayerControllerRef)
    {
        InventoryWidget->InitializeInventory(PlayerControllerRef->Inventory);
        UE_LOG(LogTemp, Log, TEXT("InventoryWidget initialized with %d items."), PlayerControllerRef->Inventory.Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryWidget or PlayerControllerRef is null."));
    }
}

void UCombatUI::OnInventoryButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("InventoryButton clicked."));
    // PlayerController에서 인벤토리 토글 기능 호출
    if (PlayerControllerRef)
    {
        if (InventoryWidget->IsVisible())
        {
            PlayerControllerRef->InventoryOff();
            UE_LOG(LogTemp, Log, TEXT("InventoryOff called."));
        }
        else
        {
            PlayerControllerRef->InventoryOn();
            UE_LOG(LogTemp, Log, TEXT("InventoryOn called."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerControllerRef is null."));
    }
}

void UCombatUI::HandleInventoryChanged(const TArray<FItemData>& UpdatedInventory)
{
    UE_LOG(LogTemp, Log, TEXT("HandleInventoryChanged called with %d items."), UpdatedInventory.Num());
    if (InventoryWidget)
    {
        InventoryWidget->InitializeInventory(UpdatedInventory);
        UE_LOG(LogTemp, Log, TEXT("InventoryWidget updated with %d items."), UpdatedInventory.Num());
        UE_LOG(LogTemp, Warning, TEXT("InventoryWidgetChanged"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryWidget is null."));
    }
}

// 이후 추가내용
void UCombatUI::UpdateCharacterList(const TArray<AProjectKLBCharacter*>& Characters)
{
    //if (!CharacterListVerticalBox) return;

    //CharacterListVerticalBox->ClearChildren();

    //for (AProjectKLBCharacter* Character : Characters)
    //{
    //    if (!Character) continue;

    //    UWBP_CharacterInfo* CharInfoWidget = CreateWidget<UWBP_CharacterInfo>(this, CharacterInfoWidgetClass);
    //    if (CharInfoWidget)
    //    {
    //        // CharInfoWidget에 캐릭터 정보 설정
    //        CharInfoWidget->SetCharacterImage(/* 캐릭터 이미지 설정 */);
    //        CharInfoWidget->SetHealth(Character->CurrentHP / Character->MaxHP);

    //        CharacterListVerticalBox->AddChildToVerticalBox(CharInfoWidget);
    //    }
    //}

}


UCombatUI::~UCombatUI()
{
    UE_LOG(LogTemp, Log, TEXT("CombatUI instance destroyed."));
}


