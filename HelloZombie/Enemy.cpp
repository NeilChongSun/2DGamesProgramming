#include "Enemy.h"
#include"Survivor.h"
#include "Weapon.h"
extern Survivor survivor;
void Enemy::Load()
{
	mTextureId = X::LoadTexture("zombie_idle.png");
}

void Enemy::Unload()
{

}

void Enemy::Updata(float deltaTime)
{
	const X::Math::Vector2& targetPos = survivor.GetPosition();
	mHeading = X::Math::Normalize(targetPos - mPosition);
	mPosition += mHeading * mSpeed*deltaTime;
	mCircle = X::Math::Circle(mPosition, X::GetSpriteHeight(mTextureId)*0.5);

	std::vector<X::Math::LineSegment> line = survivor.GetCurrentWeapon()->GetLine();

	for (auto it = line.begin(); it != line.end();)
	{
		CheckCollision(*it, mCircle);
		++it;
	}
}

void Enemy::Render()
{
	float angle = atan2(mHeading.y, mHeading.x);
	if (mHealth <= 0)
	{
		X::DrawSprite(mTextureId2, mPosition, angle);
	}
	else
	{
		X::DrawSprite(mTextureId, mPosition, angle);
	}
}

void Enemy::CheckCollision(X::Math::LineSegment line, X::Math::Circle enemy)
{
	if (X::Math::Intersect(line, enemy))
	{
		TakeDamage();
	}
}

void Enemy::TakeDamage()
{
	mHealth -= survivor.GetCurrentWeapon()->GetDamage();
	if (Die())
	{
		mDieTime = X::GetTime() + 1;
	}
}

bool Enemy::Die()
{
	if (mHealth <= 0)
	{
		return true;
	}
	return false;
}

bool Enemy::CanDelete()
{
	if (Die() && mDieTime < X::GetTime())
	{
		return true;
	}
	return false;
}
