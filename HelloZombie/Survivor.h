#pragma once

#include <XEngine.h>

class Weapon;

class Survivor
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	const X::Math::Vector2& GetPosition() const { return mPosition; }
	const Weapon* GetCurrentWeapon() const { return mCurrentWeapon; }
	void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
private:

	std::array<Weapon*, 3> mWeapons;
	Weapon* mCurrentWeapon = nullptr;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mHeading;
	//float mAngle;
};
