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

	// create window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter1)", 
		200, 
		200, 
		static_cast<int>(mScreenWidth), 
		static_cast<int>(mScreenHeight), 
		0
	);
	if (!mWindow)
	{
		SDL_Log("Falied to create window: %s", SDL_GetError());
		return false;
	}

	// setup Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,	// 렌더링할 윈도우
		-1,			// 어떤 그래픽 카드를 사용할지
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC 
	);
	if (!mRenderer) { // 렌더러 생성 실패 체크도 추가하는 것이 좋습니다.
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// setup paddle position
	mPaddlePos.x = (mScreenWidth / 2) - (paddleWidth / 2);
	mPaddlePos.y = mScreenHeight - 30;
	mBallPos.x = (mScreenWidth / 2) - (thickness / 2);
	mBallPos.y = mScreenHeight - 100;

	// setup Bricks
	// 벽돌 1개의 너비를 계산하기 위해
	// 화면 너비 - 양 사이드 여백 - 벽돌 사이사이 여백
	float availableWidth = mScreenWidth - (sideMargin * 2) - (padding * (nCols - 1));

	// 각 벽돌의 너비를 정수타입으로 형변환
	float intBrickWidth = static_cast<int>(availableWidth / nRows);

	// 양 사이드를 제외한 모든 너비의 값(모든 벽돌 너비 + 모든 벽돌 사이 간격 너비)
	float totalContentWidth = (nRows * intBrickWidth) + ((nRows - 1) * padding);

	// totalContentWidth의 값을 mScreenWidth에서 빼면 양 사이드 너비만 남게되고
	// 해당 사이드 너비를 2로 나누면 각각 왼쪽 오른쪽 여백의 크기가 된다.
	float adjustedSideMargin = (mScreenWidth - totalContentWidth) / 2.0f;

	for (int i = 0; i < nCols; i++)
	{
		for (int j = 0; j < nRows; j++)
		{
			Brick b;

			float x = adjustedSideMargin + (j * (intBrickWidth + padding));
			float y = topMargin + (i * (brickHeight + padding));
			printf("x : %f\n", x);

			b.rect.x = static_cast<int>(x);
			b.rect.y = static_cast<int>(y);
			b.rect.w = intBrickWidth;
			b.rect.h = static_cast<int>(brickHeight);
			b.mIsActive = true;

			mBricks.push_back(b);
		}
	}

	mTickCount = SDL_GetTicks();

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

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
	{
		mPaddleDir += 1;
	}
	if (state[SDL_SCANCODE_ESCAPE]) // ESC키를 입력하면 1을 반환. 아닐 경우 0을 반환
	{
		mIsRunning = false;
	}
}

void Game::UpdateGame()
{
	float deltaTime = (SDL_GetTicks() - mTickCount) / 1000.0f;
	mTickCount = SDL_GetTicks();

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	if (mPaddleDir != 0) // 조작 입력을 받았을 때
	{
		mPaddlePos.x += mPaddleDir * mPaddleSpeed * deltaTime;
		float halfPaddle = paddleWidth / 2;

		if (mPaddlePos.x < 0)
		{
			mPaddlePos.x = 0;
		}
		else if(mPaddlePos.x > mScreenWidth - paddleWidth)
		{
			mPaddlePos.x = mScreenWidth - paddleWidth;
		}
	}

	mBallPos.x += mBallDir.x * mBallVel * deltaTime;
	mBallPos.y += mBallDir.y * mBallVel * deltaTime;

	// 벽 튕기기
	if (mBallPos.x < 0) // 왼쪽 벽과 충돌했을 때
	{
		mBallPos.x = 0;
		mBallDir.x *= -1;
	}
	if (mBallPos.x + thickness > mScreenWidth) // 오른쪽 벽과 충돌했을 때
	{
		mBallPos.x = mScreenWidth - thickness;
		mBallDir.x *= -1;
	}
	if (mBallPos.y < 0) // 위쪽 벽과 충돌했을 때
	{
		mBallPos.y = 0;
		mBallDir.y *= -1;
	}
	if (mBallPos.y + thickness > mScreenHeight) // 아래쪽 벽과 충돌했을 때
	{
		mBallPos.y = mScreenHeight - thickness;
		mBallDir.y *= -1;
	}

	// 패들 튕기기
	if((mBallPos.y + thickness) > mPaddlePos.y && mBallDir.y > 0) // 패들 Y값과 같을경우
	{
		if ((mBallPos.x + thickness > mPaddlePos.x) && (mBallPos.x < mPaddlePos.x + paddleWidth)) // 패들 x너비 안에 공의 X값이 있을 때
		{
			mBallDir.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		mRenderer,
		130,
		130,
		130,
		230
	);
	SDL_RenderClear(mRenderer);		// 위에서 얻은 색 값으로 화면 채우기

	// Paddle
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect wall{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y),
		static_cast<int>(paddleWidth),
		thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x),
		static_cast<int>(mBallPos.y),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Bricks
	SDL_SetRenderDrawColor(mRenderer, 255, 100, 100, 255);
	for (Brick& brick : mBricks)
	{
		if (brick.mIsActive)
		{
			SDL_RenderFillRect(mRenderer, &brick.rect);
		}
	}

	SDL_RenderPresent(mRenderer);	// 버퍼를 교체해서 렌더러 출력하기
}