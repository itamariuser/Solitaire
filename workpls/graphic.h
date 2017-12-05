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
	public:
		Point& const getCenter() { return center; }
		Point& const getSpeed()  { return speed; }
		GameView* gView;
		float mass;
		bool visible;
		Graphic(Point center, Point speed, GameView* gView, float mass = 1.0, bool visible = true) : center(center), speed(speed), gView(gView), mass(mass), visible(visible) {}

		virtual void draw(Color c = { 0,0,0,0 }) = 0;

		virtual void next() { center += speed; }//
	protected:
		Point center;
		Point speed;
	};

	class SimpleLine : public Graphic
	{
	public:

		Point end;
		Point endSpeed;
		SimpleLine(Point center, Point end, GameView* gView, Point startSpeed = Point(0, 0), Point endSpeed = Point(0, 0), float mass = 1.0) : Graphic(center, startSpeed, gView, mass), end(end), endSpeed(endSpeed) {}
		SimpleLine(GameView* gView, float mass = 1.0) : Graphic(Point(0, 0), Point(0, 0), gView, mass) {}

		virtual void next();

		virtual void draw(Color c = { 255, 165, 0, 255 });

		virtual ~SimpleLine() {  };
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
		float slope;
		Color secondaryColor;

		Diamond(Point center, GameView* gView, Point speed, int halfHeight, int animationSpeed, Color secondaryColor) : Graphic(center, speed, gView), lu(gView), lr(gView), ld(gView), ll(gView), halfHeight(halfHeight), animationSpeed(animationSpeed), secondaryColor(secondaryColor)
		{

			lu = SimpleLine(center, Point(center.x, center.y - halfHeight), gView, speed, speed);
			lr = SimpleLine(center, Point(center.x + halfHeight, center.y), gView, speed, speed);
			ld = SimpleLine(center, Point(center.x, center.y + halfHeight), gView, speed, speed);
			ll = SimpleLine(center, Point(center.x - halfHeight, center.y), gView, speed, speed);
			slope = (lr.end.y - lu.end.y) / (lr.end.x - lu.end.x) * animationSpeed;
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

		float radius;
		Circle(Point center, Point speed, GameView* gView, int radius = 20) : Graphic(center, speed, gView), radius(radius) {}

		virtual void next();

		virtual void draw(Color c = { 255, 165, 0, 255 });

		virtual ~Circle() {  };
	private:
		
	};

	
	class Texture : public Graphic
	{
	public:
		Texture(Point center, Point speed, GameView* gView, SDL_Texture* texture, int width = -1, int height = -1) :Graphic(center, speed, gView), texture(texture),renderRect(0,0,0,0)
		{
			
			if(width == -1 && height == -1)
			{
				auto sizes = gView->ren.getImageSize(texture);
				width = sizes.x;
				height = sizes.y;
			}
			renderRect = Shapes::Rect(center.x,center.y , width, height);
		}

		virtual void next();

		virtual void draw(Color c = { 0, 0, 0, 0 });
		virtual ~Texture() {  };
		const Shapes::Rect& getRenderRect() const { return renderRect; }
		const SDL_Texture* const getTexture() const { return texture; }
	protected:
		SDL_Texture* texture;
		Shapes::Rect renderRect;

		
	};

	class Card : public Texture
	{
	public:
		Card(Point center, Point speed, GameView* gView, SDL_Texture* texture, int width = -1, int height = -1) :Texture(center,speed,gView,texture,width,height),renderRect(0, 0, 0, 0)
		{

			if (width == -1 && height == -1)
			{
				auto sizes = gView->ren.getImageSize(texture);
				width = sizes.x;
				height = sizes.y;
			}
			center = { center.x + 100, center.y + 100 };
			renderRect = Shapes::Rect(center.x, center.y, width, height);
		}

		virtual void next();

		virtual void draw(Color c = { 0, 0, 0, 0 });

		inline void setCenter(Point centerPt)
		{
			center.x = centerPt.x - renderRect.w / 2;
			center.y = centerPt.y - renderRect.h / 2;
		}
		virtual ~Card() {  };
		const Shapes::Rect& getRenderRect() const { return renderRect; }
		const SDL_Texture* const getTexture() const { return texture; }
	protected:
		SDL_Texture* texture;
		Shapes::Rect renderRect;

		
	};