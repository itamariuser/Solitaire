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
		flipSpeedX();
		stopSizeSpeedBoth();
	}

	else if (center.x < 0)
	{
		center.x = 1;
		flipSpeedX();
		stopSizeSpeedBoth();
	}

	if (center.y + renderRect.h > gView->window.getDimensions().y)
	{
		center.y = gView->window.getDimensions().y - 1 - renderRect.h;
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

 void Card::getInfoFromName(const std::string& name, Type& type, Color& color, int& number)
 {
	auto token = strtok(const_cast<char*>(name.c_str()), "_");
	std::vector<std::string> attributes;
	//split into tokens and put into attributes
	while (token)
	{
		attributes.push_back(token);
		token = strtok(nullptr, "_");
	}
	if (attributes.size() < 3)
		throw std::exception("Card name not in format : \"[number]_of_[type] \"");

	// check card's number
	auto firstChar = attributes[0][0];
	if (attributes[0] == "10")
	{
		//handle 2 digit number
		number = 10;
	}
	else if (firstChar> '9' || firstChar < '1')
	{
		//card's number is royal / ace
		auto royalNumber = attributes[0];
		std::transform(royalNumber.begin(), royalNumber.end(), royalNumber.begin(), ::tolower); // tolower(royalNumber)
		if (royalNumber == "jack")
			number = 11;
		else if (royalNumber == "queen")
			number = 12;
		else if (royalNumber == "king")
			number = 13;
		else //if (royalNumber == "ace")
			number = 1;
	}
	else
	{
		//card's number is normal
		number = firstChar - '0';
	}

	//check card's type
	auto typeToken = attributes[2];
	color = black;

	if (typeToken == "diamonds")
	{
		type = diamonds;
		color = red;
	}
	else if (typeToken == "hearts")
	{
		type = hearts;
		color = red;
	}
	else if (typeToken == "clubs")
		type = clubs;
	else //if (typeToken == "spades")
	{
		type = spades;
	}
 }

 bool Card::canPutOnTop(const Card& existing, const Card& wantToPut)
 {
	 return (existing.color != wantToPut.color && existing.number > wantToPut.number);
 }

 bool Stack::addCards(const std::vector<std::shared_ptr<Card>>& cardsToAdd)
 {
	 if (cards.empty() && openCards.empty())
	 {
		 cards.insert(cards.begin(), cardsToAdd.begin(), cardsToAdd.end());
		 openCards.insert(openCards.begin(), cardsToAdd.begin(), cardsToAdd.end());
		 return true;
	 }
	 auto topCard = openCards.back();
	 if (cardsToAdd.empty() || !Card::canPutOnTop(*topCard, *cardsToAdd.back())) return false;

	 return true;
 }

 void Stack::next()
 {
	 auto distance = 0;
	 auto highestPriority = 11;
	 auto prio = highestPriority + cards.size();
	 for (auto itr = cards.begin(); itr != cards.end(); ++itr, ++distance, --prio)
	 {
		 auto card = *itr;
		 if (gView->isFollowingMouse(card))
		 {
			 //remove card from both openCards and cards
			 openCards.erase(openCards.begin() + distance);
			 itr = cards.erase(itr);
			 if (itr == cards.end()) break;
			 continue;
		 }
		 else
		 {
			 auto offset = this->getCenter().y + (distance *  spacing);
			 card->setCenter({this->getCenter().x , offset});
			 gView->getDrawPriorities()[card] = prio;
		 }
	 }
 }
