#pragma once
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Container.h"
#include "Chip.h"

class Hand
{
public:
    friend class Computer;
    friend class Reloader;
    friend class Table;
    friend class Score;

    enum class Who
    {
        Computer,
        Player
    };

    Hand(std::array<Chip, 28u>& chips, Container& container, Who who);

    void coverAll();
    void show();
    void hide();
    void draw(sf::RenderWindow& window);
    bool isEmpty();
    void take(int index);
    bool intersects(sf::Vector2i& mousePos);
    int pass(int index);
    int pass();

private:
    enum class State
    {
        Active,
        Inactive
    };

    void sort();
    void set();
    void leave();

    State state{ State::Inactive };
    std::reference_wrapper<std::array<Chip, 28u>> chips_ref;
    std::vector<int> indexes;
    sf::Sprite cover;
    sf::Sound take_chip;

    const Hand::Who who;
    float startX{ (modeX - (7.0f * Chip::getWidth())) / 2.0f };
    float startY;
    int moving{ -1 };
    bool show_all{ false };
};