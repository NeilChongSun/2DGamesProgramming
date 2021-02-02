#include "Handgun.h"

void Handgun::Load()
{
	mSurvivorTextureId = X::LoadTexture("survivor_handgun.png");
	mCrosshairTextureId = X::LoadTexture("crosshair_handgun.png");
	mAmmoTextureId = X::LoadTexture("ammo_handgun.png");
	mAmmoCount = 12;
	mDamage = 50;
}

void Handgun::Unload()
{
}

void Handgun::Render()
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

void Handgun::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & direction)
{
	mLine.push_back(X::Math::LineSegment((position + (direction*50.0f)), (position + (direction*1500.0f))));
	for (auto it = mLine.begin(); it != mLine.end();)
	{
		auto i = *it;
		X::DrawScreenLine(i.from, i.to, X::Colors::Red);
		++it;
	}
	mAmmoCount--;
}

bool Handgun::CanFire() const
{
	if (mAmmoCount > 0)
		return true;
	else
		return false;
}

void Handgun::Reload()
{

		mAmmoCount = 12;

}

void Handgun::ResetLine()
{
	mLine.clear();
}


