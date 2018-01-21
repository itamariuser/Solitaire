#include "CardsRendering.h"

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
	else if (firstChar > '9' || firstChar < '1')
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
	return (existing.color != wantToPut.color && existing.number == wantToPut.number + 1);
}

void Card::next()
{
	Texture::next();
	if (!gView->isFollowingMouse(std::make_shared<Texture>(*this)))
	{
		isRest ? lastRestPoint = center : center = lastRestPoint;
	}
	setResting(false);

	auto putPoint = center;
	for (auto& card : cardsHolding)
	{
		putPoint.y += spacing;
		card->center = putPoint;
	}

}

bool Card::addCard(const std::shared_ptr<Card>& cardToAdd)
{
	if (!cardToAdd.get())
		return false;

	if (cardsHolding.empty())
	{
		if (!canPutOnTop(*this, *cardToAdd))
			return false;
	}
	else
	{
		if (!canPutOnTop(*cardsHolding.back(), *cardToAdd))
			return false;
	}

	cardsHolding.push_back(cardToAdd);
	return true;
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
	auto prio = highestPriority + cards.size();//65 255
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
			card->setCenter({ this->getCenter().x , offset });
			card->setResting(true);
			gView->getDrawPriorities()[card] = prio;
		}
	}
}