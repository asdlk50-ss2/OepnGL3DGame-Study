#pragma once
#include <SDL.h>
#include <iostream>

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void ShutDown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	float mScreenWidth = 1024;
	float mScreenHeight = 768;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Window* mWindow = nullptr;
	bool mIsRunning = true;

	int thickness = 15;
	Vector2 mPaddlePos{ 0.0f, 0.0f };
	float paddleHeight = 100;
	Vector2 mBallPos{ 0.0f, 0.0f };
};