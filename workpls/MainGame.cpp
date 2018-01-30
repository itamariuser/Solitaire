#include "MainGame.h"
#include <SDL_image.h>
#include <iostream>

void MainGame::start(bool displayOpeningScreen)
{
	init_video();
	init_img();
	init_fonts();
	GameView mainView = init_gameView();
	mainView.start(displayOpeningScreen);
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
		std::cerr << "Failed to init video\n";
	}
}

GameView MainGame::init_gameView() throw (int)
{
	if (screenWidth == -1 || screenHeight == -1)
	{
		SDL_DisplayMode dm;

		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			std::cerr<<"SDL_GetDesktopDisplayMode failed: "<<SDL_GetError()<<'\n';
		}
		screenWidth = dm.w*0.7;
		screenHeight = dm.h*0.8;
	}
	Window window(windowTitle, { 200.0f, 60.0f }, { screenWidth, screenHeight }, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	ClassRenderer renderer(window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return GameView(window, renderer);
}


void MainGame::init_fonts() throw (int)
{
	if (TTF_Init() == -1)
	{
		std::cerr << "Failed to init fonts\n";
		throw - 1;
	}
}

MainGame::~MainGame()
{
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}