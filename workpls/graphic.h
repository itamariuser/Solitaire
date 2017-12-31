#pragma once
#ifndef GRAPHIC
#define GRAPHIC
#include "point.h"
#include "color.h"
#include "gameView.h"
#include <SDL_image.h>
#include "Shapes.h"
#endif

	class Graphic
	{
		friend class GameView;
	public:
		Point getCenter() const { return center; }
		Point getSpeed() const { return speed; }
		GameView* gView;
		float mass;
		
		Graphic(Point center, Point speed, GameView* gView, std::string name = "DEFAULT", float mass = 1.0, Color c = { 255,0,0,0 }, bool visible = true) : center(center), speed(speed), gView(gView), mass(mass), visible(visible), name(name), color(c), updateDraw(true) {}

		virtual void draw(Color c = { 0,0,0,0 }) = 0;

		virtual void next() { center += speed; }
		virtual void setCenter(Point p) { center = p; }
		virtual const std::string& getName() const { return name; }
		void setColor(const Color& c) 
		{
			if (color != c)
				setUpdateDraw();
			color = c; 
		}
		const Color& getColor() const { return color; }


		bool isVisible() const { return visible; }
		bool setVisible(const bool& isVisible) { visible = isVisible; }
		void setUpdateDraw() { updateDraw = true; }
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
		SimpleLine(Point center, Point end, GameView* gView, std::string name, Point startSpeed = Point(0, 0), Point endSpeed = Point(0, 0), float mass = 1.0) : Graphic(center, startSpeed, gView, name, mass), end(end), endSpeed(endSpeed) {}
		SimpleLine(GameView* gView, std::string name, float mass = 1.0) : Graphic(Point(0, 0), Point(0, 0), gView, name, mass) {}

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

		Diamond(Point center, GameView* gView, Point speed, int halfHeight, int animationSpeed, Color secondaryColor, std::string name) : Graphic(center, speed, gView, name), lu(gView,name), lr(gView, name), ld(gView, name), ll(gView, name), halfHeight(halfHeight), animationSpeed(animationSpeed), secondaryColor(secondaryColor)
		{

			lu = SimpleLine(center, Point(center.x, center.y - halfHeight), gView, name, speed, speed);
			lr = SimpleLine(center, Point(center.x + halfHeight, center.y), gView, name, speed, speed);
			ld = SimpleLine(center, Point(center.x, center.y + halfHeight), gView, name, speed, speed);
			ll = SimpleLine(center, Point(center.x - halfHeight, center.y), gView, name, speed, speed);
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
		Circle(Point center, Point speed, GameView* gView, int radius = 20, std::string name = "DEFAULT") : Graphic(center, speed, gView, name), radius(radius) {}

		virtual void next();

		virtual void draw(Color c = { 255, 165, 0, 255 });

		virtual ~Circle() {  };
	private:
		
	};

	
	class Texture : public Graphic
	{
	public:
		Texture(Point center, Point speed, GameView* gView, SDL_Texture* texture, Point sizeSpeed, std::string name ,int width = -1, int height = -1) :Graphic(center, speed, gView, name), sdlTexture(texture),renderRect(0,0,0,0),sizeSpeed(sizeSpeed)
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
		inline void setCenter(Point centerPt)
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
		Card(Point center, Point speed, GameView* gView, SDL_Texture* texture, Point sizeSpeed, int width = -1, int height = -1, std::string name = "DEFAULT")
			: Texture(center,speed,gView,texture,sizeSpeed,name,width,height)
		{
		}
		

		virtual ~Card() {  };

	protected:

		
	};

	class Text : public Texture
	{
	public:
		Text(Point center, Point speed, GameView* gView, const char* const fontPath, Color color, Point sizeSpeed, char* text, int width = -1, int height = -1, std::string name="DEFAULT") :Texture(center, speed, gView, nullptr, sizeSpeed, name, width, height), text(text)
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
		ColorSwitchText(Text baseText, Color colorSpeed = {0,0,0,0}) : Text(baseText), colorSpeed(colorSpeed) {}
		virtual void next();
	};