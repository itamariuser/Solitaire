#include "classRenderer.h"
#include "point.h"
#include "gameView.h"
#include "SDL_image.h"
#include <thread>
#include <algorithm>
#include <iostream>
#include "graphic.h"
	GameView::GameView(char* ntitle, int nxPos, int nyPos, int nwidth, int nheight, Uint32 nflags, int newTimeFrame) : backgroundColor(Color(0, 0, 0, 0)), brushColor(Color(0, 0, 0, 0)), 
		title(ntitle),xPos(nxPos),yPos(nyPos),width(nwidth),height(nheight),
		flags(nflags),timeFrame(newTimeFrame)
	{
		stop = true;
		canCont = true;
		init_keyBindings();
		
	}

	void GameView::init_keyBindings()
	{
		map = {
			{ SDLK_w,[&]() { SDL_SetWindowTitle(mainWindow, "LEL"); } }
		};
	}

	void GameView::displayOpeningScreen()
	{
		//Display opening screen
		Circle circle(Point(100,height/2), Point(width/50,width/50),this,20.0f);	

		int xspeed = width / 50;
		int yspeed = width / 50;
		for (int i = 0; i++ < 100;)
		{
			ren.clear();
			circle.draw();
			renderLineColored(Point(0, 0), circle.center);
			ren.present();
		}
		canCont = true;// signal to the input handler that it should exit
		SDL_Delay(1 * 1000); // Pause for 1 second
	}

	void GameView::shutdown()
	{
		SDL_Quit();
	}

	void GameView::handleInput()
	{
		SDL_Event e;
		if (SDL_PollEvent(&e) && !canCont) /* execution suspends here while waiting on an event */
		{
			if (e.type == SDL_QUIT)
				shutdown();
			else if (map.find(e.key.keysym.sym) != map.end())
				map[e.key.keysym.sym]();
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
		//tests
		SDL_Surface* s = nullptr;
		//SDL_Texture * texture = SDL_CreateTextureFromSurface(ren.renderer, s);
		//!tests
		mainWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		
		ren = ClassRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		brushColor = Color(255,165,0,255);//set brush color to ORANGE
		ren.setRenderColor(backgroundColor);

		ren.clear();
		ren.present();
		
		stop = false;
		canCont = false; //bool for the opening to signal the inputLoop to continue
		std::thread tOpening(&GameView::displayOpeningScreen, this);
		
		inputLoop();
		tOpening.join();
		
		canCont = false;
		mainLoop();

		shutdown();
	}
	void GameView::mainLoop()
	{
		SDL_Rect texr; texr.y = this->height / 2; texr.x = this->width / 2; texr.h = 200; texr.w = 200;
		SDL_Texture* image = IMG_LoadTexture(ren.renderer, "E:/Projects/workpls/workpls/workpls/assets/smiley.png");
		while (1)
		{
			SDL_RenderClear(ren.renderer);
			SDL_RenderCopy(ren.renderer, image, NULL, &texr);
			SDL_RenderPresent(ren.renderer);
		}
	}

	bool GameView::renderLineColored(const Point start, const Point end)
	{
		Color oldColor = ren.getRenderColor();
		
		ren.setRenderColor(brushColor);
		// Draw a line 
		//--- 
		int ret =
			SDL_RenderDrawLine(
				ren.renderer, // SDL_Renderer* renderer: the renderer in which draw 
				start.x,               // int x1: x of the starting point 
				start.y,          // int y1: y of the starting point 
				end.x,                 // int x2: x of the end point 
				end.y);           // int y2: y of the end point 
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