#pragma once
#include <SDL.h>
#include "color.h"
#include <unordered_map>
#include <functional>
#include "classRenderer.h"
#include <SDL_ttf.h>

class Text;

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
	GameView(char* ntitle, int nxPos, int nyPos, int nwidth, int nheight, Uint32 nflags, int nnewTimeFrame);


	//simple rendering
public:
	bool renderLineColored(const Point start, const Point end);
	ClassRenderer ren;
	Color backgroundColor;
	Color brushColor;
private:


	//various control actions
public:
	void start();
private:
	bool stop;
	bool canCont;
	void shutdown(int retVal);
	void mainLoop();
	void inputLoop();//this is for handling input in the opening screen
	void displayOpeningScreen();
	void handleInput();
	bool loopCondition();

	//image, handling, loading and rendering
public:
	Point getImageSize(char* imagePath, Uint32* format = nullptr, int* access = nullptr) const throw(int);
	Point getImageSize(SDL_Texture* image, Uint32* format = nullptr, int* access = nullptr) const throw(int);
	SDL_Texture* getTexture(char* imagePath);
	void GameView::renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect);
	void GameView::renderImage(char* const imagePath, const Shapes::Rect* const rect);

private:
	std::vector<char*> imagePaths;
	std::unordered_map< char*, SDL_Texture*> loadedImages;
	void ctor_init_imagePaths();
	void loadImages() throw (int);


	//handling mouse events
public:
private:
	bool isMouseDown;
	void handleMouseEvents(SDL_Event& e);
	void handleMouseDown(Point clickPos);
	void handleMouseUp(Point clickPos);
	void handleMouseMove(Point clickPos);


	//handling keyboard events
public:
private:
	std::unordered_map<SDL_Keycode, std::function<void(void)>> map;
	void init_keyBindings();

	//text rendering
public:
	const TTF_Font* const getFont(const char* const filePath) { return loadedFonts[const_cast<char*>(filePath)]; }
	void renderText(Text text) throw (int);
private:
	std::vector < std::pair< char*,int>> fontsAndSizes;
	std::unordered_map<char*, TTF_Font*> loadedFonts;
	void ctor_init_fontPaths();
	void loadFonts() throw (int);
	

	
	
	
	
	

	
};


