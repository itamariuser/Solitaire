#pragma once
#include "point.h"
#include "color.h"
#include "gameView.h"
#include "Shapes.h"
#include <time.h>
#include <chrono>
#include <random>
#include "CardGenerator.h"
class Graphic
{
	friend class GameView;
public:
	Point getCenter() const { return center; }
	Point getSpeed() const { return speed; }
	GameView* gView;
	float mass;

	Graphic(std::string name, Point center, Point speed, GameView* gView, float mass = 1.0, Color c = { 255,0,0,0 }, bool visible = true) : center(center), speed(speed), gView(gView), mass(mass), visible(visible), name(name), color(c), updateDraw(true) {}

	virtual void draw(Color c = { 0,0,0,0 }) = 0;

	virtual void next() { center += speed; }
	virtual void setCenter(const Point& p) { center = p; }
	virtual const std::string& getName() const { return name; }
	void setColor(const Color& c)
	{
		if (color != c)
			setUpdateDraw();
		color = c;
	}
	const Color& getColor() const { return color; }

	bool isVisible() const { return visible; }
	void setVisible(const bool& isVisible) { visible = isVisible; }
	void setUpdateDraw() { updateDraw = true; }

	virtual ~Graphic() = 0;
protected:
	Point center;
	Point speed;
	virtual void flipSpeedY() { speed = { speed.x,-speed.y }; }
	virtual void flipSpeedX() { speed = { -speed.x,speed.y }; }
	virtual void stopSpeedX() { speed = { 0,speed.y }; }
	virtual void stopSpeedY() { speed = { speed.x,0 }; }
	std::string name;
	Color color;
	bool updateDraw;
	bool visible;
};



class SimpleLine : public Graphic
{
public:

	Point end;
	Point endSpeed;
	SimpleLine(std::string name, Point center, Point end, GameView* gView, Point startSpeed = Point(0, 0), Point endSpeed = Point(0, 0), float mass = 1.0) : Graphic(name, center, startSpeed, gView, mass), end(end), endSpeed(endSpeed) {}
	SimpleLine(std::string name, GameView* gView, float mass = 1.0) : Graphic(name, Point(0, 0), Point(0, 0), gView, mass) {}

	virtual void next();

	virtual void draw(Color c = { 255, 165, 0, 255 });

	virtual ~SimpleLine() { };
private:

};

class Diamond : public Graphic
{
public:
	SimpleLine lu;
	SimpleLine lr;
	SimpleLine ld;
	SimpleLine ll;

	Point uEnd;
	Point rEnd;
	Point dEnd;
	Point lEnd;

	int halfHeight;
	int animationSpeed;
	int slope;
	Color secondaryColor;

	Diamond(std::string name, Point center, GameView* gView, Point speed, int halfHeight, int animationSpeed, Color secondaryColor) : Graphic(name, center, speed, gView), lu(name, gView), lr(name, gView), ld(name, gView), ll(name, gView), halfHeight(halfHeight), animationSpeed(animationSpeed), secondaryColor(secondaryColor)
	{

		lu = SimpleLine(name, center, Point(center.x, center.y - halfHeight), gView, speed, speed);
		lr = SimpleLine(name, center, Point(center.x + halfHeight, center.y), gView, speed, speed);
		ld = SimpleLine(name, center, Point(center.x, center.y + halfHeight), gView, speed, speed);
		ll = SimpleLine(name, center, Point(center.x - halfHeight, center.y), gView, speed, speed);
		slope = int((lr.end.y - lu.end.y) / float(lr.end.x - lu.end.x) * float(animationSpeed));
		uEnd = lu.end;
		rEnd = lr.end;
		dEnd = ld.end;
		lEnd = ll.end;
	}

	virtual void next();

	virtual void draw(Color c = { 255, 165, 0, 255 });
	virtual ~Diamond() {  };
private:

};

class Circle : public Graphic
{
public:

	int radius;
	Circle(std::string name, Point center, Point speed, GameView* gView, int radius = 20) : Graphic(name, center, speed, gView), radius(radius) {}

	virtual void next();

	virtual void draw(Color c = { 255, 165, 0, 255 });

	virtual ~Circle() {  };
private:

};


class Texture : public Graphic
{
public:
	Texture(std::string name, Point center, Point speed, GameView* gView, SDL_Texture* texture, Point sizeSpeed, int width = -1, int height = -1) :Graphic(name, center, speed, gView), sdlTexture(texture), renderRect(0, 0, 0, 0), sizeSpeed(sizeSpeed)
	{
		if (width == -1 && height == -1)
		{
			Point sizes;
			SDL_QueryTexture(texture, nullptr, nullptr, &sizes.x, &sizes.y);
			width = sizes.x;
			height = sizes.y;
		}
		renderRect = Shapes::Rect(center.x, center.y, width, height);
	}

