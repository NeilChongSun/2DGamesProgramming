#include "Link.h"
#include"Camera.h"
#include"TileMap.h"
TileMap map;
void Link::Load()
{
	mTextureId = X::LoadTexture("crosshair_handgun.png");
	mPosition = { 10.0f,10.0f };
}

void Link::Unload()
{
}

void Link::Update(float deltaTime)
{
	const float moveSpeed = 450.0f;
	//Check input and record the amount of displacement
	X::Math::Vector2 displacement;
	if (X::IsKeyDown(X::Keys::D))
		displacement.x = moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::A))
		displacement.x = -moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::W))
		displacement.y = -moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::S))
		displacement.y = moveSpeed * deltaTime;
	//Project the bounding box to the future position and check the corresponding edge for collision
	auto currentBox = GetBoundingBox();
	/*if (displacement.x>0.0f)
	{
		X::Math::LineSegment rightEdge{
			currentBox.max.x+displacement.x,
			currentBox.min.y,
			currentBox.max.x+displacement.x,
			currentBox.max.y
		};
		if (TileMap::Get().CheckCollision(rightEdge))
		{
			displacement.x=0;
		}
	}

	if (displacement.x < 0.0f)
	{
		X::Math::LineSegment leftEdge{
			currentBox.min.x + displacement.x,
			currentBox.min.y,
			currentBox.min.x + displacement.x,
			currentBox.max.y
		};
		if (TileMap::Get().CheckCollision(leftEdge))
		{
			displacement.x = 0;
		}
	}

	if (displacement.y < 0.0f)
	{
		X::Math::LineSegment TopEdge{
			currentBox.min.x,
			currentBox.min.y+displacement.y,
			currentBox.max.x,
			currentBox.min.y+displacement.y
		};
		if (TileMap::Get().CheckCollision(TopEdge))
		{
			displacement.y = 0;
		}
	}

	if (displacement.y > 0.0f)
	{
		X::Math::LineSegment BottomEdge{
			currentBox.min.x,
			currentBox.max.y + displacement.y,
			currentBox.max.x,
			currentBox.max.y + displacement.y
		};
		if (TileMap::Get().CheckCollision(BottomEdge))
		{
			displacement.y = 0;
		}
	}*/

	//Apply dispaacement and update our position
	mPosition += displacement;
}

void Link::Render()
{
	X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(mPosition);
	X::DrawSprite(mTextureId, screenPos);
}

X::Math::Rect Link::GetBoundingBox() const
{
	return{
		mPosition.x - 12.0f,
		mPosition.y - 12.0f,
		mPosition.x + 12.0f,
		mPosition.y + 12.0f,
	};
}
