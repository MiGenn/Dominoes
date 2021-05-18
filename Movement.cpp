#include "Movement.h"

Movement::Movement(std::array<Chip, 28u>& chips) :
	chips_ref(chips)
{

}

void Movement::draw(sf::RenderWindow& window)
{
	if (state == State::Inactive)
		return;

	window.draw(chips_ref.get()[chip_index].sprite);
}

bool Movement::isActive()
{
	return state == State::Active;
}

void Movement::take(int index, sf::Vector2i mousePos)
{
	if (state == State::Inactive)
	{
		chipPos = chips_ref.get()[index].sprite.getPosition();
		state = State::Active;
	}

	chip_index = index;
	dx = (float)mousePos.x - chips_ref.get()[index].sprite.getPosition().x;
	dy = (float)mousePos.y - chips_ref.get()[index].sprite.getPosition().y;
}

void Movement::move(sf::Vector2i mousePos)
{
	if (state == State::Active)
		chips_ref.get()[chip_index].sprite.setPosition((float)mousePos.x - dx, (float)mousePos.y - dy);
}

void Movement::leave()
{
	state = State::Inactive;
	chip_index = -1;
}

int Movement::pass()
{
	return chip_index;
}

void Movement::giveBack()
{
	if (state == State::Inactive)
		return;

	chips_ref.get()[chip_index].sprite.setPosition(chipPos);
	leave();
}