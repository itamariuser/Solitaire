#include "SDL.h"
#include "point.h"
#include <stdio.h>
#include "classRenderer.h"
#include "gameView.h"
#include "blorb.h"
#include <SDL_image.h>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIME_FRAME_MILLIS 1000

void init_img() throw (int);
void init_video() throw (int);
GameView init_window() throw (int);
int main(int, char**) {
	init_video();
	init_img();
	GameView mainView = init_window();
	mainView.start();
	SDL_Delay(2 * 1000);
	SDL_Quit();
	return 0;
}

GameView init_window() throw (int)
{
	return GameView("SDL Tutorial", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 1000);
}


void init_video() throw (int)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) //|SDL_INIT_EVENTS
	{
		std::cerr << "Failed to init video\n";//
	}
}

void init_img() throw (int)
{
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
	{
		std::cerr << "Failed to init image\n";
		throw - 1;
	}
}


