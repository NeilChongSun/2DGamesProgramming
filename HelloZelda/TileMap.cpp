#include "TileMap.h"
#include<ImGui/Inc/imgui.h>
#include"Camera.h"
namespace {
	std::unique_ptr<TileMap> sInstance;
}
void TileMap::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "tilemap already initialized!");
	sInstance = std::make_unique<TileMap>();
}

void TileMap::StaticTerminate()
{
	sInstance.reset();
}

TileMap & TileMap::Get()
{
	XASSERT(sInstance != nullptr, "No TileMap created!");
	return *sInstance;
}

void TileMap::Load()
{
		
}

void TileMap::LoadMap(std::filesystem::path fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	fscanf_s(file, "Columns: %d\n", &mColumns);
	fscanf_s(file, "Rows: %d\n", &mRows);
	fscanf_s(file, "Layer_1\n");
	
	mMap = std::make_unique<int[]>(mColumns*mRows);
	mLayer2 = std::make_unique<int[]>(mColumns*mRows);

	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			fscanf_s(file, "%d ", &mMap[i]);
			//mMap[i] = fgetc(file) - '0';
			//fgetc(file);
		}
		//fgetc(file);
		fscanf_s(file, "\n");
	}

	// Load Obstacle, Enemy, and Player
	fscanf_s(file, "Layer_2\n");
	
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			fscanf_s(file, "%d ", &mLayer2[i]);
		}
		fscanf_s(file, "\n");
	}
		
	fclose(file);

	mCurrentTile = 0;
	SetGrid();
}

void TileMap::LoadTexture(std::filesystem::path fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	int count = 0;
	fscanf_s(file, "Count: %d\n", &count);

	mTiles.resize(count);
	for (int i = 0; i < count; i++)
	{
		char buffer[256];
		fscanf_s(file, "%s\n", buffer, static_cast<int>(std::size(buffer)));
		mTiles[i] = X::LoadTexture(buffer);
	}
	fclose(file);
	mEnemyId = X::LoadTexture("NBEnemy_0.png");
	mPlayerId = X::LoadTexture("zelda.png");
}

void TileMap::SaveMap(std::filesystem::path savePath)
{
	std::ofstream write;
	write.open(savePath);
	write << "Columns: " << mColumns << std::endl;
	write << "Rows: " << mRows << std::endl;
	write << "Layer_1" << std::endl;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			write << mMap[i] << " ";
		}
		write << "\n";
	}

	//Save Obstacle, Enemy, and Player
	write << "Layer_2" << std::endl;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			write << mLayer2[i] << " ";
		}
		write << "\n";
	}

	write.close();
}


