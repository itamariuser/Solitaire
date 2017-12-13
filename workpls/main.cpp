#include "MainGame.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define TIME_FRAME_MILLIS 1000


int main(int, char**) {
	
	MainGame mainGame(-1, -1,"Solitaire by Itamar Sheffer");
	mainGame.start();
	return 0;
}