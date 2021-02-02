#pragma once

#include<XEngine.h>

class Enemy
{
public:
	void Load();
	void Unload();

	void Updata(float deltaTime);
	void Render();
	void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
	void CheckCollision(X::Math::LineSegment line, X::Math::Circle enemy);
	void TakeDamage();
	bool Die();
	bool CanDelete();

protected:
	X::TextureId mTextureId=0;
	X::TextureId mTextureId2 = 0;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mHeading;
	float mSpeed = 100.0f;
	int mHealth = 100;
	double mDieTime = 0;
	X::Math::Circle mCircle;
};