// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DialogueData/WolfDialogueData.h" // ȣ���� ����ü�� �����ϴ� ���
#include "CharacterStat/CharacterStat.h"
#include "FavorabilityBaseNpcCharacter.generated.h"

UCLASS()
class PROJECTKLB_API AFavorabilityBaseNpcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFavorabilityBaseNpcCharacter();


    // NPC ���� �ʱ�ȭ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void InitializeStats();

    // ĳ���� ���� (������ ���̺��� ������ ��)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    struct FCharacterStat CharacterStats;

    // NPC�� ����� StatComponent (���� �����͸� ����)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    class UCharacterStatComponent* StatComponent;

    // NPC �̸�(������ ���̺� ���� Ű�� ���)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FName StatRowName;

    // �÷��̾��� ȣ������ ���� ��縦 ��ȯ�ϴ� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    virtual FText GetDialogueForPlayer(class AVillageCharacter* PlayerCharacter) const;


    // �÷��̾�� ��ȣ�ۿ��� �� ȣ��Ǵ� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void Interact(class AVillageCharacter* PlayerCharacter);

protected:
    virtual void BeginPlay() override;

    // NPC �̸� �� �⺻ ��� ���� (���� �ִϸ��̼�, AI ��� ���� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FText NPCName;

    // ��ȭ ������ ���̺� (ȣ������ ���� ��� �����͸� ������ �� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    class UDataTable* DialogueDataTable;

};
