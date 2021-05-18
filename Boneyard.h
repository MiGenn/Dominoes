#pragma once
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Container.h"
#include "Chip.h"

class Boneyard
{
public:
	friend class Reloader;

	Boneyard(std::array<Chip, 28u>& chips, Container& Container);

	int size();
	void show();
	void hide();
	bool isActive();
	bool isEmpty();
	bool intersects(sf::Vector2i mousePos);
	int pass(int index);
	int pass();
	void draw(sf::RenderWindow& window);

private:
	enum class State
	{
		Active,
		Inactive
	};

	void set();
	void leave();

	State state{ State::Inactive };
	std::reference_wrapper<std::array<Chip, 28u>> chips_ref;
	std::vector<int> indexes;
	sf::Sprite boneyard;
	sf::Sprite cover;

	int passIN{ -1 };
	float startX;
	float startY;
};