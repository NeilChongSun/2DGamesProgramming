#pragma once
#include<XEngine.h>
#include"Bullet.h"
class Enemy
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	X::Math::Rect GetBoundingBox() const;
	void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
	void SetStartPosition(const X::Math::Vector2& position) { mStartPosition = position; }
	X::Math::Circle GetCircle()const { return mCircle; }
	bool Die();
	void Fire();
	void BulletUpdate(float DeltaTime);

protected:
	X::TextureId mTextureId=0;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mStartPosition;
	X::Math::Circle mCircle;
	X::Math::Vector2 mHeading;
	X::Math::Vector2 mBulletDirection;
	float mHealth = 150.0f;
	float mSpeedY = 0.0f;
	float mSpeedX = 180.0f;
	bool mCanJump = true;
	bool mFaceLeft = false;
	bool mIsMove = false;
	int mDamage = 1;

	double mAnimTime = 0.0f;
	int mTileSheetIndex = 0;
	//Player in this area will be found by enemy
	X::Math::Vector2 mAlertRange = { 250.0f,100.0f };
	std::vector<Bullet*> mBullet;
	float mNextAttackTime = 0.5f;
};