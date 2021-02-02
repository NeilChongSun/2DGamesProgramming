#pragma once
#include<XEngine.h>
class Camera
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static Camera& Get();

	X::Math::Vector2 ConvertToScreenPosition(const X::Math::Vector2& worldPos) const;
	X::Math::Vector2 ConvertToWorldPosition(const X::Math::Vector2& screenPos) const;
	void SetViewPosition(const X::Math::Vector2& viewPos);

	void SetViewBound(const X::Math::Rect& viewBound);
	void ClearViewBound();
private:
	X::Math::Vector2 mOffset;
	X::Math::Rect mViewBound{ -FLT_MAX,-FLT_MAX,FLT_MAX,FLT_MAX };
};
