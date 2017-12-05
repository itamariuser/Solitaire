#pragma once
#include "SDL.h"
#include "point.h"
#include <stdio.h>
#include "classRenderer.h"
#include "gameView.h"
#include "blorb.h"
#include <SDL_image.h>
#include <iostream>
class MainGame
{
public:
	MainGame(int screenWidth, int screenHeight) { this->screenWidth = screenWidth; this->screenHeight = screenHeight; }
	 int getScreenWidth() { return screenWidth; }
	 int getScreenHeight() { return screenHeight; }
	 int setScreenWidth(const int newWidth) { screenWidth = newWidth; }
	 int setScreenHeight(const int newHeight) { screenHeight = newHeight; }
	 void start();
	 void init_img() throw (int);
	 void init_video() throw (int);
	GameView init_window() throw (int);
private:
	 int screenWidth;
	 int screenHeight;
};