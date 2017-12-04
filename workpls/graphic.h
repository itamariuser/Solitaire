#pragma once
#ifndef GRAPHIC
#define GRAPHIC
#include "point.h"
#include "color.h"
#include "gameView.h"
#include <SDL_image.h>
#include "Shapes.h"
#endif
namespace Graphic
{
	class Graphic
	{
	public:
		Point center;
		Point speed;
		GameView* gView;
		float mass;
		bool visible;
		Graphic(Point center, Point speed, GameView* gView, float mass = 1.0,bool visible = true) : center(center), speed(speed), gView(gView), mass(mass), visible(visible)
		{

		}

		virtual void draw(Color c) = 0;

	protected:
		virtual void next() { center += speed; }
	};

	class SimpleLine : public Graphic
	{
	public:

		Point end;
		Point endSpeed;
		SimpleLine(Point center, Point end, GameView* gView, Point startSpeed = Point(0, 0), Point endSpeed = Point(0, 0), float mass = 1.0) : Graphic(center, startSpeed, gView, mass), end(end), endSpeed(endSpeed) {}
		SimpleLine(GameView* gView, float mass = 1.0) : Graphic(Point(0, 0), Point(0, 0), gView, mass) {}
		virtual void draw(Color c = { 255, 165, 0, 255 })
		{
			next();
			if (visible)
			{
				gView->brushColor = c;
				gView->renderLineColored(center, end);
			}
		}

		virtual ~SimpleLine() {  };
	private:
		virtual void next()
		{
			center += speed;
			end += endSpeed;
		}
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
		virtual void draw(Color c = { 255, 165, 0, 255 })
		{
			next();
			if (visible)
			{
				gView->brushColor = c;
				gView->renderLineColored(uEnd, rEnd);
				gView->renderLineColored(rEnd, dEnd);
				gView->renderLineColored(dEnd, lEnd);
				gView->renderLineColored(lEnd, uEnd);
				gView->brushColor = secondaryColor;
				lu.draw(secondaryColor);
				lr.draw(secondaryColor);
				ld.draw(secondaryColor);
				ll.draw(secondaryColor);
			}
		}
		virtual ~Diamond() {  };
	private:
		virtual void next()
		{
			center += speed;
			uEnd += speed;
			rEnd += speed;
			dEnd += speed;
			lEnd += speed;
			lu.end = (lu.end.y < center.y) ? lu.end + Point(slope, slope) : uEnd;
			lr.end = (lr.end.x > center.x) ? lr.end + Point(-slope, slope) : rEnd;
			ld.end = (ld.end.y > center.y) ? ld.end + Point(-slope, -slope) : dEnd;
			ll.end = (ll.end.x < center.x) ? ll.end + Point(slope, -slope) : lEnd;
		}
	};

	class Circle : public Graphic
	{
	public:

		float radius;
		Circle(Point center, Point speed, GameView* gView, int radius = 20) : Graphic(center, speed, gView), radius(radius) {}

		virtual void draw(Color c = { 255, 165, 0, 255 })
		{
			gView->brushColor = c;
			next();
			if (visible)
			{
				auto pointsNumber = 10;
				if (pointsNumber == 0)
				{
					pointsNumber = int(M_PI * radius / 2);
				}

				float d_a = M_PI / pointsNumber,
					angle = d_a;

				Point start, end(radius, 0.0f);
				end += center;
				for (auto i = 0; i < pointsNumber * 2; i++)
				{
					start = end;
					end.x = cos(angle) * radius;
					end.y = sin(angle) * radius;
					end += center;
					angle += d_a;
					gView->renderLineColored(start, end);
				}
			}
		}

		virtual ~Circle() {  };
	private:
		virtual void next()
		{
			if (center.x + radius > gView->width || center.x - radius < 0)
				speed.x *= -1;
			if (center.y + radius > gView->height || center.y - radius < 0)
				speed.y *= -1;

			center += speed;
		}
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
			//renderRect = Shapes::Rect(center.x - width / 2, center.y - height / 2, width, height);
			center = { center.x +100, center.y +100};
			renderRect = Shapes::Rect(center.x,center.y , width, height);
		}

		virtual void draw(Color c = { 0, 0, 0, 0 })
		{
			next();
			if(visible)
				gView->ren.renderImage(*this);
			
		}

		void updateCenter(Point centerPt)
		{
			center.x = centerPt.x - renderRect.w/2;
			center.y = centerPt.y - renderRect.h/2;
		}
		virtual ~Texture() {  };
		const Shapes::Rect& getRenderRect() const { return renderRect; }
		const SDL_Texture* const getTexture() const { return texture; }
	protected:
		SDL_Texture* texture;
		Shapes::Rect renderRect;

		virtual void next()
		{
			Graphic::next();
			
			if (center.x  + renderRect.w > gView->width)
				center.x = gView->width - 1 - renderRect.w;

			else if (center.x < 0)
				center.x = 1;

			if (center.y + renderRect.h > gView->height )
				center.y = gView->height - 1 - renderRect.h;

			else if (center.y < 0)
				center.y = 1;
			renderRect.center = center;
		}
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
			//renderRect = Shapes::Rect(center.x - width / 2, center.y - height / 2, width, height);
			center = { center.x + 100, center.y + 100 };
			renderRect = Shapes::Rect(center.x, center.y, width, height);
		}

		virtual void draw(Color c = { 0, 0, 0, 0 })
		{
			next();
			if (visible)
				gView->ren.renderImage(*this);

		}

		inline void updateCenter(Point centerPt)
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

		virtual void next()
		{
			Graphic::next();

			if (center.x + renderRect.w > gView->width)
				center.x = gView->width - 1 - renderRect.w;

			else if (center.x < 0)
				center.x = 1;

			if (center.y + renderRect.h > gView->height)
				center.y = gView->height - 1 - renderRect.h;

			else if (center.y < 0)
				center.y = 1;
			renderRect.center = center;
		}
	};
}


