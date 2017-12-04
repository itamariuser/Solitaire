#include "classRenderer.h"
#include "point.h"
#include "gameView.h"
#include "SDL_image.h"
#include <thread>
#include "graphic.h"
#include "Shapes.h"

GameView::GameView(char* ntitle, int nxPos, int nyPos, int nwidth, int nheight, Uint32 nflags, int newTimeFrame) : backgroundColor(Color(0, 0, 0, 0)), brushColor(Color(0, 0, 0, 0)), 
		title(ntitle),xPos(nxPos),yPos(nyPos),width(nwidth),height(nheight),
		flags(nflags),timeFrame(newTimeFrame), isMouseDown(false),
		mainWindow(SDL_CreateWindow(title, xPos, yPos, width, height, flags)),
		ren(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
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
		Graphic::Circle circle(Point(100,height/2), Point(width/50,width/50),this,20.0f);	

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

	void GameView::shutdown(int retVal)
	{
		SDL_Quit();
		exit(retVal);
	}
	
	void GameView::handleInput()
	{
		SDL_Event e;
		if (SDL_PollEvent(&e) && !canCont) /* execution suspends here while waiting on an event */
		{
			if (e.type == SDL_QUIT)
				shutdown(0);
			else if (map.find(e.key.keysym.sym) != map.end())
				map[e.key.keysym.sym]();
			handleMouseEvents(e);
		}
		
	}
	void GameView::handleMouseEvents(SDL_Event& e)
	{
		Point clickPos(0, 0);
		SDL_GetMouseState(&clickPos.x, &clickPos.y);

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			//TODO: call appropriate object's button up function
			handleMouseDown(clickPos);
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			//TODO: call appropriate object's button down function
			handleMouseUp(clickPos);
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			//TODO: call appropriate object's button up function
			handleMouseMove(clickPos);
		}
	}

	void GameView::handleMouseDown(Point clickPos)
	{

	}

	void GameView::handleMouseUp(Point clickPos)
	{

	}

	void GameView::handleMouseMove(Point clickPos)
	{

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

		shutdown(0);
	}
	void GameView::mainLoop()
	{
		//SDL_Rect texr; texr.y = this->height / 2; texr.x = this->width / 2; texr.h = 200; texr.w = 200;
		Graphic::Card kingLeaf1({ 0,0 }, { 0,0 }, this, ren.getTexture("assets/KingLeaf.png"));
		
		int i = 0;
		while (i++<500)
		{
			SDL_RenderClear(ren.renderer);
			/*auto p = ren.getImageSize("assets/KingLeaf.png");
			SDL_Rect texr = Shapes::Rect(0, 0, p.x, p.y);
			SDL_RenderCopy(ren.renderer, ren.getTexture("assets/KingLeaf.png"), nullptr, &texr);*/
			kingLeaf1.draw();
			ren.present();
			handleInput();
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