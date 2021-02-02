#include<XEngine.h>
#include"TileMap.h"

TileMap myMap;


void GameInit()
{
	myMap.Load();
}

void GameCleanUp()
{
	myMap.Unload();
}

bool GameLoop(float deltaTime)
{
	myMap.Update(deltaTime);
	myMap.Render();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();
	X::Run(GameLoop);
	GameCleanUp();
	X::Stop();
	return 0;
}