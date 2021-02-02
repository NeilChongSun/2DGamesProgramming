#ifndef INCLUDED_TILEMAP_H
#define INCLUDED_TILEMAP_H
#include"XEngine.h"

class TileMap
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static TileMap& Get();

	void Load();
	void LoadMap(std::filesystem::path fileName);
	void LoadTexture(std::filesystem::path fileName);
	void SaveMap(std::filesystem::path savePath);
	void Unload();
	void Update(float deltaTime);
	void Render();
	bool CheckCollision(const X::Math::LineSegment& lineSegment) const;
	void ShowEditor();
	void SetGrid();

	X::Math::Rect GetBound() const;

private:
	int GetIndex(int col, int row,int numColumns)const;
	
	std::unique_ptr<int[]> mMap;
	std::unique_ptr<int[]> mLayer2;

	int mColumns = 0;
	int mRows = 0;

	std::vector<X::TextureId> mTiles;
	X::TextureId mEnemyId;
	X::TextureId mPlayerId;

	int mCurrentTile;
	bool mIsShowGrid = true;
	
	bool mIsDrawTexture = false;
	X::Math::Vector2 mClickPos;

	int mSelect = 0;

	std::vector<X::Math::LineSegment> mVerticalLine;
	std::vector<X::Math::LineSegment> mHorizontalLine;
};
#endif // !INCLUDED_TILEMAP_H
