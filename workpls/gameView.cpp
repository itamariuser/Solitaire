#include "classRenderer.h"
#include "point.h"
#include "gameView.h"
#include "SDL_image.h"
#include <thread>
#include "graphic.h"
#include "Shapes.h"
#include <iostream>
#include <filesystem>
#include <vcruntime_exception.h>
#include "ResourceLoader.h"


void GameView::mainLoop()
{

	while (loopCondition())
	{
		ren.clear();
		handleInput();
		sortDrawPriorities();
		drawTextures();
		ren.present();
	}
}

void GameView::loadFonts()
{
	ResourceLoader::load<TTF_Font>("./assets/fonts", loadedFonts, [&](std::string file) { return TTF_OpenFont(file.c_str(), 150); });
}

void GameView::loadTextures()
{
	ResourceLoader::load<SDL_Texture>("./assets/images", loadedImages, [&](std::string file) { return IMG_LoadTexture(ren.renderer, file.c_str()); });
}

void GameView::init_objects()
{
	addTexture("kingLeaf1", new Card({ 1,1 }, { 0,0 }, this, getTexture("KingLeaf.png").get(), { 0,0 }, 135,178, "kingLeaf1"),10,true);
	addTexture("debugText", new Text({ 100,100 }, { 0,0 }, this, "arial.ttf", { 0,200,0,0 }, { 0,0 }, "DEBUG TEXT", 160, 100, "debugText"),0);
	//addTexture("helloText1", new ColorSwitchText(Text({ window.getDimensions().x - 160,1 }, { -6,2 }, this, "arial.ttf", { 255,0,0,255 }, { 2,2 }, "Solitaire!", 160, 100, "helloText1"), { 1, 3, 6, 0 }),11, false);

}

void GameView::sortDrawPriorities()
{
	std::sort(drawOrder.begin(), drawOrder.end(), [&](auto texture1, auto texture2) {return drawPriorities[texture1] > drawPriorities[texture2]; });
}

void GameView::drawTextures()
{
	for (auto texture : drawOrder)
	{
		if(texture->isVisible())
			texture->draw();
	}
}


