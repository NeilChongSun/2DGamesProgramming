#pragma once
#include "Camera.h"

namespace
{
	std::unique_ptr<Camera> sInstance;
}

void Camera::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "Camera already initialized!");
	sInstance = std::make_unique<Camera>();
}

void Camera::StaticTerminate()
{
	sInstance.reset();
}

Camera & Camera::Get()
{
	XASSERT(sInstance != nullptr, "No Camera created!");
	return *sInstance;
}


X::Math::Vector2 Camera::ConvertToScreenPosition(const X::Math::Vector2 & worldPos) const
{
	return worldPos-mOffset;
}

X::Math::Vector2 Camera::ConvertToWorldPosition(const X::Math::Vector2 & screenPos) const
{	
	return screenPos + mOffset;
}

void Camera::SetViewPosition(const X::Math::Vector2 & viewPos)
{
	float screenWidth = static_cast<float>(X::GetScreenWidth());
	float screenHeight = static_cast<float>(X::GetScreenHeight());
	
	X::Math::Vector2 halfScreen = { X::GetScreenWidth()*0.5f,X::GetScreenHeight()*0.5f };
	mOffset = viewPos - halfScreen;

	if (mOffset.x < mViewBound.left)
		mOffset.x = mViewBound.left;
	if (mOffset.y < mViewBound.top)
		mOffset.y = mViewBound.top;

	if (mOffset.x > mViewBound.right-screenWidth)
		mOffset.x = mViewBound.right-screenWidth;
	if (mOffset.y > mViewBound.bottom-screenHeight)
		mOffset.y = mViewBound.bottom-screenHeight;
}

void Camera::SetViewBound(const X::Math::Rect & viewBound)
{
	mViewBound = viewBound;
	float screenWidth = static_cast<float>(X::GetScreenWidth());
	float screenHeight = static_cast<float>(X::GetScreenHeight());
	if (viewBound.right - mViewBound.left < screenWidth)
		mViewBound.right = mViewBound.left + screenWidth;
	if (viewBound.bottom - mViewBound.top < screenHeight)
		mViewBound.bottom = mViewBound.top + screenHeight;
	
}

void Camera::ClearViewBound()
{
}
