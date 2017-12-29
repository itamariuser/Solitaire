#pragma once
#include "SDL.h"
#include "point.h"
#include <stdio.h>
#include "classRenderer.h"
#include "gameView.h"
#include "blorb.h"
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include "Window.h"
class MainGame
{
public:
	MainGame(int screenWidth, int screenHeight, std::string windowTitle = "Solitaire by Itamar Sheffer"): screenWidth(screenWidth), screenHeight(screenHeight), windowTitle(windowTitle){};
	 int getScreenWidth() { return screenWidth; }
	 int getScreenHeight() { return screenHeight; }
	 int setScreenWidth(const int newWidth) { screenWidth = newWidth; }
	 int setScreenHeight(const int newHeight) { screenHeight = newHeight; }
	 void start();
	 void init_img() throw (int);
	 void init_fonts() throw (int);
	 void init_video() throw (int);
	 GameView init_window() throw (int);

	 ~MainGame();
private:
	 int screenWidth;
	 int screenHeight;
	 std::string windowTitle;
};