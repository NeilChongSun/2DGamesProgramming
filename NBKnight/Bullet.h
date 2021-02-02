#pragma once
#include<XEngine.h>
class Bullet
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
	void SetHeading(const X::Math::Vector2& heading) { mHeading = heading; }
	X::Math::Circle GetCircle()const { return mCircle; }
	const bool Destroy()const { return mDestroy; }
protected:
	X::TextureId mTextureId = 0;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mHeading;
	X::Math::Circle mCircle;
	float mSpeedX = 200.0f;
	int mDamage = 1;
	bool mDestroy=false;
	float mDestroyTime = 4;
};
