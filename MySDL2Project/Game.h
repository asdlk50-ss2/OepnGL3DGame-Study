#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

struct Vector2
{
	float x;
	float y;
};

struct Brick
{
public:
	SDL_Rect rect;
	bool mIsActive;
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

	// System
	Uint32 mTickCount;

	// Window and Renderer
	float mScreenWidth = 1024;
	float mScreenHeight = 768;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Window* mWindow = nullptr;
	bool mIsRunning = true;

	// Paddle and Ball
	int thickness = 15;

	Vector2 mPaddlePos{ 0.0f, 0.0f };
	float paddleWidth = 100;
	float mPaddleSpeed = 300;
	int mPaddleDir = 0;

	Vector2 mBallPos{ 0.0f, 0.0f };
	Vector2 mBallDir{ 0.3f, 1.0f };
	float mBallVel = 250;

	// Brick
	std::vector<Brick> mBricks;
	const int nRows = 10;
	const int nCols = 5;
	const float padding = 10.f;
	const float sideMargin = 60.f;
	const float topMargin = 50.f;
	const float brickHeight = 30.f;
};