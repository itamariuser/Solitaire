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
	//init_objects();
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
	auto card0 = std::shared_ptr<Card>(new Card(std::string("king_of_spades"), { 30,280 }, { 0,0 }, this, getTexture("king_of_spades.png").get(), { 0,0 }, defaultCardSize.x, defaultCardSize.y));
	auto card1 = std::shared_ptr<Card>(new Card(std::string("queen_of_hearts"), {}, { 0,0 }, this, getTexture("queen_of_hearts.png").get(), { 0,0 }, defaultCardSize.x, defaultCardSize.y));
	
	auto card2 = std::shared_ptr<Card>(new Card(std::string("jack_of_clubs"), {}, { 0,0 }, this, getTexture("jack_of_clubs.png").get(), { 0,0 }, defaultCardSize.x, defaultCardSize.y));
	card0->addCard(card1);
	card0->addCard(card2);
	
	
	std::vector<std::shared_ptr<Card>> cards{ card0, card1, card2 };
	addTexture(card0, 12, true);
	addTexture(card1, 11, true);
	addTexture(card2, 10, true);
	addTexture(std::shared_ptr<Text>(new Text("debugText", { 700,100 }, { 0,0 }, this, "arial.ttf", { 0,200,0,0 }, { 0,0 }, "DEBUG TEXT", 160, 100)), 0);

	CardGenerator generator(*this);
	addTexture(std::shared_ptr<Deck>(new Deck("deck", { 150,60 }, this, getTexture("king_of_spades.png").get(), defaultCardSize.x, defaultCardSize.y, generator, 10)), 12);
	auto stack = new Stack("stack0", { 30,280 }, this, nullptr, defaultCardSize.x, defaultCardSize.y);
	//stack->addCards(cards);
	addTexture(std::shared_ptr<Stack>(stack),20);

	/*putRandomCardAt({ 500,500 }); putRandomCardAt({ 500,500 });
	putRandomCardAt({ 100,500 });*/
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
		if (texture->isVisible())
			texture->draw();
	}
}

void GameView::putRandomCardAt(const Point& pt)
{
	auto deck = std::dynamic_pointer_cast<Deck>(getObject("deck"));
	addTexture(deck->genCard(),latestPriority,true);
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

bool GameView::collide(std::shared_ptr<Texture> g1, std::shared_ptr<Texture> g2)
{
	return g1->getRenderRect().contains(g2->center) || g2->getRenderRect().contains(g1->center);
}
