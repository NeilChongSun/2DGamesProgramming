#include "Survivor.h"
#include"Enemy.h"
#include"Handgun.h"
#include"Shotgun.h"
#include"Rifle.h"
Enemy enemy;
namespace
{
	X::Math::Vector2 GetTargetPostion()
	{
		int mouseX = X::GetMouseScreenX();
		int mouseY = X::GetMouseScreenY();
		return{ static_cast<float>(mouseX), static_cast<float>(mouseY) };
	}
}


void Survivor::Load()
{
	mWeapons[0] = new Handgun();
	mWeapons[1] = new Rifle();
	mWeapons[2] = new Shotgun();

	for (auto& w : mWeapons)
	{
		w->Load();
	}

	//Select the first weapon by default
	mCurrentWeapon = mWeapons[0];
}

void Survivor::Unload()
{
	mCurrentWeapon = nullptr;
	for (auto& w : mWeapons)
	{
		w->Unload();
		delete w;
	}
}

void Survivor::Update(float deltaTime)
{
	mCurrentWeapon->ResetLine();
	//move
	const float moveSpeed = 250.0f;
	if (X::IsKeyDown(X::Keys::D))
		mPosition.x += moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::A))
		mPosition.x -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::W))
		mPosition.y -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::S))
		mPosition.y += moveSpeed * deltaTime;
	//switch weapon
	if (X::IsKeyPressed(X::Keys::ONE))
		mCurrentWeapon = mWeapons[0];
	if (X::IsKeyPressed(X::Keys::TWO))
		mCurrentWeapon = mWeapons[1];
	if (X::IsKeyPressed(X::Keys::THREE))
		mCurrentWeapon = mWeapons[2];
	//fire
	if (X::IsMousePressed(X::Mouse::LBUTTON) && mCurrentWeapon->CanFire())
	{
		mCurrentWeapon->Fire(mPosition, mHeading);
	}
	if (X::IsMouseDown(X::Mouse::LBUTTON) && mCurrentWeapon->CanFire()&&mCurrentWeapon==mWeapons[1])
	{
		mCurrentWeapon->Fire(mPosition, mHeading);
	}

	if (X::IsMousePressed(X::Mouse::RBUTTON))
		mCurrentWeapon->Reload();

	//mAngle = -atan2(mPosition.y - X::GetMouseScreenY(), X::GetMouseScreenX()- mPosition.x);
	//X::DrawScreenText(std::to_string(mAngle).c_str(), 222, 222, 14, X::Colors::Red);

	X::Math::Vector2 target = GetTargetPostion();
	mHeading = X::Math::Normalize(target - mPosition);
}

void Survivor::Render()
{
	float angle = atan2(mHeading.y, mHeading.x);
	X::DrawSprite(mCurrentWeapon->GetSurvivorTextureId(), mPosition, angle);
	X::Math::Vector2 target = GetTargetPostion();
	X::DrawSprite(mCurrentWeapon->GetCrosshairTextureId(), target);
	mCurrentWeapon->Render();
}
