#include "MainGame.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIME_FRAME_MILLIS 1000


int main(int, char**) {
	
	MainGame mainGame(SCREEN_WIDTH, SCREEN_HEIGHT,"Solitaire by Itamar Sheffer");
	mainGame.start();
	return 0;//
}