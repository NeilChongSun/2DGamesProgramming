#include<XEngine.h>
#include"TileMap.h"
#include<ImGui/Inc/imgui.h>
#include"Link.h"
#include"Camera.h"

Camera myCamera;
Link link;
bool showEditor=true;
void GameInit()
{
	TileMap::StaticInitialize();
	TileMap::Get().LoadMap("test1.txt");
	TileMap::Get().LoadTexture("tile.txt");
	TileMap::Get().Load();
	link.Load();

	myCamera.StaticInitialize();
}

void GameCleanUp()
{
	TileMap::StaticTerminate();
	myCamera.StaticTerminate();

}

bool GameLoop(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::GRAVE))
		showEditor = !showEditor;
	if (showEditor)
	{
		TileMap::Get().ShowEditor();
	}

	Camera::Get().SetViewBound(TileMap::Get().GetBound());
	Camera::Get().SetViewPosition(link.GetPosition());

	TileMap::Get().Update(deltaTime);
	link.Update(deltaTime);

	Camera::Get().SetViewPosition(link.GetPosition());

	TileMap::Get().Render();
	link.Render();


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
