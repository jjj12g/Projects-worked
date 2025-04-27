// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterStatComponent.h"
#include "ProjectKLBCharacter.h"


// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // 기본 스탯 초기값 (데이터 테이블에서 불러온 후 덮어씌울 예정)
    BaseStats.Name = FName("");
    BaseStats.Health = 0;
    BaseStats.Attack = 0;
    BaseStats.Defense = 0;
    BaseStats.Speed = 0.f;
    BaseStats.SpecialAbilityDescription = "";
    BaseStats.Favorability = 0;

    FinalStats = BaseStats;

    // 장비 아이템은 기본적으로 빈 아이템으로 초기화합니다.
    Weapon = FItemData();
    Helmet = FItemData();
    Chest = FItemData();
    Legs = FItemData();
}




// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCharacterStatComponent::InitializeBaseStats(const FCharacterStat& InStats)
{
    BaseStats = InStats;
    // 기본 스탯으로 FinalStats를 우선 설정한 뒤, 장비 보너스 재계산
    FinalStats = BaseStats;
    RecalculateStats();

}

void UCharacterStatComponent::RecalculateStats()
{
    // 예를 들어, 최종 스탯은 기본 스탯에 각 장비 아이템의 보너스를 더해서 계산합니다.
    // 여기서는 FCharacterStat의 Attack, Defense, Health는 int32, Speed는 float라고 가정하고,
    // FItemData에서 보너스 값(Attack, Defense, Health, Speed)을 불러와 더합니다.
    FinalStats.Attack = BaseStats.Attack +
        static_cast<int32>(Weapon.Attack) +
        static_cast<int32>(Helmet.Attack) +
        static_cast<int32>(Chest.Attack) +
        static_cast<int32>(Legs.Attack);

    FinalStats.Defense = BaseStats.Defense +
        static_cast<int32>(Weapon.Defense) +
        static_cast<int32>(Helmet.Defense) +
        static_cast<int32>(Chest.Defense) +
        static_cast<int32>(Legs.Defense);

    FinalStats.Health = BaseStats.Health +
        static_cast<int32>(Weapon.Health) +
        static_cast<int32>(Helmet.Health) +
        static_cast<int32>(Chest.Health) +
        static_cast<int32>(Legs.Health);

    FinalStats.Speed = BaseStats.Speed +
        Weapon.Speed +
        Helmet.Speed +
        Chest.Speed +
        Legs.Speed;

    // 이름와 특수 능력 설명은 그대로 사용하거나 필요에 따라 수정합니다.
    FinalStats.Name = BaseStats.Name;
    FinalStats.SpecialAbilityDescription = BaseStats.SpecialAbilityDescription;

    UE_LOG(LogTemp, Log, TEXT("RecalculateStats: FinalAttack=%d, FinalDefense=%d, FinalHealth=%d, FinalSpeed=%f"),
        FinalStats.Attack, FinalStats.Defense, FinalStats.Health, FinalStats.Speed);

    // GetOwner()를 플레이어 캐릭터로 캐스팅한 후, 유효할 때만 업데이트 호출
    if (AProjectKLBCharacter* Player = Cast<AProjectKLBCharacter>(GetOwner()))
    {
        Player->UpdatePlayerStatsFromComponent();
    }
}

void UCharacterStatComponent::IncreaseFavorability(int32 Amount)
{
    // FinalStats.Favorability에 Amount를 더하고, 예를 들어 최대 100으로 클램프합니다.
    FinalStats.Favorability = FMath::Clamp(FinalStats.Favorability + Amount, 0, 100);
    UE_LOG(LogTemp, Log, TEXT("Favorability increased to: %d"), FinalStats.Favorability);

    // 스탯 재계산 (필요한 경우 활성화 하기)
    //RecalculateStats();
}
