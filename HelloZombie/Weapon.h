#pragma once

#include <XEngine.h>

class Weapon
{
public:
	virtual ~Weapon() = default;

	virtual void Load() = 0;

	virtual void Unload() = 0;

	//homework : for each weapon draw ammo remaining on screen;
	virtual void Render() = 0;

	virtual void Fire(const X::Math::Vector2& position,const X::Math::Vector2& direction) = 0;

	virtual bool CanFire() const = 0;
	virtual void Reload() = 0;

	const int GetDamage() const { return mDamage; }
	//X::Math::LineSegment GetLine()const { return mLine; }
	std::vector<X::Math::LineSegment> GetLine() const { return mLine; }

	X::TextureId GetSurvivorTextureId() const { return mSurvivorTextureId; }
	X::TextureId GetCrosshairTextureId() const { return mCrosshairTextureId; }
	virtual void ResetLine() = 0;

protected:

	X::TextureId mSurvivorTextureId = 0;
	X::TextureId mCrosshairTextureId = 0;
	X::TextureId mAmmoTextureId = 0;
	int mDamage = 0;
	int mAmmoCount = 0;
	//X::Math::LineSegment mLine;
	std::vector<X::Math::LineSegment> mLine;
	double mReloadTimer = 0;
};