	virtual void next();

	virtual void draw(Color c = { 0, 0, 0, 0 });

	virtual Point getCenter() const
	{
		return { center.x - renderRect.w / 2 , center.y - renderRect.h / 2 };
	}
	inline void setCenter(const Point& centerPt)
	{
		center.x = centerPt.x - renderRect.w / 2;
		center.y = centerPt.y - renderRect.h / 2;
	}

	virtual void updateTexture() {}

	virtual const SDL_Texture* const getTexture() const { return sdlTexture; }

	const Shapes::Rect& getRenderRect() const { return renderRect; }
	virtual ~Texture() {  };



protected:
	Shapes::Rect renderRect;
	Point sizeSpeed;
	mutable SDL_Surface* sdlSurface;
	mutable SDL_Texture* sdlTexture;

	virtual void flipSizeSpeedY() { sizeSpeed = { sizeSpeed.x,-sizeSpeed.y }; }
	virtual void flipSizeSpeedX() { sizeSpeed = { -sizeSpeed.x,sizeSpeed.y }; }
	virtual void stopSizeSpeedX() { sizeSpeed = { 0,sizeSpeed.y }; }
	virtual void stopSizeSpeedY() { sizeSpeed = { sizeSpeed.x,0 }; }
	virtual void stopSizeSpeedBoth() { stopSizeSpeedX(); stopSizeSpeedY(); }
};

class Card : public Texture
{
public:
	Card(std::string name, Point center, Point speed, GameView* gView, SDL_Texture* texture, Point sizeSpeed, int width = -1, int height = -1)
		: Texture(name, center, speed, gView, texture, sizeSpeed, width, height)
	{
		getInfoFromName(name, type, color, number);
	}

	virtual ~Card() { };
	
	int number;
	static bool canPutOnTop(const Card& existing, const Card& wantToPut);
protected:
	enum Type { clubs, diamonds, hearts, spades };
	enum Color { red, black };
	static void getInfoFromName(const std::string& name, Type& type, Color& color, int& number);
	Type type;
	Color color;
};

class Text : public Texture
{
public:
	Text(std::string name, Point center, Point speed, GameView* gView, const char* const fontPath, Color color, Point sizeSpeed, char* text, int width = -1, int height = -1) :Texture(name, center, speed, gView, nullptr, sizeSpeed, width, height), text(text)
	{
		this->color = color;
		font = const_cast<TTF_Font*>(gView->getFont(fontPath));
		sdlSurface = TTF_RenderText_Solid(const_cast<TTF_Font*>(font), text, color);
		sdlTexture = SDL_CreateTextureFromSurface(gView->ren.renderer, sdlSurface);
	}


	virtual void draw(Color c = { 0, 0, 0, 0 });


	virtual ~Text() {  };

	virtual void updateTexture();
	std::string getText() const { return text; }
	void setText(std::string newText)
	{
		if (newText != text)
			setUpdateDraw();
		text = newText;
	}

protected:
	TTF_Font* font;
	std::string text;
};

class ColorSwitchText : public Text
{
	Color colorSpeed;
public:
	ColorSwitchText(Text baseText, Color colorSpeed = { 0,0,0,0 }) : Text(baseText), colorSpeed(colorSpeed) {}
	virtual void next();
};

class Deck : public Texture
{
public:
	Deck(std::string name, Point center, GameView* gView, SDL_Texture* texture, int width, int height, CardGenerator& generator, int spacing = 20 ) : Texture(name, center, { 0,0 }, gView, texture, { 0,0 }, width, height), generator(generator), spacing(spacing), currPos(center)
	{

	}
	std::shared_ptr<Card> genCard(const Point& startPt) { return generator.genCard(startPt); }
	std::shared_ptr<Card> genCard()
	{
		currPos += Point{ gView->getDefaultCardSize().x + spacing,0 };
		return generator.genCard(currPos);
	}
private:
	CardGenerator generator;
	int spacing;
	Point currPos;
};

class Stack : public Texture
{
public:
	Stack(std::string name, Point center, GameView* gView, SDL_Texture* texture, int width, int height, int spacing = 35) : Texture(name, center, { 0,0 }, gView, texture, { 0,0 }, width, height), spacing(spacing)
	{
	}

	auto getOpenCards()
	{
		return std::make_shared<std::vector<std::shared_ptr<Card>>>(openCards);
	}

	bool addCards(const std::vector<std::shared_ptr<Card>>& cardsToAdd);

	virtual void next();
private:
	std::vector<std::shared_ptr<Card>> cards;
	std::vector<std::shared_ptr<Card>> openCards;
	int spacing;
};