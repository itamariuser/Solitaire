#pragma once
#include <SDL.h>
#include "color.h"
#include <unordered_map>
#include <functional>
#include "classRenderer.h"


class GameView
{
public:
	char* title;
	int xPos;
	int yPos;
	int width;
	int height;
	Uint32 flags;
	int timeFrame;


	SDL_Window *mainWindow;
	ClassRenderer ren;

	Color backgroundColor;
	Color brushColor;

	GameView(char* ntitle, int nxPos, int nyPos, int nwidth, int nheight, Uint32 nflags, int nnewTimeFrame);

	void start();
	bool renderLineColored(const Point start, const Point end);

	Point getImageSize(char* imagePath, Uint32* format = nullptr, int* access = nullptr) const throw(int);
	Point getImageSize(SDL_Texture* image, Uint32* format = nullptr, int* access = nullptr) const throw(int);
	SDL_Texture* getTexture(char* imagePath);
	void GameView::renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect);
	void GameView::renderImage(char* const imagePath, const Shapes::Rect* const rect);//
private:
	


	bool stop;
	bool canCont;

	//ImageDisplayer* displayer;
	void init_keyBindings();
	void displayOpeningScreen();
	void handleInput();
	
	void shutdown(int retVal);
	void mainLoop();
	void inputLoop();//this is for handling input in the opening screen

	std::unordered_map<SDL_Keycode, std::function<void(void)>> map;
	bool isMouseDown;
	void handleMouseEvents(SDL_Event& e);
	void handleMouseDown(Point clickPos);
	void handleMouseUp(Point clickPos);
	void handleMouseMove(Point clickPos);


	std::vector<char*> imagePaths;
	std::unordered_map< char*, SDL_Texture*> loadedImages;
	void ctor_init_imagePaths();
	void loadImages();
};


