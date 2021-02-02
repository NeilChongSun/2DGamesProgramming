#pragma once
#include"XEngine.h"

class Level
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static Level& Get();

	void LoadLevel(std::filesystem::path fileName);
	void LoadTexture(std::filesystem::path fileName);
	void Unload();
	void Update(float deltaTime);
	void Render();
	bool CheckCollision(const X::Math::LineSegment& lineSegment) const;
	X::Math::Rect GetBound() const;
	const std::vector<X::Math::Vector2>& GetEmenySpawnPoint() const { return mEnemySpawnPoint; }

	const X::Math::Vector2& GetPlayerPosition() { return mPlayerSpawnPoint; }
private:
	int GetIndex(int col, int row, int numColumns) const;

	std::unique_ptr<int[]> mLevel;
	std::unique_ptr<int[]> mLayer2;

	int mColumns = 0;
	int mRows = 0;
	std::vector<X::TextureId> mTiles;
	std::vector<X::Math::Vector2> mEnemySpawnPoint;
	X::Math::Vector2 mPlayerSpawnPoint = { 50.0f,50.0f };
};