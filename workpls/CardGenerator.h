#pragma once
#include "GameView.h"

class CardGenerator
{
public:
	CardGenerator(GameView& gView);
	std::shared_ptr<Card> genCard(const Point& startPt);

private:
	std::shared_ptr<GameView> view;
	unsigned shuffleSeed;
	std::vector<std::pair<std::string, std::shared_ptr<SDL_Texture>>> cards;
	void shuffleCards();
};

