#include "Player.h"
#include"Level.h"
#include"Camera.h"
#include"EnemyManager.h"

namespace
{
	Player* sInstance = nullptr;
}

void Player::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "Player already initialized!");
	sInstance = new Player();
}

void Player::StaticTerminate()
{
	delete sInstance;
	sInstance = nullptr;
}

Player & Player::Get()
{
	XASSERT(sInstance != nullptr, "No Player created!");
	return *sInstance;
}


void Player::Load()
{
	mLifeTextureId = X::LoadTexture("heart.png");
	mIdleTextureId = X::LoadTexture("NBIdle.png");

	mAttackTextureId.push_back(X::LoadTexture("NBAttack0.png"));
	mAttackTextureId.push_back(X::LoadTexture("NBAttack1.png"));
	mAttackTextureId.push_back(X::LoadTexture("NBAttack2.png"));
	mAttackTextureId.push_back(X::LoadTexture("NBAttack3.png"));

	mMoveTextureId.push_back(X::LoadTexture("NBRun0.png"));
	mMoveTextureId.push_back(X::LoadTexture("NBRun1.png"));
	mMoveTextureId.push_back(X::LoadTexture("NBRun2.png"));
	mMoveTextureId.push_back(X::LoadTexture("NBRun3.png"));
	mMoveTextureId.push_back(X::LoadTexture("NBRun4.png"));

	mCurrentTextureId = mIdleTextureId;

	mPosition = Level::Get().GetPlayerPosition(); //{ 50.0f, 50.0f };
}

void Player::Unload()
{

}

void Player::Update(float deltaTime)
{
	//Gravity
	mSpeedY += 12.0f * deltaTime*60.0f;
	X::Math::Vector2 displacement;
	//Move
	if (X::IsKeyDown(X::Keys::D))
		displacement.x = mSpeedX * deltaTime;
	if (X::IsKeyDown(X::Keys::A))
		displacement.x = -mSpeedX * deltaTime;
	//Jump	
	if (X::IsKeyPressed(X::Keys::SPACE) && mCanJump)
	{
		mCanJump = false;
		mSpeedY = -380.0f;
	}
	displacement.y = mSpeedY * deltaTime;
#pragma region CheckCollision
	auto currentBox = GetBoundingBox();
	if (displacement.x > 0.0f)
	{
		X::Math::LineSegment rightEdge{
			currentBox.max.x + displacement.x,
			currentBox.min.y,
			currentBox.max.x + displacement.x,
			currentBox.max.y
		};
		if (Level::Get().CheckCollision(rightEdge))
			displacement.x = 0;
	}

	if (displacement.x < 0.0f)
	{
		X::Math::LineSegment leftEdge{
			currentBox.min.x + displacement.x,
			currentBox.min.y,
			currentBox.min.x + displacement.x,
			currentBox.max.y
		};
		if (Level::Get().CheckCollision(leftEdge))
			displacement.x = 0;
	}

	if (displacement.y < 0.0f)
	{
		X::Math::LineSegment TopEdge{
			currentBox.min.x,
			currentBox.min.y + displacement.y,
			currentBox.max.x,
			currentBox.min.y + displacement.y
		};
		if (Level::Get().CheckCollision(TopEdge))
		{
			mSpeedY = 0.0f;
			displacement.y = 0;
		}
	}

	if (displacement.y >= 0.0f)
	{
		X::Math::LineSegment BottomEdge{
			currentBox.min.x,
			currentBox.max.y + displacement.y,
			currentBox.max.x,
			currentBox.max.y + displacement.y
		};
		if (Level::Get().CheckCollision(BottomEdge))
		{
			mCanJump = true;
			displacement.y = 0.0f;
			mSpeedY = 0.0f;
		}
		/*else
			mCanJump = false;*/
	}
#pragma endregion
#pragma region Animation
	if (displacement.x > 0)
		mFaceLeft = false;
	else if (displacement.x < 0)
		mFaceLeft = true;
	mPosition += displacement;
	//Move Animation
	if (displacement.x == 0)
		mIsMove = false;
	if (displacement.x != 0)
		mIsMove = true;
	if (mIsMove && !mIsPlayAttackAnimation)
	{
		if (mMoveTime < X::GetTime() && mIsMove&&displacement.y == 0)
		{
			if (mMoveIndex == mMoveTextureId.size())
			{
				mMoveIndex = 0;
			}
			mMoveTime = X::GetTime() + 0.05f;
			mCurrentTextureId = mMoveTextureId[mMoveIndex];
			mMoveIndex++;
		}
	}
	//Attack Animation
	if (X::IsKeyPressed(X::Keys::J))
	{
		mIsPlayAttackAnimation = true;
		mIsAttack = true;
	}

	if (mAttackTime < X::GetTime() && mIsPlayAttackAnimation)
	{
		if (mAttackIndex == mAttackTextureId.size())
		{
			mIsPlayAttackAnimation = false;
			mAttackIndex = 0;
		}
		mCurrentTextureId = mAttackTextureId[mAttackIndex];
		mAttackIndex++;
		mAttackTime = X::GetTime() + 0.02f;
	}

	if (!mIsPlayAttackAnimation && !mIsMove)
	{
		mCurrentTextureId = mIdleTextureId;
	}
#pragma endregion 
	mCircle = X::Math::Circle(mPosition, 15.0f);
#pragma region Attack
	if (mFaceLeft)
		mAttackDistance = -28.0f;
	else if (!mFaceLeft)
		mAttackDistance = 28.0f;
	mAttackCircle = X::Math::Circle(0, 0, 0);
	if (mIsAttack)
	{
		mAttackCircle = X::Math::Circle(mPosition.x + mAttackDistance, mPosition.y - 5.0f, 18.0f);
		mIsAttack = false;
	}
#pragma endregion
}

void Player::Render()
{
	X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(mPosition);
	if (mLife > 0)
	{
		if (mFaceLeft)
			X::DrawSprite(mCurrentTextureId, screenPos, X::Pivot::Center, X::Flip::Horizontal);
		else
			X::DrawSprite(mCurrentTextureId, screenPos);
	}
	//test
	if (X::IsKeyDown(X::Keys::K))
	{
		X::DrawScreenCircle(Camera::Get().ConvertToScreenPosition(mPosition).x, Camera::Get().ConvertToScreenPosition(mPosition).y, 15.0f, X::Colors::Blue);
		X::DrawScreenCircle(Camera::Get().ConvertToScreenPosition(mAttackCircle.center).x, Camera::Get().ConvertToScreenPosition(mAttackCircle.center).y, mAttackCircle.radius, X::Colors::Purple);
	}	
	//Left Life
	X::Math::Vector2 pos;
	pos.x = 20.0f;
	pos.y = 20.0f;
	for (int i = 0; i < mLife; i++)
	{
		X::DrawSprite(mLifeTextureId, pos);
		pos.x += 25.0f;
	}	
}

X::Math::Rect Player::GetBoundingBox() const
{
	return{
		mPosition.x - 13.0f,
		mPosition.y - 12.0f,
		mPosition.x + 13.0f,
		mPosition.y + 13.0f,
	};
}

void Player::TakeDamage(int damage)
{
	mLife -= damage;
}

