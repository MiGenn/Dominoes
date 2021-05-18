#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include "Chip.h"

class Movement
{
public:
	Movement(std::array<Chip, 28u>& chips);

	void draw(sf::RenderWindow& window);
	bool isActive();
	void giveBack();
	void take(int index, sf::Vector2i mousePos);
	void move(sf::Vector2i mousePos);
	void leave();
	int pass();

private:
	enum class State
	{
		Active,
		Inactive
	};

	int chip_index{ -1 };
	float dx{ 0.0f };
	float dy{ 0.0f };
	State state{ State::Inactive };
	std::reference_wrapper<std::array<Chip, 28u>> chips_ref;
	sf::Vector2f chipPos;
};