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


void GameView::mainLoop()
{
	auto helloText1 = getObject("helloText1");
	auto kingLeaf1 = getObject("kingLeaf1");

	while (loopCondition())
	{
		ren.clear();
		handleInput();

		helloText1->setColor(helloText1->getColor() + Color{ 1, 3, 6, 0 });
		helloText1->draw();
		kingLeaf1->draw();


		ren.present();


	}
}

void GameView::ctor_init_fontPaths()
{
	fontsAndSizes.push_back({ "assets/arial.ttf", 150 });
}

void GameView::init_objects()
{
	addTexture("kingLeaf1", new Card({ 1,1 }, { 4,4 }, this, getTexture("assets/KingLeaf.png"), { 1,1 }, -1, -1, "kingLeaf1"),true);
	//followingMouse.insert("kingLeaf1");
	addTexture("helloText1", new Text({ width - 160,1 }, { -6,2 }, this, "assets/arial.ttf", { 255,0,0,255 }, { 2,2 }, "Solitaire!", 160, 100, "helloText1"),false);


}


GameView::GameView(char* ntitle, int nxPos, int nyPos, int nwidth, int nheight, Uint32 nflags, int newTimeFrame) : backgroundColor(Color(0, 0, 0, 0)), brushColor(Color(0, 0, 0, 0)), 
		title(ntitle),xPos(nxPos),yPos(nyPos),width(nwidth),height(nheight),
		flags(nflags),timeFrame(newTimeFrame), isMouseDown(false), lastMousePos{0,0},
		mainWindow(SDL_CreateWindow(title, xPos, yPos, width, height, flags)),
		ren(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
	{
		stop = true;
		canCont = true;
		loadImages();
		loadFonts();
		init_keyBindings();
		init_objects();
		
	}

	void GameView::init_keyBindings()
	{
		map = {
			{ SDLK_w,[&]() { SDL_SetWindowTitle(mainWindow, "LEL"); } }
		};
	}
	
	void GameView::changeFollow(std::string objectName, bool shouldFollow)
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


	void GameView::addGraphic(std::string name, const Graphic* gp)
	{
		std::shared_ptr<Graphic> shared;
		shared.reset(const_cast<Graphic*>(gp));
		objects[name] = shared;
	}

	void GameView::removeGraphic(std::string name)
	{
		objects.erase(name);
	}

	void GameView::removeTexture(std::string name)
	{
		textures.erase(name);
		removeGraphic(name);
	}

	void GameView::addTexture(std::string name, const Texture* gp, bool shouldFollowMs = false)
	{
		std::shared_ptr<Texture> shared;
		shared.reset(const_cast<Texture*>(gp));
		textures[name] = shared;
		if (shouldFollowMs) shouldFollowMouse.insert(shared->getName());
		addGraphic(name, gp);
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
		Circle circle(Point(100,height/2), Point(width/50,width/50),this,20);	//

		int xspeed = width / 50;
		int yspeed = width / 50;
		for (int i = 0; i++ < 100;)
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
		if (SDL_PollEvent(&e) && !canCont) /* execution suspends here while waiting on an event */
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
		
		//lastMousePos = clickPos;
		if (e.type == SDL_MOUSEBUTTONDOWN)
			handleMouseDown(clickPos);

		else if (e.type == SDL_MOUSEBUTTONUP)
			handleMouseUp(clickPos);
		
		
		
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

	//void GameView::addTexture()

	void GameView::handleMouseUp(Point clickPos)
	{
		//TODO: call appropriate object's button up function
		isMouseDown = false;
		//for (auto name : followingMouse)
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

	void GameView::updateFollowingMouse()
	{
		//TODO: call appropriate object's mouse move function
		if(isMouseDown)
			for (auto itr = followingMouse.begin(); itr != followingMouse.end(); ++itr)
			{
				auto objectName = *itr;
				if (shouldFollowMouse.find(objectName) == shouldFollowMouse.end())
				{
					if((itr = followingMouse.erase(itr)) == followingMouse.end()) break;
					continue;
				}
				objects[objectName]->setCenter(lastMousePos);
			}
	}


	void GameView::inputLoop()
	{
		while (!canCont && !stop)
		{
			handleInput();
		}
	}

	
	void GameView::start()
	{
		stop = false;

		//displayOpeningScreen();
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
	

	bool GameView::loopCondition()
	{
		static int i = 0;
		return true;//i++ < 500;
	}

	
	void GameView::renderText(Text text) throw (int)
	{
		SDL_RenderCopy(ren.renderer, const_cast<SDL_Texture*>(text.getTexture()), nullptr, new SDL_Rect(text.getRenderRect()));
	}

	bool GameView::renderLineColored(const Point start, const Point end)
	{
		Color oldColor = ren.getRenderColor();
		
		ren.setRenderColor(brushColor);
		// Draw a line 
		//--- 
		int ret =
			SDL_RenderDrawLine(
				ren.renderer,
				start.x,     
				start.y,     
				end.x,       
				end.y);      
		if (ret != 0)
		{
			const char *error = SDL_GetError();
			if (*error != '\0')
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not renderDrawLineColored. SDL Error: %s at line #%d of file %s/n", error, __LINE__, __FILE__);
				SDL_ClearError();
			}
			ren.setRenderColor(oldColor);
			return false;
		}
		ren.setRenderColor(oldColor);
		return true;
	}

	void GameView::ctor_init_imagePaths()
	{
		imagePaths.push_back("assets/KingLeaf.png");
	}

	void GameView::loadImages() throw (int)
	{
		ctor_init_imagePaths();
		for (char* path : imagePaths)
		{
			auto z = IMG_LoadTexture(ren.renderer, path);
			loadedImages[path] = IMG_LoadTexture(ren.renderer, path);
		}
	}

	

	void GameView::loadFonts() throw (int)
	{

		ctor_init_fontPaths();
		for (auto pair : fontsAndSizes)
			loadedFonts[pair.first] = TTF_OpenFont(pair.first, pair.second);
	}

	

	Point GameView::getImageSize(char* imagePath, Uint32* format, int* access) const throw(int)
	{
		if (loadedImages.find(imagePath) == loadedImages.end())
		{
			std::cerr << "No such image loaded, add it to imagePaths.\n";
			throw(-1);
		}
		Point retPt;
		SDL_QueryTexture(loadedImages.at(imagePath), format, access, &retPt.x, &retPt.y);
		return retPt;
	}

	Point GameView::getImageSize(SDL_Texture* image, Uint32* format, int* access) const throw(int)
	{
		if (!image)
		{
			std::cerr << "No image to load.\n";
			throw(-1);
		}
		Point retPt;
		SDL_QueryTexture(image, format, access, &retPt.x, &retPt.y);
		return retPt;
	}

	SDL_Texture* GameView::getTexture(char* imagePath)
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
		SDL_RenderCopy(ren.renderer, const_cast<SDL_Texture*>(texture), nullptr, &SDL_Rect(*rect));
	}

	void GameView::renderImage(char* const imagePath, const Shapes::Rect* const rect)
	{
		auto texture = getTexture(imagePath);
		renderImage(texture, rect);
	}
