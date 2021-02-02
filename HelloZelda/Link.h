#pragma once
#include<XEngine.h>

class Link
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();

	const X::Math::Vector2& GetPosition() const { return mPosition; }
	void SetPosition(X::Math::Vector2 pos) {  mPosition=pos; }

	X::Math::Rect GetBoundingBox() const;
private:
	X::TextureId mTextureId;
	X::Math::Vector2 mPosition;
};