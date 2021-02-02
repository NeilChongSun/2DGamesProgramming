#include "Rifle.h"

void Rifle::Load()
{
	mSurvivorTextureId = X::LoadTexture("survivor_rifle.png");
	mCrosshairTextureId = X::LoadTexture("crosshair_rifle.png");
	mAmmoTextureId = X::LoadTexture("ammo_rifle.png");
	mAmmoCount = 30;
	mDamage = 40;
}

void Rifle::Unload()
{
}

void Rifle::Render()
{
	X::Math::Vector2 pos;
	pos.x = 50.0f;

	pos.y = -5000.0f;
	for (size_t i = 0; i < mAmmoCount; i++)
	{
		X::DrawSprite(mAmmoTextureId, pos);
		pos.x += 25.0f;
	}
}

double fireRate = 0.0f;
void Rifle::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & direction)
{
	if (fireRate < X::GetTime())
	{
		mLine.push_back(X::Math::LineSegment((position + (direction*50.0f)), (position + (direction*1500.0f))));
		for (auto it = mLine.begin(); it != mLine.end();)
		{
			auto i = *it;
			X::DrawScreenLine(i.from, i.to, X::Colors::Red);
			++it;
		}
		--mAmmoCount;
		fireRate = X::GetTime() + 0.1f;
	}
}

void Rifle::ResetLine()
{
	mLine.clear();
}


bool Rifle::CanFire() const
{
	if (mAmmoCount > 0)
		return true;
	else
		return false;
}

void Rifle::Reload()
{
	mAmmoCount = 30;
}

