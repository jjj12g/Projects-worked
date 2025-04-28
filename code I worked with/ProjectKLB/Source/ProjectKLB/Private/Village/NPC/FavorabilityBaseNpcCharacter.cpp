// Fill out your copyright notice in the Description page of Project Settings.


#include "Village/NPC/FavorabilityBaseNpcCharacter.h"
#include "Village/VillageCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Player/CharacterStatComponent.h"
#include "DataGameInstance.h"




// Sets default values
AFavorabilityBaseNpcCharacter::AFavorabilityBaseNpcCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // StatComponent 생성 및 초기화
    StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

    // 기본 StatRowName 설정
    StatRowName = FName("DefaultNPC");

}



// Called when the game starts or when spawned
void AFavorabilityBaseNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

    InitializeStats();


}

void AFavorabilityBaseNpcCharacter::InitializeStats()
{
    UDataGameInstance* GameInstance = Cast<UDataGameInstance>(GetGameInstance());
    if (GameInstance && GameInstance->CharacterStatsTable)
    {
        static const FString ContextString(TEXT("Character Stats Context"));
        FCharacterStat* FoundStats = GameInstance->CharacterStatsTable->FindRow<FCharacterStat>(StatRowName, ContextString);
        if (FoundStats)
        {
            // StatComponent를 통해 스탯을 초기화
            StatComponent->InitializeBaseStats(*FoundStats);
            UE_LOG(LogTemp, Log, TEXT("NPC Stats Initialized for %s"), *StatRowName.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Stats not found for StatRowName: %s"), *StatRowName.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterStatsTable is not loaded in GameInstance."));
    }
}


FText AFavorabilityBaseNpcCharacter::GetDialogueForPlayer(AVillageCharacter* PlayerCharacter) const
{
    // 기본 대사를 반환.
    return FText::FromString(FString::Printf(TEXT("%s: 기본 대사입니다."), *StatRowName.ToString()));
}

void AFavorabilityBaseNpcCharacter::Interact(AVillageCharacter* PlayerCharacter)
{
    // 플레이어와 상호작용할 때 대사 출력
    FText Dialogue = GetDialogueForPlayer(PlayerCharacter);
    UE_LOG(LogTemp, Log, TEXT("%s와의 대화: %s"), *StatRowName.ToString(), *Dialogue.ToString());

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s: %s"), *StatRowName.ToString(), *Dialogue.ToString()));
    }
}





