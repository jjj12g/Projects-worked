
#include "Schema/DataManager.h"
#include "Schema/EnemySchema.h"


TMap<int32, UEnemy*> UDataManager::_EnemyDict;

void UDataManager::LoadData()
{
	auto a = LoadJson<FEnemyData>("EnemyData");
        auto _EnemyData = a->MakeDict();
        for (auto& EnemyPair : _EnemyData)
	    {
		    if (UEnemy* Enemy = Cast<UEnemy>(EnemyPair.Value))
		    {
		    	_EnemyDict.Add(EnemyPair.Key, Enemy);
		    }
		    else
		    {
		    	UE_LOG(LogTemp, Warning, TEXT("Failed to cast to UEnemy for ID: %d"), EnemyPair.Key);
		    }
	    }
}
