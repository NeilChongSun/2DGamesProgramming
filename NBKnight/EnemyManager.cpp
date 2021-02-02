#include "EnemyManager.h"
#include"Level.h"
#include"Camera.h"
namespace
{
	EnemyManager* sInstance = nullptr;
	std::vector<X::Math::Vector2> pos;
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
	pos = Level::Get().GetEmenySpawnPoint();
	for (auto iter = pos.begin(); iter != pos.end();)
	{
		auto it = *iter;
		Enemy* enemy = new Enemy();
		enemy->Load();
		enemy->SetPosition(it);
		enemy->SetStartPosition(it);
		mEnemy.push_back(enemy);
		++iter;
	}
}

void EnemyManager::Unload()
{

}

void EnemyManager::Update(float deltaTime)
{

	for (auto iter = mEnemy.begin(); iter != mEnemy.end();)
	{
		Enemy* enemy = (*iter);
		if (!enemy->Die())
			enemy->Update(deltaTime);
		if (enemy->Die())
		{
			enemy->Unload();
			delete enemy;
			iter = mEnemy.erase(iter);
		}

		else
			++iter;
	}
}

void EnemyManager::Render()
{
	for (auto enemy : mEnemy)
	{
		enemy->Render();
	}
}
