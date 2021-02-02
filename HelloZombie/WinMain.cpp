#include <XEngine.h>
#include"Survivor.h"
#include"EnemyMananger.h"

Survivor survivor;
X::TextureId barEmpty;
X::TextureId barRed;

void GameInit()
{
	EnemyManager::StaticInitialize();
	EnemyManager::Get().Load();
	survivor.Load();
	X::ShowSystemCursor(false);
	X::SetBackgroundColor(X::Colors::LightGray);
	survivor.SetPosition((200.0f,200.0f));
	barEmpty = X::LoadTexture("bar_empty.png");
	barRed = X::LoadTexture("bar_red.png");
}

void GameCleanUp()
{
	EnemyManager::Get().Unload();
	EnemyManager::StaticTerminate();
	survivor.Unload();
	
}

bool GameLoop(float deltaTime)
{
	survivor.Update(deltaTime);
	EnemyManager::Get().Update(deltaTime);

	EnemyManager::Get().Render();
	survivor.Render();

	static float percent = 0.0f;
	percent += deltaTime;
	if (percent>1.0f)
	{
		percent -= 1.0f;

	}

	X::Math::Rect rect{ 0.0f,0.0f,percent*X::GetSpriteWidth(barRed),1.0f*X::GetSpriteHeight(barRed)};

	X::DrawSprite(barEmpty, { 100.0f,100.0f }, X::Pivot::TopLeft);
	//X::DrawSprite(barRed,rect, { 100.0f,100.0f }, X::Pivot::TopLeft);
	
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