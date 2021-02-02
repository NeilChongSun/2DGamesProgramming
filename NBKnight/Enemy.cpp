#include "Enemy.h"
#include"Camera.h"
#include"Player.h"
#include"Level.h"
void Enemy::Load()
{
	mTextureId = X::LoadTexture("NBEnemy.png");
}

void Enemy::Unload()
{

}

void Enemy::Update(float deltaTime)
{
	//Enemy AI
#pragma region AI
	//Set jump force
	mSpeedY += 12.0f*deltaTime*60.0f;
	X::Math::Vector2 targetPos;
	const X::Math::Vector2& playerPos = Player::Get().GetPosition();
	//Let target= player
	X::Math::Vector2 displacement;
	if (X::Math::Abs(playerPos.x - mPosition.x) <= mAlertRange.x&&X::Math::Abs(playerPos.y - mPosition.y) <= mAlertRange.y)
	{
		targetPos = playerPos;
		//Ememy Jump
		if (mCanJump&&targetPos.y < mPosition.y - 32.0f)
		{
			mSpeedY = -380.0f;
			mCanJump = false;
		}
		//If target is player:
		Fire();
	}
	//Let target = Spawn point
	else
	{
		targetPos = mStartPosition;
		//jump back to spawn point
		if (mCanJump&&targetPos.y < mPosition.y-32.0f)
		{
			mSpeedY = -380.0f;
			mCanJump = false;
		}
		//come back spawn point direction if too far
		if (X::Math::Abs(mPosition.y - targetPos.y) >= 500.0f || (X::Math::Abs(mPosition.x - targetPos.x) <= 30.0f&& mPosition.y-targetPos.y >= 64.0f))
			mPosition = targetPos;
			
	}
	//left or right
	mHeading = X::Math::Normalize((targetPos + X::Math::Vector2(0.1f, 0.0f)) - mPosition);

	displacement.x = mHeading.x*mSpeedX*deltaTime;
	displacement.y = mSpeedY * deltaTime;
	
	if (X::Math::Abs(mPosition.y - targetPos.y) <= 1.5 && (X::Math::Abs(mPosition.x - targetPos.x) <= 1))
		displacement.x = 0;
#pragma endregion
#pragma region CheckCollision
	//Check edge
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
			displacement.y = 0.0f;
			mSpeedY = 0.0f;
			mCanJump = true;
		}
		else
		{
			mCanJump = false;
		}
	}
#pragma endregion
	mPosition += displacement;

#pragma region Animation
	//TODO: animation
	if (displacement.x > 0)
		mFaceLeft = false;
	else
		mFaceLeft = true;

	if(displacement.x==0)
	{
		mIsMove = false;
		mAnimTime = 0;
	}
	else
		mIsMove = true;

	if (mIsMove)
	{
		if (mFaceLeft)
		{
			if (mAnimTime < X::GetTime())
			{
				if (mTileSheetIndex==2)
				{
					mTileSheetIndex = 0;
				}
				mAnimTime = X::GetTime() + 0.05f;
				mTileSheetIndex++;
			}
		}
		else
		{
			if (mAnimTime < X::GetTime())
			{
				if (mTileSheetIndex == 2)
				{
					mTileSheetIndex = 0;
				}
				mAnimTime = X::GetTime() + 0.1f;
				mTileSheetIndex++;
			}
		}		
	}

#pragma endregion
#pragma region CheckDamage
	//Intersect with damage circle will be damaged
	mCircle = X::Math::Circle(mPosition, 16);
	X::Math::Circle playerAttackCircle = Player::Get().GetAttackCircle();
	if (X::Math::Intersect(mCircle, playerAttackCircle))
	{
		mHealth -= Player::Get().GetDamage();
		mPosition.x -= mHeading.x * 50.0f;
	}
#pragma endregion	
	BulletUpdate(deltaTime);
}
void Enemy::Render()
{
	X::Math::Vector2 enemyScreenPos = Camera::Get().ConvertToScreenPosition(mPosition);
	if (!mIsMove)
		X::DrawSprite(mTextureId, X::Math::Rect(0,0,32,32),enemyScreenPos);
	else
	{
		if (mFaceLeft)
			X::DrawSprite(mTextureId, X::Math::Rect(32*mTileSheetIndex, 32, 32 * (mTileSheetIndex + 1), 32 *2), enemyScreenPos);
		else
			X::DrawSprite(mTextureId, X::Math::Rect(32 * mTileSheetIndex, 32 * 2, 32 * (mTileSheetIndex + 1), 32 * 3), enemyScreenPos);
	}
	
	//Render bullet
	for (auto bullet : mBullet)
		bullet->Render();
	//test 
	if (X::IsKeyDown(X::Keys::K))
	{
		X::Math::Vector2 enemyScreenPos = Camera::Get().ConvertToScreenPosition(mPosition);
		X::Math::Vector2 testAlertRangeXAxis;
		X::Math::Vector2 testAlertRangeYAxis;
		testAlertRangeXAxis = X::Math::Vector2(enemyScreenPos.x - mAlertRange.x, enemyScreenPos.x + mAlertRange.x);
		X::DrawScreenLine(testAlertRangeXAxis.x, enemyScreenPos.y, testAlertRangeXAxis.y, enemyScreenPos.y, X::Colors::Pink);

		testAlertRangeYAxis = X::Math::Vector2(enemyScreenPos.y + mAlertRange.y, enemyScreenPos.y - mAlertRange.y);
		X::DrawScreenLine(enemyScreenPos.x, testAlertRangeYAxis.x, enemyScreenPos.x, testAlertRangeYAxis.y, X::Colors::Pink);

		X::DrawScreenCircle(Camera::Get().ConvertToScreenPosition(mCircle.center).x, Camera::Get().ConvertToScreenPosition(mCircle.center).y, mCircle.radius, X::Colors::Red);
	}	
}

X::Math::Rect Enemy::GetBoundingBox() const
{
	return{
		mPosition.x - 15.0f,
		mPosition.y - 15.0f,
		mPosition.x + 15.0f,
		mPosition.y + 15.0f,
	};
}

bool Enemy::Die()
{
	if (mHealth <= 0.0f)
		return true;
	return false;
}

void Enemy::Fire()
{
	if (mNextAttackTime < X::GetTime())
	{
		//Fire
		Bullet* bullet = new Bullet();
		bullet->Load();
		bullet->SetPosition(mPosition);
		bullet->SetHeading(mBulletDirection);
		mBullet.push_back(bullet);
		//Pick the next fire in the future
		mNextAttackTime = X::GetTime() + X::RandomFloat(1, 3);
	}
}

void Enemy::BulletUpdate(float deltaTime)
{
	mBulletDirection = X::Math::Normalize(Player::Get().GetPosition() - mPosition);	
	for (auto iter = mBullet.begin(); iter != mBullet.end();)
	{
		Bullet* bullet = (*iter);
		if (!bullet->Destroy())
		{
			bullet->Update(deltaTime);
		}

		if (bullet->Destroy())
		{
			bullet->Unload();
			delete bullet;
			iter = mBullet.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}





