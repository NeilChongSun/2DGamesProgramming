#include<XEngine.h>
#include"Level.h"
#include"Player.h"
#include"Camera.h"
#include"EnemyManager.h"

enum State
{
	mainScene,
	gameLoop,
	win,
	lost,
	quitGame
};
State state = mainScene;
void GameInit()
{
	Level::StaticInitialize();
	Level::Get().LoadLevel("Level.txt");
	Level::Get().LoadTexture("Tile.txt");
	Camera::StaticInitialize();
	EnemyManager::StaticInitialize();
	EnemyManager::Get().Load();
	Player::StaticInitialize();
	Player::Get().Load();
}
void GameCleanUp()
{
	Level::StaticTerminate();
	Camera::StaticTerminate();
	EnemyManager::StaticTerminate();
	Player::StaticTerminate();

}

bool MainScene(float deltaTime)
{
	X::DrawScreenText("NB Knight", X::GetScreenWidth() / 2-80,X::GetScreenHeight()/2-100, 36, X::Colors::Blue);
	X::DrawScreenText("Press S to Start", X::GetScreenWidth() / 2-150, X::GetScreenHeight() / 2+72-100, 36, X::Colors::Blue);
	X::DrawScreenText("Press ESC to Quit", X::GetScreenWidth() / 2-150, X::GetScreenHeight() / 2+108-100, 36, X::Colors::Blue);

	if (X::IsKeyPressed(X::Keys::S))
	{
		state = gameLoop;
		return true;
	}

	if (X::IsKeyPressed(X::Keys::ESCAPE))
	{
		state = quitGame;
		return true;
	}
}

bool Win(float deltaTime)
{
	X::DrawScreenText("You Win!", X::GetScreenWidth() / 2 - 80, X::GetScreenHeight() / 2 - 100, 36, X::Colors::Green);
	X::DrawScreenText("Press R to Restart", X::GetScreenWidth() / 2 - 150, X::GetScreenHeight() / 2 + 72 - 100, 36, X::Colors::Green);
	X::DrawScreenText("Press ESC to Quit", X::GetScreenWidth() / 2 - 150, X::GetScreenHeight() / 2 + 108 - 100, 36, X::Colors::Green);

	if (X::IsKeyPressed(X::Keys::R))
	{
		state = gameLoop;
		return true;
	}

	if (X::IsKeyPressed(X::Keys::ESCAPE))
	{
		state = quitGame;
		return true;
	}

}

bool Lost(float deltaTime)
{
	X::DrawScreenText("You Lost!", X::GetScreenWidth() / 2 - 80, X::GetScreenHeight() / 2 - 100, 36, X::Colors::Red);
	X::DrawScreenText("Press R to Restart", X::GetScreenWidth() / 2 - 150, X::GetScreenHeight() / 2 + 72 - 100, 36, X::Colors::Red);
	X::DrawScreenText("Press ESC to Quit", X::GetScreenWidth() / 2 - 150, X::GetScreenHeight() / 2 + 108 - 100, 36, X::Colors::Red);

	if (X::IsKeyPressed(X::Keys::R))
	{
		state = gameLoop;
		return true;
	}

	if (X::IsKeyPressed(X::Keys::ESCAPE))
	{
		state = quitGame;
		return true;
	}

}

bool GameLoop(float deltaTime)
{
	Level::Get().Update(deltaTime);
	Level::Get().Render();
	
	Camera::Get().SetViewBound(Level::Get().GetBound());
	Camera::Get().SetViewPosition(Player::Get().GetPosition());
	
	EnemyManager::Get().Update(deltaTime);
	EnemyManager::Get().Render();
	
	Player::Get().Update(deltaTime);
	Player::Get().Render();
	
	if (X::IsKeyPressed(X::Keys::ESCAPE))
	{
		state = mainScene;
		return true;
	}


	if (Player::Get().IsLost())
	{
		state = lost;
		return true;
	}

	if (EnemyManager::Get().IsWin())
	{
		state = win;
		return true;
	}
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	bool isQuitGame = false;
	while (!isQuitGame)
	{
		switch (state)
		{
		case mainScene:
			GameCleanUp();
			X::Run(MainScene);
			break;
		case gameLoop:
			GameInit();
			X::Run(GameLoop);
			GameCleanUp();
			break;
		case win:
			GameCleanUp();
			X::Run(Win);
			break;
		case lost:
			GameCleanUp();
			X::Run(Lost);
			break;
		case quitGame:
			isQuitGame = true;
			break;
		default:
			break;
		}
	}
	X::Stop();
	return 0;
}
