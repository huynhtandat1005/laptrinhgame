#include "Game.h"

int main(int argc, char *argv[])
{
	Game *game = new Game(); 
	game->init("TINY FOOTBALL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	game->showMenu();

	while (game->running())
	{
		Uint32 frameStart = SDL_GetTicks(); 
		
		game->handleEvents();
		game->update();
		game->render();

		if (game->goReset())
		{
			SDL_Delay(2000);	
			game->reset();
		}

		if (game->isGameOver())
		{ 
			SDL_Delay(2000);	
			game->showWinner();
		}

		if (game->isGoingBack())
		{
			game->showMenu();
		}

		const int frameTime = SDL_GetTicks() - frameStart; 
		if (FRAME_DELAY > frameTime)
		{
			SDL_Delay(FRAME_DELAY - frameTime);
		}
	}
	game->clean();
	delete game;
	return 0;
}