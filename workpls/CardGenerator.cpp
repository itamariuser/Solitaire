#include "CardGenerator.h"
#include <chrono>
#include <random>
#include "Graphic.h"

CardGenerator::CardGenerator(GameView& gView) : view(&gView)
{
	shuffleCards();
}

std::shared_ptr<Card> CardGenerator::genCard(const Point & startPt)
{
	auto size = view->getDefaultCardSize();
	auto chosenEntry = cards.back();
	auto card = std::make_shared<Card>(Card{ chosenEntry.first, startPt,{ 0,0 }, view.get(), chosenEntry.second.get(),{ 0,0 }, size.x, size.y });
	cards.pop_back();
	return card;
}

void CardGenerator::shuffleCards()
{
	auto loadedCards = view->getLoadedCards();
	for (auto citr = loadedCards->cbegin(); citr != loadedCards->cend(); ++citr)
	{
		cards.push_back(*citr);
	}
	shuffleSeed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(cards.begin(), cards.end(), std::default_random_engine(shuffleSeed));
}
