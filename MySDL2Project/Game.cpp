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

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter1)", 200, 200, mScreenWidth, mScreenHeight, 0);
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
	if (!mRenderer) { // 렌더러 생성 실패 체크도 추가하는 것이 좋습니다.
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mIsRunning = true;
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
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);
	SDL_RenderClear(mRenderer);		// 위에서 얻은 색 값으로 화면 채우기


	// Paddle
	mPaddlePos.x = (mScreenWidth / 2) - (paddleHeight / 2);
	mPaddlePos.y = mScreenHeight - 30;

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect wall{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y),
		paddleHeight,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Ball
	mBallPos.x = (mScreenWidth / 2) - (thickness / 2);
	mBallPos.y = mScreenHeight - 100;

	SDL_Rect ball{
		static_cast<int>(mBallPos.x),
		static_cast<int>(mBallPos.y),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer);	// 버퍼를 교체해서 렌더러 출력하기
}
