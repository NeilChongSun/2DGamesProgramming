#include "Shotgun.h"

void Shotgun::Load()
{
	mSurvivorTextureId = X::LoadTexture("survivor_shotgun.png");
	mCrosshairTextureId = X::LoadTexture("crosshair_shotgun.png");
	mAmmoTextureId = X::LoadTexture("ammo_shotgun.png");
	mAmmoCount = 6;
	mDamage = 40;
}

void Shotgun::Unload()
{
}

void Shotgun::Render()
{
	X::Math::Vector2 pos;
	pos.x = 50.0f;
	pos.y = X::GetScreenHeight() - 100.0f;
	for (size_t i = 0; i < mAmmoCount; i++)
	{
		X::DrawSprite(mAmmoTextureId, pos);
		pos.x += 25.0f;
	}
}

void Shotgun::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & direction)
{
	mLine.push_back(X::Math::LineSegment((position + (direction*80.0f)), position + ((X::Math::Rotate(direction, 10.0f * X::Math::kDegToRad))*500.0f)));	
	mLine.push_back(X::Math::LineSegment((position + (direction*80.0f)), position + ((X::Math::Rotate(direction, -10.0f * X::Math::kDegToRad))*500.0f)));
	mLine.push_back(X::Math::LineSegment((position + (direction*80.0f)), (position+(direction)*500.0f)));

	for (auto it=mLine.begin();it!=mLine.end();)
	{
		auto i = *it;
		X::DrawScreenLine(i.from, i.to, X::Colors::Red);
		++it;
	}

	mAmmoCount--;
}

bool Shotgun::CanFire() const
{
	if (mAmmoCount > 0)
		return true;
	else
		return false;
}

void Shotgun::Reload()
{
	mAmmoCount = 6;
}

void Shotgun::ResetLine()
{
	mLine.clear();
}


