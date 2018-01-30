#pragma once
#include <SDL.h>
#include "color.h"
#include <unordered_map>
#include <functional>
#include "classRenderer.h"
#include <SDL_ttf.h>
#include <unordered_set>
#include <memory>
#include "PhysicsEngine.h"
#include <queue>
#include "Window.h"
class Graphic;
class Text;
class PhysicsEngine;
class Physical;
class GameView
{
public:
	Window window;
	GameView(Window& window, ClassRenderer& renderer);



	//various control actions
public:
	void start(bool displayOpeningScreen = false);
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
	void addTexture(const std::shared_ptr<Texture> const gp, int priority = 10, bool shouldFollowMs = false);
	auto removeTexture(const std::string& name);
	void addGraphic(const std::shared_ptr<Graphic> const gp);
	auto removeGraphic(const std::string& name);
	void addPhysical(const std::shared_ptr<Physical> const gp, int priority = 10, bool shouldFollowMs = false);
	auto removePhysical(const std::string& name);
	auto getLoadedCards() const { return std::make_shared<const std::unordered_map <std::string, std::shared_ptr<SDL_Texture>>>(loadedCards); };
	Point getDefaultCardSize() const { return defaultCardSize; }
	bool isFollowingMouse(std::shared_ptr<Texture> texture);
	auto getDrawPriorities() const { return drawPriorities; }
private:
	std::unordered_map<std::string, std::shared_ptr<Graphic>> objects;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	std::unordered_map<std::string, std::shared_ptr<Physical>> physicals;
	void init_objects();
	std::unordered_set<std::string> followingMouse;
	std::unordered_set<std::string> shouldFollowMouse;//TODO: add priority rendering
	std::vector<std::shared_ptr<Graphic>> drawOrder;
	std::unordered_map<std::shared_ptr<Graphic>, int> drawPriorities;
	void sortDrawPriorities();
	std::shared_ptr<Graphic> getObject( const std::string& name);
	void changeFollow( const std::string& objectName, bool shouldFollow);
	void drawTextures();
	const Point defaultCardSize;
	int latestPriority;
	void checkCollisions();
	void handleDestroyObjects();

	//simple rendering
public:
	bool renderLineColored(const Point& start, const Point& end, int thickness = 1);
	ClassRenderer ren;
	Color backgroundColor;
	Color brushColor;
private:
	void playClickAnimation(const Point& atPoint);
	
	//image, handling, loading and rendering
public:
	std::shared_ptr<SDL_Texture> getTexture(const std::string& imagePath);
	void GameView::renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect);
	void GameView::renderImage(const std::string& imagePath, const Shapes::Rect* const rect);

private:
	std::unordered_map <std::string, std::shared_ptr<SDL_Texture>> loadedImages;
	std::unordered_map <std::string, std::shared_ptr<SDL_Texture>> loadedCards;
	void loadImages();
	void loadCards();
	void loadAssets();
	
	//handling mouse events
private:
	Point lastMousePos;
	bool isMouseDown;
	void handleMouseEvents(SDL_Event& e);
	void handleMouseDown(Point clickPos);
	void handleMouseUp(Point clickPos);
	void handleMouseMove();
	void updateFollowingMouse();
	void updateLastMousePos();
	
	//handling keyboard events
private:
	std::unordered_map<SDL_Keycode, std::function<void(void)>> map;
	void init_keyBindings();

	//text rendering
public:
	const TTF_Font* const getFont(const std::string& filePath) { return loadedFonts[filePath]; }
	void renderText(Text& text);
private:
	std::unordered_map<std::string, TTF_Font*> loadedFonts;
	void loadFonts();


	//physics handling
private:
	std::shared_ptr<PhysicsEngine> engine;


	//debug
private:
	std::shared_ptr<Text> debugText;
};


