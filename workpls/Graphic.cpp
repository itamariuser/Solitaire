#include "Graphic.h"
#include "point.h"
#include "gameView.h"
#include "classRenderer.h"

void SimpleLine::draw(Color c)
{
	next();
	if (visible)
	{
		gView->brushColor = c;
		gView->renderLineColored(center, end);
	}
}

void SimpleLine::next()
{
	center += speed;
	end += endSpeed;
}

void Diamond::draw(Color c)
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

void Diamond::next()
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
//
void Circle::draw(Color c)
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

		double d_a = M_PI / double(pointsNumber), angle = d_a;

		Point start, end(radius, 0);
		end += center;
		for (auto i = 0; i < pointsNumber * 2; i++)
		{
			start = end;
			end.x = int(cos(angle) * radius);
			end.y = int(sin(angle) * radius);
			end += center;
			angle += d_a;
			gView->renderLineColored(start, end);
		}
	}
}

void Circle::next()
{
	if (center.x + radius > gView->width || center.x - radius < 0)
		speed.x *= -1;
	if (center.y + radius > gView->height || center.y - radius < 0)
		speed.y *= -1;

	center += speed;
}



void Texture::draw(Color c)
{
	next();
	if (visible)
		gView->renderImage(this->texture,&this->renderRect);

}

void Texture::next()
{
	Graphic::next();

	if (center.x + renderRect.w > gView->width)
	{
		center.x = gView->width - 1 - renderRect.w;
		flipSpeedX();
		stopSizeSpeedBoth();
	}

	else if (center.x < 0)
	{
		center.x = 1;
		flipSpeedX();
		stopSizeSpeedBoth();
	}

	if (center.y + renderRect.h > gView->height)
	{
		center.y = gView->height - 1 - renderRect.h;
		flipSpeedY();
		stopSizeSpeedBoth();
	}

	else if (center.y < 0)
	{
		center.y = 1;
		flipSpeedY();
		stopSizeSpeedBoth();
	}

	renderRect.w += sizeSpeed.x;
	renderRect.h += sizeSpeed.y;
	renderRect.center = center;
}

void Card::draw(Color c)
{
	Texture::draw();
}

void Card::next()
{
	Texture::next();

	/*if (center.x + renderRect.w > gView->width)
	{
		center.x = gView->width - 1 - renderRect.w;
	}

	else if (center.x < 0)
		center.x = 1;

	if (center.y + renderRect.h > gView->height)
	{
		center.y = gView->height - 1 - renderRect.h;
	}

	else if (center.y < 0)
		center.y = 1;
	renderRect.center = center;*/
}



 void Text::next()
{
	 Texture::next();
	 //if (center.x + renderRect.w > gView->width)
	 //{
		// center.x = gView->width - 1 - renderRect.w;
		// 
	 //}

	 //else if (center.x < 0)
		// center.x = 1;

	 //if (center.y + renderRect.h > gView->height)
	 //{
		// center.y = gView->height - 1 - renderRect.h;
		///* speed = {speed.x, -speed.y};*/
	 //}

	 //else if (center.y < 0)
		// center.y = 1;
	 //renderRect.center = center;
	 
}

 void Text::draw(Color c)
{
	 next();
	 //this->color = c;
	 gView->renderText(*this);
}
