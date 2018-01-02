#pragma once
#include "gameView.h"

class MainGame
{
public:
	MainGame(int screenWidth, int screenHeight, std::string windowTitle = "Solitaire by Itamar Sheffer"): screenWidth(screenWidth), screenHeight(screenHeight), windowTitle(windowTitle){};
	 int getScreenWidth() const { return screenWidth; }
	 int getScreenHeight() const { return screenHeight; }
	 void setScreenWidth(int newWidth) { screenWidth = newWidth; }
	 void setScreenHeight(int newHeight) { screenHeight = newHeight; }
	 void start(bool displayOpeningScreen = false);
	 void init_img() throw (int);
	 void init_fonts() throw (int);
	 void init_video() throw (int);
	 GameView init_gameView() throw (int);

	 ~MainGame();
private:
	 int screenWidth;
	 int screenHeight;
	 std::string windowTitle;
};