#pragma once
#include "point.h"
#include "color.h"
#include "gameView.h"
#include "Shapes.h"



class Mover
{
public:
	Mover(const Point& speed) : speed(speed)
 	{
	}

	void setSpeed(const Point& speed) { this->speed = speed; }
	Point getSpeed() const { return speed; }
	virtual void flipSpeedY() { speed = { speed.x,-speed.y }; }
	virtual void flipSpeedX() { speed = { -speed.x,speed.y }; }
	virtual void stopSpeedX() { speed = { 0,speed.y }; }
	virtual void stopSpeedY() { speed = { speed.x,0 }; }
	virtual ~Mover() = 0;
protected:
	Point speed;
	
};

class Graphic : public Mover
{
	friend class GameView;
public:
	Point getCenter() const { return center; }
	Point getSpeed() const { return speed; }
	GameView* gView;
	

	Graphic(std::string name, Point center, Point speed, GameView* gView, Color c = { 255,0,0,0 }, bool visible = true) : center(center), Mover(speed), gView(gView), visible(visible), name(name), color(c), updateDraw(true), shouldBeDestroyed(false) {}

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
	bool getShouldBeDestroyed() const { return shouldBeDestroyed; }
	virtual ~Graphic() = 0;
protected:
	Point center;
	
	std::string name;
	Color color;
	bool updateDraw;
	bool visible;
	bool shouldBeDestroyed;
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

class SizeChanger
{
public:
	SizeChanger(const Point& sizeSpeed) : sizeSpeed(sizeSpeed) {}

	virtual void flipSizeSpeedY() { sizeSpeed = { sizeSpeed.x,-sizeSpeed.y }; }
	virtual void flipSizeSpeedX() { sizeSpeed = { -sizeSpeed.x,sizeSpeed.y }; }
	virtual void stopSizeSpeedX() { sizeSpeed = { 0,sizeSpeed.y }; }
	virtual void stopSizeSpeedY() { sizeSpeed = { sizeSpeed.x,0 }; }
	virtual void stopSizeSpeedBoth() { stopSizeSpeedX(); stopSizeSpeedY(); }

	virtual void setSizeSpeed(const Point& p) { sizeSpeed = p; }
	Point getSizeSpeed() const { return sizeSpeed; }
	virtual ~SizeChanger() = 0;
	
protected:
	Point sizeSpeed;
};

class Texture : public Graphic , public SizeChanger
{
public:
	Texture(std::string name, Point center, Point speed, GameView* gView, SDL_Texture* texture = nullptr, Point sizeSpeed = { 0,0 }, int width = -1, int height = -1) :Graphic(name, center, speed, gView), sdlTexture(texture), renderRect(0, 0, 0, 0), SizeChanger(sizeSpeed)
	{
		if (width == -1 && height == -1)
		{
			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
		}
		renderRect = Shapes::Rect(center.x, center.y, width, height);
	}

	virtual void next();

	virtual void draw(Color c = { 0, 0, 0, 0 });

	virtual Point getCenter() const
	{
		return { center.x - renderRect.w / 2 , center.y - renderRect.h / 2 };
	}
	void setCenter(const Point& centerPt)
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
	
	mutable SDL_Surface* sdlSurface;
	mutable SDL_Texture* sdlTexture;

	
};

class Text : public Texture
{
public:
	Text(std::string name, Point center, Point speed, GameView* gView, const char* const fontPath, Color color, Point sizeSpeed, char* text, int width = -1, int height = -1);


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

class ClickAnimation : public Texture// can also be fireworks!
{
public:
	ClickAnimation(const std::string& name, const Point& center, const Color& drawColor, int length, int times, GameView* gView) : Texture(name, center, { 0,0 },gView),
		length(length), timesLeft(times),stop_upper_right(false),
		upper_right_start(center), upper_right(center),
		upper_left_start(center), upper_left(center),

		middle_right_start(center), middle_right(center),
		middle_left_start(center), middle_left(center),

		lower_right_start(center), lower_right(center),
		lower_left_start(center), lower_left(center),

		up_start(center), up(center),
		down_start(center), down(center)
	{
		color = drawColor;
	}
	virtual void draw(Color c = { 0,0,0,0 });
	virtual void next();

private:
	Point upper_right_start;
	Point upper_right;

	Point upper_left_start;
	Point upper_left;
	
	Point middle_right_start;
	Point middle_right;

	Point middle_left_start;
	Point middle_left;

	Point lower_right_start;
	Point lower_right;

	Point lower_left_start;
	Point lower_left;

	Point up_start;
	Point up;
	
	Point down_start;
	Point down;

	int length;
	int timesLeft;
	bool stop_upper_right;
};