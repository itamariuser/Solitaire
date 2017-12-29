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
#include <queue>
#include "Window.h"
class Text;
class Graphic;
template<> struct std::hash<Graphic>;


class GameView
{
public:
	
	Window window;
	GameView(Window& window, ClassRenderer& renderer);
	

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
	void addTexture(std::string name, const Texture* gp, int priority = 10, bool shouldFollowMs = false);
	void removeTexture(std::string name);
	void addGraphic(std::string name, const Graphic* gp);
	void removeGraphic(std::string name);
private:
	std::unordered_map<std::string, std::shared_ptr<Graphic>> objects;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	void init_objects();
	std::unordered_set<std::string> followingMouse;
	std::unordered_set<std::string> shouldFollowMouse;//TODO: add priority rendering
	std::priority_queue <std::weak_ptr<Graphic>> renderOrder;
	std::unordered_map<std::shared_ptr<Graphic>, int> priorities;
	std::shared_ptr<Graphic> getObject( const std::string& name);
	void changeFollow(std::string objectName, bool shouldFollow);
	void drawTextures();

	//simple rendering
public:
	bool renderLineColored(const Point start, const Point end);
	ClassRenderer ren;
	Color backgroundColor;
	Color brushColor;
private:




	//image, handling, loading and rendering
public:
	
	std::shared_ptr<SDL_Texture> getTexture(char* imagePath);
	void GameView::renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect);
	void GameView::renderImage(char* const imagePath, const Shapes::Rect* const rect);

private:
	std::vector<char*> imagePaths;
	std::unordered_map < std::string, std::shared_ptr<SDL_Texture> > loadedImages;
	void loadTextures();
	


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
	std::unordered_map<std::string, TTF_Font*> loadedFonts;
	void loadFonts();
	

};


