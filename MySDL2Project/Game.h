#include <SDL.h>
#include <iostream>

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

	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	bool mIsRunning;
};