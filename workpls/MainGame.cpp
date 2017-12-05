#include "MainGame.h"

void MainGame::start()
{
	init_video();
	init_img();
	GameView mainView = init_window();
	mainView.start();
	SDL_Delay(2 * 1000);
	SDL_Quit();
}

void MainGame::init_img() throw (int)
{
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
	{
		std::cerr << "Failed to init image\n";
		throw - 1;
	}
}

void MainGame::init_video() throw (int)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) //|SDL_INIT_EVENTS
	{
		std::cerr << "Failed to init video\n";//
	}
}

GameView MainGame::init_window() throw (int)
{
	return GameView("SDL Tutorial", screenWidth / 4, screenHeight / 4, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 1000);
}