void TileMap::ShowEditor()
{
	//Start ImGui
	ImGui::SetNextWindowPos({ X::GetScreenWidth() - 300.0f,0.0f });
	ImGui::SetNextWindowSize({ 300.0f, 400.0f });
	ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_NoResize);

	//Buttons
	if (ImGui::Button("Open"))
	{
		char filePath[MAX_PATH]{};
		const char* title = "Open Level";
		const char* filter = "Text Files (*txt)\0*.txt";
		if (X::OpenFileDialog(filePath, title, filter))
		{
			LoadMap(filePath);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		char filePath[MAX_PATH]{};
		const char* title = "Save Map";
		const char* filter = "Text Files (*txt)\0*.txt";
		if (X::SaveFileDialog(filePath, title, filter))
		{
			std::filesystem::path savePath = filePath;
			if (savePath.extension().empty())
				savePath += ".txt";

			SaveMap(savePath);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("New"))
	{

		mMap = std::make_unique<int[]>(mColumns*mRows);
		mLayer2 = std::make_unique<int[]>(mColumns*mRows);
	}

	ImGui::SameLine();
	ImGui::Checkbox("Show Grid", &mIsShowGrid);
	ImGui::Separator();
	int columnTemp = mColumns;
	int rowTemp = mRows;

	if (ImGui::SliderInt("Colums", &columnTemp, 1, 100))
	{
		/*	std::unique_ptr<int[]> newMap = std::make_unique<int[]>(columnTemp*mRows);

			for (int y = 0; y < mRows; y++)
			{
				for (int x = 0; x < columnTemp; x++)
				{
					int i = GetIndex(x, y, columnTemp);
					if (x>mColumns)
					{
						newMap[i] = 0;
					}
					else if (x<=mColumns)
					{
						newMap[i] = mMap[i];
					}
				}
			}*/
		mColumns = columnTemp;
		mMap = std::make_unique<int[]>(mColumns*mRows);
		mLayer2 = std::make_unique<int[]>(mColumns*mRows);
		SetGrid();

		/*for (int i = 0; i < mColumns*mRows; i++)
		{
			mMap[i] = newMap[i];
		}*/
	}

	if (ImGui::SliderInt("Rows", &rowTemp, 1, 100))
	{
		mRows = rowTemp;
		mMap = std::make_unique<int[]>(mColumns*mRows);
		mLayer2 = std::make_unique<int[]>(mColumns*mRows);
		SetGrid();
	}

	ImGui::Separator();

	ImGui::RadioButton("Texture", &mSelect, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Obstacle", &mSelect, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Enemy", &mSelect, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Player", &mSelect, 3);
	
	if (mSelect == 0)
		mIsDrawTexture = true;
	else
		mIsDrawTexture = false;

	//Show the image that I choose currently
	ImGui::Text("Current: ");
	ImGui::SameLine();

	if (!mIsDrawTexture)
	{
		if (mSelect==1)
			ImGui::Text("Obstacle");
		else if(mSelect==2)
			ImGui::Text("Enemy");
		else if(mSelect==3)
			ImGui::Text("Player");
	}
	else
	{
		ImGui::Image(X::GetSprite(mTiles[mCurrentTile]), { 32.0,32.0 });

		ImGui::Separator();

		//Show tiles can be choose
		for (int i = 0; i < (int)mTiles.size(); i++)
		{
			if (ImGui::ImageButton(X::GetSprite(mTiles[i]), { 32.0f,32.0f }))
				mCurrentTile = i;
			if (i % 5 != 4)
				ImGui::SameLine();
		}
	}

	ImGui::End();
}

void TileMap::SetGrid()
{
	mVerticalLine.clear();
	for (int i = 0; i <= mColumns; i++)
	{
		X::Math::Vector2 from = { 32.0f * i, 0.0f };
		X::Math::Vector2 end = { 32.0f * i , 32.0f * mRows };
		X::Math::LineSegment line = X::Math::LineSegment(from, end);
		mVerticalLine.push_back(line);
	}
	mHorizontalLine.clear();
	for (int i = 0; i <= mRows; i++)
	{
		X::Math::Vector2 from = { 0.0f, 32.0f * i };
		X::Math::Vector2 end = { 32.0f * mColumns,32.0f * i };
		X::Math::LineSegment line = X::Math::LineSegment(from, end);
		mHorizontalLine.push_back(line);
	}
}

X::Math::Rect TileMap::GetBound() const
{
	return{ 0.0f,0.0f,mColumns*32.0f,mRows*32.0f };
}

void TileMap::Unload()
{
	mTiles.clear();
	mColumns = 0;
	mRows = 0;
	mMap.reset();
	mLayer2.reset();
}

void TileMap::Update(float deltaTime)
{
	if (!mIsDrawTexture)
	{
		if (X::IsMouseDown(X::Mouse::LBUTTON))
		{
			X::Math::Vector2 mousePos = { static_cast<float>(X::GetMouseScreenX()),static_cast<float>(X::GetMouseScreenY()) };
			X::Math::Vector2 worldPos = Camera::Get().ConvertToWorldPosition(mousePos);
			int col = static_cast<int>(worldPos.x / 32);
			int row = static_cast<int>(worldPos.y / 32);
			int i = GetIndex(col, row, mColumns);
			if (col < mColumns && row < mRows)
			{
				mLayer2[i] = mSelect;
			}
		}		
	}
	else
	{
		if (X::IsMouseDown(X::Mouse::LBUTTON))
		{

			X::Math::Vector2 mousePos = { static_cast<float>(X::GetMouseScreenX()),static_cast<float>(X::GetMouseScreenY()) };
			X::Math::Vector2 worldPos = Camera::Get().ConvertToWorldPosition(mousePos);
			int col = static_cast<int>(worldPos.x / 32);
			int row = static_cast<int>(worldPos.y / 32);
			int i = GetIndex(col, row, mColumns);
			if (col < mColumns && row < mRows)
			{
				mMap[i] = mCurrentTile;
			}
		}
	}
	
	if (X::IsMouseDown(X::Mouse::RBUTTON))
	{
		X::Math::Vector2 mousePos = { static_cast<float>(X::GetMouseScreenX()),static_cast<float>(X::GetMouseScreenY()) };
		X::Math::Vector2 worldPos = Camera::Get().ConvertToWorldPosition(mousePos);
		int col = static_cast<int>(worldPos.x / 32);
		int row = static_cast<int>(worldPos.y / 32);
		int i = GetIndex(col, row, mColumns);
		if (col < mColumns && row < mRows)
		{
			mLayer2[i] = 0;
		}
	}
}

void TileMap::Render()
{
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			//Draw texture
			int i = GetIndex(x, y, mColumns);
			X::TextureId id = mTiles[mMap[i]];
			X::Math::Vector2 worldPos{ x*32.0f,y*32.0f };
			X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(worldPos);
			X::DrawSprite(id, screenPos, X::Pivot::TopLeft);

			//Draw Obstacles
			if (mLayer2[i] == 1)
			{
				//X::DrawScreenRect(x*32.0f, y*32.0f, (x + 1)*32.0f, (y + 1)*32.0f, X::Colors::Red);
				X::Math::Vector2 from1 = { x*32.0f, y*32.0f };
				X::Math::Vector2 to1 = { (x + 1)*32.0f, (y + 1)*32.0f };
				X::Math::Vector2 from2 = { (x+1)*32.0f, y*32.0f };
				X::Math::Vector2 to2 = { x*32.0f, (y + 1)*32.0f };
				X::DrawScreenLine(Camera::Get().ConvertToScreenPosition(from1), Camera::Get().ConvertToScreenPosition(to1), X::Colors::Red);
				X::DrawScreenLine(Camera::Get().ConvertToScreenPosition(from2), Camera::Get().ConvertToScreenPosition(to2), X::Colors::Red);
			}
			//Draw Enemies
			else if (mLayer2[i]==2)
			{				
				X::Math::Vector2 worldPos{ x*32.0f,y*32.0f };
				X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(worldPos);
				X::DrawSprite(mEnemyId, screenPos, X::Pivot::TopLeft);
			}
			//Draw Player
			else if (mLayer2[i]==3)
			{
				X::Math::Vector2 worldPos{ x*32.0f,y*32.0f };
				X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(worldPos);
				X::DrawSprite(mPlayerId, screenPos, X::Pivot::TopLeft);
			}
		}
	}

	if (mIsShowGrid)
	{
		for (auto line : mVerticalLine)
		{
			X::DrawScreenLine(Camera::Get().ConvertToScreenPosition(line.from), Camera::Get().ConvertToScreenPosition(line.to), X::Colors::White);
		}
		for (auto line : mHorizontalLine)
		{
			X::DrawScreenLine(Camera::Get().ConvertToScreenPosition(line.from), Camera::Get().ConvertToScreenPosition(line.to), X::Colors::White);
		}
	}
}

bool TileMap::CheckCollision(const X::Math::LineSegment & lineSegment) const
{
	int startX = static_cast<int>(lineSegment.from.x / 32.0f);
	int startY = static_cast<int>(lineSegment.from.y / 32.0f);
	int endX = static_cast<int>(lineSegment.to.x / 32.0f);
	int endY = static_cast<int>(lineSegment.to.y / 32.0f);
	if (startX<0 || startX>mColumns ||
		startY<0 || startY>mRows ||
		endX<0 || endY>mColumns ||
		endY<0 || endY>mRows)
	{
		return true;
	}

	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; x++)
		{
			int index = GetIndex(x, y, mColumns);
			if (mMap[index] > 3)
			{
				return true;
			}
		}
	}
	return false;
}

int TileMap::GetIndex(int col, int row, int numColumns) const
{
	return col + (row*numColumns);
}