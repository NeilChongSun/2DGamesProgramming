#ifndef INCLUDED_TILEMAP_H
#define INCLUDED_TILEMAP_H
#include"XEngine.h"
class TileMap
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();

private:
	int GetIndex(int col, int row)const;

	int* mMap = nullptr;
	int mColumns = 0;
	int mRows = 0;

	std::vector<X::TextureId> mTiles;
	int mCurrentTile;
	X::TextureId mId;

	bool mShowTiles;
	X::Math::Vector2 mClickPos;

};
#endif // !INCLUDED_TILEMAP_H
