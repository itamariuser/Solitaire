#pragma once
#include <SDL.h>
#include "color.h"
#include <unordered_map>
#include <functional>
#include "classRenderer.h"
#include <SDL_ttf.h>
#include <unordered_set>
#include <memory>
#include <set>

class Text;
class Graphic;
template<> struct std::hash<Graphic>;


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
	

	//various control actions
public:
	void start();
private:
	bool stop;
	bool canCont;
	void shutdown(int retVal);
	void mainLoop();
	void inputLoop();//this is for handling input in the opening screen
	void openingScreen();
	void handleInput();
	bool loopCondition();
	void displayOpeningScreen();


	//graphic objects handling
public:
	void addTexture(std::string name, const Texture* gp, bool shouldFollowMs);
	void removeTexture(std::string name);
	void addGraphic(std::string name, const Graphic* gp);
	void removeGraphic(std::string name);
private:
	std::unordered_map<std::string, std::shared_ptr<Graphic>> objects;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	void init_objects();
	std::unordered_set<std::string> followingMouse;
	std::unordered_set<std::string> shouldFollowMouse;
	std::shared_ptr<Graphic> getObject( const std::string& name);
	void changeFollow(std::string objectName, bool shouldFollow);
	

	//simple rendering
public:
	bool renderLineColored(const Point start, const Point end);
	ClassRenderer ren;
	Color backgroundColor;
	Color brushColor;
private:




	//image, handling, loading and rendering
public:
	Point getImageSize(char* imagePath, Uint32* format = nullptr, int* access = nullptr) const;
	Point getImageSize(SDL_Texture* image, Uint32* format = nullptr, int* access = nullptr) const ;
	SDL_Texture* getTexture(char* imagePath);
	void GameView::renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect);
	void GameView::renderImage(char* const imagePath, const Shapes::Rect* const rect);

private:
	std::vector<char*> imagePaths;
	std::unordered_map< char*, SDL_Texture*> loadedImages;
	void ctor_init_imagePaths(std::string locationsFile);
	void loadImages(std::string locationsFile);


	//handling mouse events
public:
private:
	Point lastMousePos;
	bool isMouseDown;
	void handleMouseEvents(SDL_Event& e);
	void handleMouseDown(Point clickPos);
	void handleMouseUp(Point clickPos);
	void updateFollowingMouse();
	void updateLastMousePos();
	

	//handling keyboard events
public:
private:
	std::unordered_map<SDL_Keycode, std::function<void(void)>> map;
	void init_keyBindings();

	//text rendering
public:
	const TTF_Font* const getFont(const char* const filePath) { return loadedFonts[const_cast<char*>(filePath)]; }
	void renderText(Text text);
private:
	std::vector < std::pair< char*,int>> fontsAndSizes;
	std::unordered_map<char*, TTF_Font*> loadedFonts;
	void ctor_init_fontPaths();
	void loadFonts();
	

	
	
	
	
	

	
};


