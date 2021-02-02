#pragma once

#include<XEngine.h>
#include"Enemy.h"

class EnemyManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static EnemyManager& Get();
public :
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

private:
	std::vector<Enemy*> mEnemy;
	float mNextSpawnTime=1;

};

