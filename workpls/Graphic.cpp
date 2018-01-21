#include "Graphic.h"
#include "point.h"
#include "gameView.h"
#include "classRenderer.h"

Graphic::~Graphic()
{

}


void SimpleLine::draw(Color c)
{
	next();
	gView->brushColor = c;
	gView->renderLineColored(center, end);
}

void SimpleLine::next()
{
	Graphic::next();
	end += endSpeed;
}

void Diamond::draw(Color c)
{
	next();
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
	updateDraw = false;
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
	updateDraw = false;
}

void Circle::next()
{
	Graphic::next();
	if (center.x + radius > gView->window.getDimensions().x || center.x - radius < 0)
		speed.x *= -1;
	if (center.y + radius > gView->window.getDimensions().y || center.y - radius < 0)
		speed.y *= -1;

	
}

void Texture::draw(Color c)
{
	next();
	gView->renderImage(this->sdlTexture,&this->renderRect);
	updateDraw = false;
}

void Texture::next()
{
	Graphic::next();

	if (center.x + renderRect.w > gView->window.getDimensions().x)
	{
		center.x = gView->window.getDimensions().x - 1 - renderRect.w;
		//flipSpeedX();
		//stopSizeSpeedBoth();
	}

	else if (center.x < 0)
	{
		center.x = 1;
		/*flipSpeedX();
		stopSizeSpeedBoth();*/
	}

	if (center.y + renderRect.h > gView->window.getDimensions().y)
	{
		center.y = gView->window.getDimensions().y - 1 - renderRect.h;
		/*flipSpeedY();
		stopSizeSpeedBoth();*/
	}

	else if (center.y < 0)
	{
		center.y = 1;
		/*flipSpeedY();
		stopSizeSpeedBoth();*/
	}

	renderRect.w += sizeSpeed.x;
	renderRect.h += sizeSpeed.y;
	renderRect.center = center;
}

void Text::draw(Color c)
{
	next();
	updateTexture();
	gView->renderText(*this);
	updateDraw = false;
}

void Text::updateTexture()
{
	if (updateDraw)
	{
		if (sdlTexture)
			SDL_DestroyTexture(sdlTexture);
		SDL_FreeSurface(sdlSurface);
		sdlSurface = TTF_RenderText_Solid(const_cast<TTF_Font*>(font), text.c_str(), color);
		sdlTexture = SDL_CreateTextureFromSurface(gView->ren.renderer, sdlSurface);
	}
}

void ColorSwitchText::next()
{
	Text::next();
	setColor(color + colorSpeed);
}

void ClickAnimation::next()
{

}

void ClickAnimation::draw(Color c)
{
	next();
	gView->brushColor = this->color;
	if (!stop_upper_right)
		upper_right += {length, -length};
	else
		upper_right_start += {length, -length};
	if (upper_right_start == upper_right)
		shouldBeDestroyed = true;
	
	auto yDist = abs(center.y - upper_right.y);
	auto xDist = abs(center.x - upper_right.x);


	upper_left = { center.x - xDist, center.y - yDist };

	middle_right = { center.x + int(xDist*1.2), center.y };
	middle_left = { center.x - int(xDist*1.2), center.y };

	lower_right = { center.x + xDist, center.y + yDist };
	lower_left = { center.x - xDist, center.y + yDist };

	up = { center.x, center.y - int(yDist*1.2) };
	down = { center.x, center.y + int(yDist*1.2) };

	gView->renderLineColored(lower_right_start, lower_right);//lower_right
	gView->renderLineColored(lower_left_start, lower_left);//lower_left

	gView->renderLineColored(upper_right_start, upper_right);//upper_right
	gView->renderLineColored(upper_left_start, upper_left);//upper_left

	gView->renderLineColored(down_start, down);//down
	gView->renderLineColored(up_start, up);//up

	gView->renderLineColored(middle_left_start, middle_left);//left
	gView->renderLineColored(middle_right_start, middle_right);//right
	if (timesLeft-- <= 0)
		stop_upper_right = true;
}
