// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterStatComponent.h"
#include "ProjectKLBCharacter.h"


// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // �⺻ ���� �ʱⰪ (������ ���̺��� �ҷ��� �� ����� ����)
    BaseStats.Name = FName("");
    BaseStats.Health = 0;
    BaseStats.Attack = 0;
    BaseStats.Defense = 0;
    BaseStats.Speed = 0.f;
    BaseStats.SpecialAbilityDescription = "";
    BaseStats.Favorability = 0;

    FinalStats = BaseStats;

    // ��� �������� �⺻������ �� ���������� �ʱ�ȭ�մϴ�.
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
    // �⺻ �������� FinalStats�� �켱 ������ ��, ��� ���ʽ� ����
    FinalStats = BaseStats;
    RecalculateStats();

}

void UCharacterStatComponent::RecalculateStats()
{
    // ���� ���, ���� ������ �⺻ ���ȿ� �� ��� �������� ���ʽ��� ���ؼ� ����մϴ�.
    // ���⼭�� FCharacterStat�� Attack, Defense, Health�� int32, Speed�� float��� �����ϰ�,
    // FItemData���� ���ʽ� ��(Attack, Defense, Health, Speed)�� �ҷ��� ���մϴ�.
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

    // �̸��� Ư�� �ɷ� ������ �״�� ����ϰų� �ʿ信 ���� �����մϴ�.
    FinalStats.Name = BaseStats.Name;
    FinalStats.SpecialAbilityDescription = BaseStats.SpecialAbilityDescription;

    UE_LOG(LogTemp, Log, TEXT("RecalculateStats: FinalAttack=%d, FinalDefense=%d, FinalHealth=%d, FinalSpeed=%f"),
        FinalStats.Attack, FinalStats.Defense, FinalStats.Health, FinalStats.Speed);

    // GetOwner()�� �÷��̾� ĳ���ͷ� ĳ������ ��, ��ȿ�� ���� ������Ʈ ȣ��
    if (AProjectKLBCharacter* Player = Cast<AProjectKLBCharacter>(GetOwner()))
    {
        Player->UpdatePlayerStatsFromComponent();
    }
}

void UCharacterStatComponent::IncreaseFavorability(int32 Amount)
{
    // FinalStats.Favorability�� Amount�� ���ϰ�, ���� ��� �ִ� 100���� Ŭ�����մϴ�.
    FinalStats.Favorability = FMath::Clamp(FinalStats.Favorability + Amount, 0, 100);
    UE_LOG(LogTemp, Log, TEXT("Favorability increased to: %d"), FinalStats.Favorability);

    // ���� ���� (�ʿ��� ��� Ȱ��ȭ �ϱ�)
    //RecalculateStats();
}
