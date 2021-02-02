#pragma once
#include "Level.h"
#include"Camera.h"
namespace
{
	std::unique_ptr<Level>sInstance;
}
void Level::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "Level already initialized!");
	sInstance = std::make_unique<Level>();
}

void Level::StaticTerminate()
{
	sInstance.reset();
}

Level & Level::Get()
{
	XASSERT(sInstance != nullptr, "No Level created!");
	return *sInstance;
}

void Level::LoadLevel(std::filesystem::path fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	
	fscanf_s(file, "Columns: %d\n", &mColumns);
	fscanf_s(file, "Rows: %d\n", &mRows);
	fscanf_s(file, "Layer_1\n");

	mLevel = std::make_unique<int[]>(mColumns*mRows);
	mLayer2 = std::make_unique<int[]>(mColumns*mRows);

	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			fscanf_s(file, "%d ", &mLevel[i]);
		}
		fscanf_s(file, "\n");
	}

	fscanf_s(file, "Layer_2\n");

	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			fscanf_s(file, "%d ", &mLayer2[i]);
			if (mLayer2[i]==2)
				mEnemySpawnPoint.push_back({ x*32.0f+16.0f, y*32.0f+16.0f });
			if (mLayer2[i]==3)
				mPlayerSpawnPoint = { x*32.0f+16.0f, y*32.0f+16.0f };
		}
		fscanf_s(file, "\n");
	}

	/*for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			if (mLevel[i] == 2)
			{
				mEnemySpawnPoint.push_back({ x*32.0f, y*32.0f });
			}
		}
	}*/
	fclose(file);
}

void Level::LoadTexture(std::filesystem::path fileName)
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
}

void Level::Unload()
{
	mLevel.reset();
	mLayer2.reset();
	mColumns = 0;
	mRows = 0;
	mTiles.clear();
	mEnemySpawnPoint.clear();
	mPlayerSpawnPoint = 0;
}

void Level::Update(float deltaTime)
{
	

}

void Level::Render()
{
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y, mColumns);
			X::TextureId id = mTiles[mLevel[i]];
			X::Math::Vector2 worldPos{ x*32.0f,y*32.0f };
			X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(worldPos);
			X::DrawSprite(id, screenPos, X::Pivot::TopLeft);
		}
	}
}

bool Level::CheckCollision(const X::Math::LineSegment & lineSegment) const
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
			if (mLayer2[index] == 1)
			{
				return true;
			}
		}
	}
	return false;
	
}

X::Math::Rect Level::GetBound() const
{
	return{ 0.0f,0.0f,mColumns*32.0f,mRows*32.0f };
}

int Level::GetIndex(int col, int row, int numColumns) const
{
	return col + (row*numColumns);
}
