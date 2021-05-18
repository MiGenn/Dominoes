#include "Chip.h"

Chip::Chip(int topV, int bottomV) 
{
    this->topV = topV;
    this->bottomV = bottomV;
}

void Chip::shuffle(std::array<Chip, 28u>& chips)
{
    std::shuffle(chips.begin(), chips.end(), std::default_random_engine{ (unsigned)rand() });
}

void Chip::setTextures(std::array<Chip, 28u>& chips, Container& container)
{
	for (unsigned i{ 0 }; i < 28; ++i)
    {
        chips[i].sprite.setTexture(container.chips_tex);
        chips[i].sprite.setScale(scaleX, scaleY);
        chips[i].sprite.setTextureRect(sf::IntRect(i * 60, 0, 60, 120));
    }
}

int Chip::getV()
{
	return topV + bottomV;
}

int Chip::getTopV()
{
    return topV;
}

int Chip::getBottomV()
{
    return bottomV;
}

bool Chip::isDouble()
{
	return topV == bottomV;
}

float Chip::getWidth()
{
    return width;
}

float Chip::getHeight()
{
    return height;
}