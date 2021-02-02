#pragma once
#include<XEngine.h>
class Player
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static Player& Get();


public:
	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();
	
	X::Math::Rect GetBoundingBox() const;
	X::Math::Circle GetAttackCircle() const { return mAttackCircle; }
	X::Math::Circle GetCircle() const { return mCircle; }
	const float GetDamage() const { return mDamage; }
	const X::Math::Vector2& GetPosition() const { return mPosition; }
	void SetPosition(X::Math::Vector2 pos) { mPosition = pos; }
	void TakeDamage(int damage);

	const bool IsLost() { return mLife <= 0; }

private:
	//X::TextureId mTextureId;
	std::vector<X::TextureId> mAttackTextureId;
	std::vector<X::TextureId> mMoveTextureId;
	X::TextureId mIdleTextureId;
	X::TextureId mCurrentTextureId;
	X::TextureId mLifeTextureId;
	X::Math::Vector2 mPosition;
	X::Math::Circle mCircle;
	X::Math::Circle mAttackCircle;

	float mAttackDistance;
	float mDamage = 50.0f;

	bool mFaceLeft = false;
	bool mIsPlayAttackAnimation=false;
	bool mIsMove=false;
	bool mCanJump = true;
	bool mIsAttack=false;
	bool mCanAttace = true;

	bool mIsInvulnerable = false;

	float mSpeedY = 0.0f;
	float mSpeedX = 250.0f;

	double mAttackTime=0.0f;
	double mMoveTime = 0.0f;
	double mInvulnerableTime = 0.0f;

	int mAttackIndex = 0;
	int mMoveIndex = 0;

	int mLife = 5;
};