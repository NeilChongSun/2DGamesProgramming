#include "TileMap.h"

namespace
{
	const int testMapColumns = 10;
	const int testMapRows = 10;

	const int testMap[testMapColumns*testMapRows] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	};
}


void TileMap::Load()
{
	mColumns = testMapColumns;
	mRows = testMapRows;
	mMap = new int[mColumns*mRows];

	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y);
			mMap[i] = testMap[i];
		}
	}

	mTiles.push_back(X::LoadTexture("grass.png")); //0
	mTiles.push_back(X::LoadTexture("flower.png"));//1
	mTiles.push_back(X::LoadTexture("tree0.png"));
	mTiles.push_back(X::LoadTexture("tree1.png"));
	mTiles.push_back(X::LoadTexture("tree2.png"));
	mTiles.push_back(X::LoadTexture("tree3.png"));

	mCurrentTile = 0;
	mShowTiles = false;
}

void TileMap::Unload()
{
	mColumns = 0;
	mRows = 0;

	delete[] mMap;
	mMap = nullptr;

	mShowTiles = false;
}

void TileMap::Update(float deltaTime)
{

	if (X::IsKeyPressed(X::Keys::ONE))
		mCurrentTile = 0;
	if (X::IsKeyPressed(X::Keys::TWO))
		mCurrentTile = 1;
	if (X::IsKeyPressed(X::Keys::THREE))
		mCurrentTile = 2;
	if (X::IsKeyPressed(X::Keys::FOUR))
		mCurrentTile = 3;
	if (X::IsKeyPressed(X::Keys::FIVE))
		mCurrentTile = 4;
	if (X::IsKeyPressed(X::Keys::SIX))
		mCurrentTile = 5;

	if (!mShowTiles)
	{
		if (X::IsMousePressed(X::Mouse::LBUTTON) || X::IsMouseDown(X::Mouse::LBUTTON))
		{

			int mouseX = X::GetMouseScreenX();
			int mouseY = X::GetMouseScreenY();
			int col = static_cast<int>(mouseX / 32);
			int row = static_cast<int>(mouseY / 32);
			int i = GetIndex(col, row);
			if (col < mColumns && row < mRows)
			{
				mMap[i] = mCurrentTile;
			}
		}
	}

	if (mShowTiles)
	{
		if (X::IsMousePressed(X::Mouse::LBUTTON))
		{
			int mouseX = X::GetMouseScreenX();
			int mouseY = X::GetMouseScreenY();
			if (mouseX >= mClickPos.x && mouseX <= mClickPos.x + 32.0f*mTiles.size() && mouseY >= mClickPos.y&&mouseY <= mClickPos.y + 32.0f)
			{
				int col = static_cast<int>((mouseX - mClickPos.x) / 32.0f);
				mCurrentTile = col;
			}
			mShowTiles = false;
		}
	}

	if (X::IsMousePressed(X::Mouse::RBUTTON))
	{
		int startMouseX = X::GetMouseScreenX();
		int startMouseY = X::GetMouseScreenY();
		mClickPos = { (float)startMouseX,(float)startMouseY };
		mShowTiles = true;
	}


}

void TileMap::Render()
{
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int i = GetIndex(x, y);
			X::TextureId id = mTiles[mMap[i]];
			X::Math::Vector2 pos{ x*32.0f,y*32.0f };
			X::DrawSprite(id, pos, X::Pivot::TopLeft);
		}
	}

	X::Math::Vector2 pos{ (mColumns + 1) * 32.0f,(mRows + 1) * 32.0f };
	X::DrawSprite(mTiles[mCurrentTile], pos, X::Pivot::TopLeft);

	if (mShowTiles)
	{
		X::Math::Vector2 pos = mClickPos;
		for (auto it = mTiles.begin(); it != mTiles.end();)
		{
			auto i = *it;
			X::DrawSprite(i, pos, X::Pivot::TopLeft);
			pos.x += 33.0f;
			++it;
		}
	}
}

int TileMap::GetIndex(int col, int row) const
{
	return col + (row*mColumns);
}