#pragma once
#include <array>
#include <algorithm>
#include <random>
#include <SFML/Graphics.hpp>
#include "Container.h"

class Chip
{
public:
    friend class Computer;
    friend class Hand;
    friend class Table;
    friend class Boneyard;
    friend class Movement;

	Chip(int topV, int bottomV);

    static void shuffle(std::array<Chip, 28u>& chips);
    static void setTextures(std::array<Chip, 28u>& chips, Container& container);

    int getV();
    int getTopV();
    int getBottomV();
    bool isDouble();
    static float getWidth();
    static float getHeight();

private:
    enum class State
    {
        Active,
        Inactive
    };

    sf::Sprite sprite;
    int topV{ 0 };
    int bottomV{ 0 };
    State state{ State::Active };

    inline static float width{ 60.0f * scaleX };
    inline static float height{ 120.0f * scaleY };
};