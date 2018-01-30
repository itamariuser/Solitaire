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
#include "Physical.h"
void GameView::mainLoop()
{
	//TODO: MINESWEEPER
	//init_objects();
	while (loopCondition())
	{
		handleDestroyObjects();
		ren.clear();
		handleInput();
		checkCollisions();
		sortDrawPriorities();
		drawTextures();
		ren.present();
	}
}

void GameView::loadFonts()
{
	ResourceLoader::load<TTF_Font>("./assets/fonts", loadedFonts, [&](std::string file) { return TTF_OpenFont(file.c_str(), 150); });
}

void GameView::loadCards()
{
	ResourceLoader::load<SDL_Texture>("./assets/cards", loadedCards, [&](std::string file) { return IMG_LoadTexture(ren.renderer, file.c_str()); });
}

void GameView::loadImages()
{
	ResourceLoader::load<SDL_Texture>("./assets/images", loadedImages, [&](std::string file) { return IMG_LoadTexture(ren.renderer, file.c_str()); });
}

void GameView::loadAssets()
{
	loadCards();
	loadedImages = loadedCards;
	loadImages();
	loadFonts();
}

void GameView::init_objects()
{
	Physical p0("circle0",{100,100},{1,0},this,getTexture("king_of_clubs.png").get(),{}, getDefaultCardSize().x, getDefaultCardSize().y,1);
	Physical p1("circle1", { 499,100 }, { -1,0 }, this, getTexture("king_of_clubs.png").get(), {}, getDefaultCardSize().x, getDefaultCardSize().y);
	addPhysical(std::make_shared<Physical>(p0),10,true);
	addPhysical(std::make_shared<Physical>(p1),10,true);
	debugText = std::make_shared<Text>(Text("debugText", { 700,100 }, { 0,0 }, this, "arial.ttf", { 0,200,0,0 }, { 0,0 }, "DEBUG TEXT", 160, 100));
	addTexture(debugText, 0);

}


void GameView::sortDrawPriorities()
{
	std::sort(drawOrder.begin(), drawOrder.end(), [&](auto texture1, auto texture2) {return drawPriorities[texture1] > drawPriorities[texture2]; });
}

void GameView::drawTextures()
{
	for (auto texture : drawOrder)
	{
		if (texture->isVisible())
			texture->draw();
	}
}