GameView::GameView(Window& window, ClassRenderer& renderer) :
	backgroundColor(0, 0, 0, 0), brushColor(0, 0, 0, 0), 
	isMouseDown(false), lastMousePos(0,0), stop(true), canCont(true),
	window(window), ren(renderer)
	{
	
		loadTextures();
		loadFonts();
		init_keyBindings();
		init_objects();
	}

	void GameView::init_keyBindings()
	{
		map = 
		{
			{ SDLK_w,[&]() { window.setTitle(" 'W' key pressed!"); } },
		};
	}
	
	void GameView::changeFollow(const std::string& objectName, bool shouldFollow)
	{
		if (shouldFollow)
		{
			shouldFollowMouse.insert(objectName);
		}
		else
		{
			shouldFollowMouse.erase(objectName);
		}
	}


	void GameView::addGraphic(const std::string& name, const Graphic* gp)
	{
		std::shared_ptr<Graphic> shared;
		shared.reset(const_cast<Graphic*>(gp));
		objects[name] = shared;
	}

	void GameView::removeGraphic(const std::string& name)
	{
		objects.erase(name);
	}

	void GameView::removeTexture(const std::string& name)
	{
		auto itr = std::find(drawOrder.begin(), drawOrder.end(), getObject(name));
		drawOrder.erase(itr);
		drawPriorities.erase(getObject(name));

		textures.erase(name);
		removeGraphic(name);
	}

	void GameView::addTexture(const std::string& name, const Texture* gp, int priority, bool shouldFollowMs)
	{
		std::shared_ptr<Texture> shared;
		shared.reset(const_cast<Texture*>(gp));
		textures[name] = shared;
		if (shouldFollowMs) shouldFollowMouse.insert(shared->getName());
		addGraphic(name, gp);
		drawPriorities[shared] = priority;
		drawOrder.push_back(shared);
	}

	void GameView::displayOpeningScreen()
	{
		std::thread tOpening(&GameView::openingScreen, this);
		inputLoop();
		tOpening.join();
	}


	void GameView::openingScreen()
	{
		canCont = false; //bool for the opening to signal the inputLoop to continue
		//Display opening screen
		Circle circle(Point(100, window.getDimensions().y / 2), Point(window.getDimensions().x / 50, window.getDimensions().y / 50),this,20);	//

		for (int i = 0; i++ < 120;)
		{
			ren.clear();
			circle.draw();
			renderLineColored(Point(0, 0), circle.getCenter());
			ren.present();
		}
		canCont = true;// signal to the input handler that it should exit
		SDL_Delay(1 * 1000); // Pause for 1 second
	}

	void GameView::shutdown(int retVal)
	{
		SDL_Quit();
		exit(retVal);
	}
	
	void GameView::updateLastMousePos()
	{
		SDL_GetMouseState(&lastMousePos.x, &lastMousePos.y);
	}

	void GameView::handleInput()
	{
		updateLastMousePos();
		SDL_Event e;
		if (SDL_PollEvent(&e) && !canCont) // execution suspends here while waiting on an event
		{
			if (e.type == SDL_QUIT)
				shutdown(0);
			else if (map.find(e.key.keysym.sym) != map.end())
				map[e.key.keysym.sym]();
			
			handleMouseEvents(e);

		}
		if (!lastMousePos.isZero() && isMouseDown)
			updateFollowingMouse();
		
	}
	void GameView::handleMouseEvents(SDL_Event& e)
	{
		Point clickPos(0, 0);
		
		if (e.type == SDL_MOUSEBUTTONDOWN)
			handleMouseDown(clickPos);
		else if (e.type == SDL_MOUSEBUTTONUP)
			handleMouseUp(clickPos);
		else if (e.type == SDL_MOUSEMOTION)
			handleMouseMove();
	}

	void GameView::handleMouseDown(Point clickPos)
	{
		//TODO: call appropriate object's button down function
		isMouseDown = true;
		for (auto pair : textures)
		{
			if (pair.second->getRenderRect().contains(lastMousePos))
			{
				followingMouse.insert(pair.first);
			}
		}
	}

	void GameView::handleMouseUp(Point clickPos)
	{
		//TODO: call appropriate object's button up function
		isMouseDown = false;
		for(auto itr = followingMouse.begin(); itr != followingMouse.end(); ++itr)
		{
			auto name = *itr;
			if (textures[name]->getRenderRect().contains(lastMousePos))
			{
				if ((itr = followingMouse.erase(itr)) == followingMouse.end())
					break;
			}
		}
	}

	void GameView::handleMouseMove()
	{
		std::string info = "X: " + std::to_string(lastMousePos.x) + ", Y: " + std::to_string(lastMousePos.y);
		std::dynamic_pointer_cast<Text>(getObject("debugText"))->setText(info);
	}

	void GameView::updateFollowingMouse()
	{
		//TODO: call appropriate object's mouse move function
		if (isMouseDown)
		{
			for (auto itr = followingMouse.begin(); itr != followingMouse.end(); ++itr)
			{
				auto objectName = *itr;
				if (shouldFollowMouse.find(objectName) == shouldFollowMouse.end())
				{
					if ((itr = followingMouse.erase(itr)) == followingMouse.end()) break;
					continue;
				}
				objects[objectName]->setCenter(lastMousePos);
			}
		}
	}


	void GameView::inputLoop()
	{
		while (!canCont && !stop)
		{
			handleInput();
		}
	}

	
	void GameView::start(bool displayOpeningScreen)
	{
		stop = false;
		if(displayOpeningScreen)
			this->displayOpeningScreen();
		canCont = false;
		mainLoop();

		shutdown(0);
	}

	std::shared_ptr<Graphic> GameView::getObject(const std::string& name)
	{
		auto shared = objects[name];
		if (!shared.get())
			throw std::exception("No Graphic found\n");
		return shared;
	}
	

	inline bool GameView::loopCondition()
	{
		static int i = 0;
		return true;//i++ < 500;
	}

	
	void GameView::renderText(Text& text)
	{
		auto oldColor = ren.getRenderColor();
		ren.setRenderColor(text.getColor());
		SDL_RenderCopy(ren.renderer, const_cast<SDL_Texture*>(text.getTexture()), nullptr, new SDL_Rect(text.getRenderRect()));
		ren.setRenderColor(oldColor);
	}

	bool GameView::renderLineColored(const Point& start, const Point& end)
	{
		return ren.renderLineColored(start, end, brushColor);
	}

	std::shared_ptr<SDL_Texture> GameView::getTexture(const std::string& imagePath)
	{
		if (loadedImages.find(imagePath) == loadedImages.end())
		{
			std::cerr << "No such image loaded, add it to imagePaths.\n";
			throw(-1);
		}
		return loadedImages[imagePath];
	}

	void GameView::renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect)
	{
		ren.renderImage(texture, rect);
	}

	void GameView::renderImage(const std::string& imagePath, const Shapes::Rect* const rect)
	{
		auto texture = getTexture(imagePath);
		renderImage(texture.get(), rect);
	}