#include "Game.h"

Game::Game()
{

}

bool Game::Initialize() // SDL initialize function
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter1)", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Falied to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,	// 렌더링할 윈도우
		-1,			// 어떤 그래픽 카드를 사용할지
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC 
	);


	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ShutDown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) // queue에 이벤트가 남아있는 동안
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// 키들의 현재 상태값을 저장하는 변수
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) // ESC키를 입력하면 1을 반환. 아닐 경우 0을 반환
	{
		mIsRunning = false;
	}
}

void Game::UpdateGame()
{

}

void Game::GenerateOutput()
{

}