GameView::GameView(Window& window, ClassRenderer& renderer) :
	backgroundColor(0, 0, 0, 0), brushColor(0, 0, 0, 0),
	isMouseDown(false), lastMousePos(0, 0), stop(true), canCont(true),
	window(window), ren(renderer), defaultCardSize(135, 178), latestPriority(0)
{

	loadAssets();
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

void GameView::addGraphic(const std::shared_ptr<Graphic> const gp)
{
	objects[gp->name] = gp;
}

void GameView::addTexture(const std::shared_ptr<Texture> const gp, int priority, bool shouldFollowMs)
{
	textures[gp->name] = gp;
	if (shouldFollowMs) shouldFollowMouse.insert(gp->getName());
	addGraphic(gp);
	drawPriorities[gp] = priority;
	drawOrder.push_back(gp);
}

auto GameView::removeGraphic(const std::string& name)
{
	auto itr = objects.find(name);
	return objects.erase(itr);
}

auto GameView::removeTexture(const std::string& name)
{
	auto textr = textures[name];
	auto itr = std::find(drawOrder.begin(), drawOrder.end(), textr);
	drawOrder.erase(itr);
	drawPriorities.erase(textr);
	auto textrItr = textures.find(name);
	
	
	return textures.erase(textrItr);
}

void GameView::addPhysical(const std::shared_ptr<Physical> const gp, int priority, bool shouldFollowMs)
{
	physicals[gp->name] = gp;
	addTexture(gp,priority,shouldFollowMs);
}

auto GameView::removePhysical(const std::string& name)
{
	auto itr = physicals.find(name);
	removeTexture(name);
	return physicals.erase(itr);
}

void GameView::handleDestroyObjects()
{
	for (auto itr = objects.begin(); itr != objects.end();)
	{
		auto graphic = *itr;
		auto name = graphic.first;
		if (graphic.second->getShouldBeDestroyed())
		{
			itr = removeGraphic(name);
			removeTexture(name);
			graphic.second.reset();
			continue;
		}
		else
			++itr;
	}
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
	Circle circle("Circle",Point(100, window.getDimensions().y / 2), Point(window.getDimensions().x / 50, window.getDimensions().y / 50), this, 20);	//

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
	auto *x = new int(-1), *y = new int(-1);
	SDL_GetMouseState(x, y);
	if(*y >= -1 && *x >= -1)
		lastMousePos = Point(*x, *y);
		
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
	////TODO: call appropriate object's button down function
	//isMouseDown = true;
	//int bestPriority = INT_MAX;
	//std::string bestPriorityName;
	//for (auto pair : textures)
	//{
	//	auto sec = *(pair.second);
	//	if (pair.second->getRenderRect().contains(lastMousePos) && drawPriorities[pair.second] <= bestPriority)
	//	{
	//		//followingMouse.insert(pair.first);
	//		bestPriority = drawPriorities[pair.second];
	//		bestPriorityName = pair.first;
	//	}
	//}
	//followingMouse.insert(bestPriorityName);

	//TODO: call appropriate object's button down function
	isMouseDown = true;
	int bestPriority = INT_MAX;
	std::string bestPriorityName;
	for (auto pair : textures)
	{
		auto sec = *(pair.second);
		if (pair.second->getRenderRect().contains(lastMousePos))// && drawPriorities[pair.second] <= bestPriority)
		{
			followingMouse.insert(pair.first);
			/*bestPriority = drawPriorities[pair.second];
			bestPriorityName = pair.first;*/
		}
	}
	playClickAnimation(lastMousePos);
	//followingMouse.insert(bestPriorityName);
}

void GameView::handleMouseUp(Point clickPos)
{
	//TODO: call appropriate object's button up function
	isMouseDown = false;
	for (auto itr = followingMouse.begin(); itr != followingMouse.end(); ++itr)
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
	std::string info = "X: " + std::to_string(int(lastMousePos.x)) + ", Y: " + std::to_string(int(lastMousePos.y));
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

bool GameView::isFollowingMouse(std::shared_ptr<Texture> texture)
{
	return followingMouse.find(texture->name) != followingMouse.end();
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
	if (displayOpeningScreen)
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

bool GameView::renderLineColored(const Point& start, const Point& end, int thickness)
{
	return ren.renderLineColored(start, end, brushColor, thickness);
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

void GameView::checkCollisions()//std::shared_ptr<Physical> t1, std::shared_ptr<Physical> t2
{
	static int i = 0;
	if (i++ % 2 != 0)
		return;
	for (auto itr1 = physicals.begin(); itr1 != physicals.end(); ++itr1)
	{
		auto t1 = itr1->second;
		++itr1;
		for (auto itr2 = itr1; itr2 != physicals.end(); ++itr2)
		{
			auto t2 = itr2->second;
			//auto isCollision = t1->getRenderRect().contains(t2->center) || t2->getRenderRect().contains(t1->center);
			const auto isCollision = t1->getRenderRect().collides(t2->getRenderRect());
			if (isCollision)
			{
				engine->collide(t1, t2);
			}
		}
		--itr1;
	}
}

void GameView::playClickAnimation(const Point& atPoint)
{
	static int times = 0;
	auto animation = std::make_shared<ClickAnimation>(ClickAnimation("clickAnimation_"+std::to_string(times++),atPoint, { 255,0,0,0 }, 2,4, this));
	addTexture(animation,0);
	//debugText->setText(std::string("CLICK: x: ") + std::to_string(atPoint.x) + std::string(", y: ") + std::to_string(atPoint.y));

}