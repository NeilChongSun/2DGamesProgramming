#include "EnemyMananger.h"


namespace
{
	EnemyManager* sInstance = nullptr;
}

void EnemyManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "EnemyManager already initialized!");
	sInstance = new EnemyManager();
}

void EnemyManager::StaticTerminate()
{

	delete sInstance;
	sInstance = nullptr;
}

EnemyManager & EnemyManager::Get()
{
	XASSERT(sInstance != nullptr, "No EnemyManager created!");
	return *sInstance;
}

void EnemyManager::Load()
{
}

void EnemyManager::Unload()
{
}

void EnemyManager::Update(float deltaTime)
{
	//Check if it is time to spwan
	if (mNextSpawnTime < X::GetTime())
	{
		//Spawn
		Enemy* enemy = new Enemy();
		enemy->Load();
		enemy->SetPosition((X::RandomFloat(0,X::GetScreenWidth()), X::RandomFloat(0,X::GetScreenHeight())));
		mEnemy.push_back(enemy);
		//Pick the next spawn in the future
		mNextSpawnTime = X::GetTime() + X::RandomFloat(1.0f, 2.5f);
	}

	for (auto iter = mEnemy.begin(); iter != mEnemy.end();)
	{
		Enemy* enemy = (*iter);
		if (!enemy->Die())
		{
			enemy->Updata(deltaTime);
		}

		if (enemy->CanDelete())
		{
			enemy->Unload();
			delete enemy;
			iter = mEnemy.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void EnemyManager::Render()
{
	for (auto enemy : mEnemy)
		enemy->Render();	
}
