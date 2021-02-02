#include <XEngine.h>

X::TextureId textureIdA;
X::Math::Vector2 positionA;
X::Math::Vector2 velocityA;

X::TextureId textureIdB;
X::Math::Vector2 positionB;
X::Math::Vector2 velocityB;

void GameInit()
{
	textureIdA = X::LoadTexture("mario.png");
	positionA = {200.0f, 200.0f};
	velocityA = { 300.0f,200.0f };

	textureIdB = X::LoadTexture("crosshair.png");
	positionB = { 500.0f,500.0f };
	velocityB = { -200.0f,-300.0f };
}

void GameCleanUp()
{

}

bool GameLoop(float deltaTime)
{
	//const float moveSpeed = 300.0f;

	/*if (X::IsKeyDown(X::Keys::D))
		position.x +=moveSpeed*deltaTime;
	if (X::IsKeyDown(X::Keys::A))
		position.x -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::W))
		position.y -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::S))
		position.y += moveSpeed * deltaTime;*/

	positionA += velocityA * deltaTime;
	positionB += velocityB * deltaTime;
	
	const float halfHeightA = X::GetSpriteHeight(textureIdA)*0.5f;
	const float halfWidthA = X::GetSpriteWidth(textureIdA)*0.5f;

	const float halfHeightB = X::GetSpriteHeight(textureIdB)*0.5f;
	const float halfWidthB = X::GetSpriteWidth(textureIdB)*0.5f;

	uint32_t screenW = X::GetScreenWidth();
	uint32_t screenH = X::GetScreenHeight();

	/*if (position.x>screenW-halfWidth)
		position.x = screenW - halfWidth;
	if (position.x < halfWidth)
		position.x = halfWidth;

	if (position.y > screenH - halfHeight)
		position.y = screenH - halfHeight;
	if (position.y < halfHeight)
		position.y = halfHeight;*/

	if ((positionA.x > screenW - halfWidthA)||( positionA.x < halfWidthA))
		velocityA.x = -velocityA.x;
	if ((positionA.y > screenH - halfHeightA) || (positionA.y < halfHeightA))
		velocityA.y = -velocityA.y;

	if ((positionB.x > screenW - halfWidthB) || (positionB.x < halfWidthB))
		velocityB.x = -velocityB.x;
	if ((positionB.y > screenH - halfHeightB) || (positionB.y < halfHeightB))
		velocityB.y = -velocityB.y;


	X::Math::Vector2 distance = positionB - positionA;
	if (distance.x<0)
	{
		distance.x = -distance.x;
	}
	if (distance.y<0)
	{
		distance.y = -distance.y;
	}
 
	if ((distance.x <= halfWidthA + halfWidthB) && (distance.y <= halfHeightA + halfHeightB))
	{	
		velocityA = -velocityA;
		velocityB = -velocityB;
	}

	
	//X::GetScreenWidth();
	//Draw a circle on the center of picture
	//X::DrawScreenCircle(position, 5.0f, X::Colors::Cyan);


	X::DrawSprite(textureIdA, positionA);
	X::DrawSprite(textureIdB, positionB);
	
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();
	//() - function invocation operator
	X::Run(GameLoop);
	GameCleanUp();
	X::Stop();
	return 0;
}