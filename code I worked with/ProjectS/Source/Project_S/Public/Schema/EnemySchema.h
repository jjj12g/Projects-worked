
#pragma once
#include "CoreMinimal.h"
#include "DataManager.h"

#include "EnemySchema.generated.h"

UCLASS(BlueprintType)
class PROJECT_S_API UEnemy : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY()
    FString hp;
    UPROPERTY()
    int32 mp;
    UPROPERTY()
    TArray<float> ddaass;
    UPROPERTY()
    int32 id;

};

USTRUCT(BlueprintType)
struct FEnemyData
{
    GENERATED_BODY()
    UPROPERTY()
    TArray<UEnemy*> _Enemys;
    TMap<int32, UObject*> MakeDict()
    {
        TMap<int32, UObject*> dict;
        for (auto _Enemy : _Enemys)
            dict.Add(_Enemy->id, _Enemy);
        return dict;
    }
};
