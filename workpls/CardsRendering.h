#include "Graphic.h"
class Card : public Texture
{
public:
	Card(std::string name, Point center, Point speed, GameView* gView, SDL_Texture* texture, Point sizeSpeed, int width = -1, int height = -1)
		: Texture(name, center, speed, gView, texture, sizeSpeed, width, height), isRest(false), lastRestPoint(center), spacing(40)
	{
		getInfoFromName(name, type, color, number);
	}

	bool addCard(const std::shared_ptr<Card>& cardToAdd);
	void setResting(const bool& isResting) { isRest = isResting; }
	bool isResting() const { return isRest; }
	static bool canPutOnTop(const Card& existing, const Card& wantToPut);
	virtual void next();
	virtual ~Card() { };
protected:
	enum Type { clubs, diamonds, hearts, spades };
	enum Color { red, black };
	static void getInfoFromName(const std::string& name, Type& type, Color& color, int& number);
	int number;
	Type type;
	Color color;
	std::vector<std::shared_ptr<Card>> cardsHolding;
	bool isRest;
	Point lastRestPoint;
	int spacing;
};


class Deck : public Texture
{
public:
	Deck(std::string name, Point center, GameView* gView, SDL_Texture* texture, int width, int height, CardGenerator& generator, int spacing = 20) : Texture(name, center, { 0,0 }, gView, texture, { 0,0 }, width, height), generator(generator), spacing(spacing), currPos(center)
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