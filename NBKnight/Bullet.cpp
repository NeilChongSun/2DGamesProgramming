#include "Bullet.h"
#include"Player.h"
#include"Camera.h"
void Bullet::Load()
{
	mTextureId = X::LoadTexture("bullet2.png");
	mDestroyTime +=X::GetTime();
}

void Bullet::Unload()
{
}

void Bullet::Update(float deltaTime)
{
	mPosition.x += mHeading.x * mSpeedX*deltaTime;
	mCircle = X::Math::Circle(mPosition, X::GetSpriteWidth(mTextureId)*0.3f);
	//const X::Math::Vector2& pos = Player::Get().GetPosition();
	const X::Math::Circle& playerCircle = Player::Get().GetCircle();
	if (X::Math::Intersect(playerCircle,mCircle))
	{
		Player::Get().TakeDamage(mDamage);
		mDestroy = true;
	}

	if (mDestroyTime<X::GetTime())
	{
		mDestroy = true;
	}
}

void Bullet::Render()
{
	X::Math::Vector2 pos = Camera::Get().ConvertToScreenPosition(mPosition);
	X::DrawSprite(mTextureId, pos, X::Pivot::Center);
	if (X::IsKeyDown(X::Keys::K))
		X::DrawScreenCircle(Camera::Get().ConvertToScreenPosition(mCircle.center).x, Camera::Get().ConvertToScreenPosition(mCircle.center).y, mCircle.radius, X::Colors::Black);
}



