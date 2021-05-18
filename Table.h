#pragma once
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Container.h"
#include "Computer.h"
#include "Chip.h"
#include "Hand.h"

class Table
{
private:
    class Helper
    {
    public:
        enum class Way
        {
            Left,
            Right,
            Down
        };

        enum class Part
        {
            Center,
            Top,
            Bottom
        };

        Helper(sf::Texture& texture)
        {
            for (sf::Sprite& element : parts)
            {
                element.setTexture(texture);
                element.setScale(scaleX, scaleY);
            }
            for (int i{ 0 }; i < 3; ++i)
                parts[i].setTextureRect(sf::IntRect(60 * i, 0, 60, 120));
        }

        void move(float x, float y)
        {
            for (sf::Sprite& element : parts)
                element.move(x, y);
        }

        void setPosition(float x, float y)
        {
            for (sf::Sprite& element : parts)
                element.setPosition(x, y);
        }

        void setRotation(float angle)
        {
            for (sf::Sprite& element : parts)
                element.setRotation(angle);
        }

        sf::Vector2f getPosition()
        {
            return parts[(int)Part::Center].getPosition();
        }

        void changePart()
        {
            if (way == Way::Left)
            {
                if (isVertical)
                    current_part = Part::Center;
                else
                    current_part = Part::Top;
            }
            else if (way == Way::Right)
            {
                if (isVertical)
                    current_part = Part::Center;
                else
                    current_part = Part::Bottom;
            }
            else if (way == Way::Down)
            {
                if (isVertical)
                    current_part = Part::Top;
                else
                    current_part = Part::Center;
            }
        }

        int v{ 0 };
        int amount{ 0 };
        bool isVertical{ true };
        bool was_first_transition{ false };
        Way way{ Way::Down };
        std::array<sf::Sprite, 3u> parts;
        Part current_part{ Part::Center };
    };

public:
    friend class Reloader;

    Table(int start_index, std::array<Chip, 28u>& chips, Container& container);

    void show();
    void hide();
    bool findSuitable(Hand& hand);
    static int find_best_chip(Hand& hand_1, Hand& hand_2, Computer& computer);
    bool isSuitable(int index);
    void place(int index);
    void draw(sf::RenderWindow& window);

private:
    enum class State
    {
        Active,
        Inactive
    };

    enum Which_Helper
    {
        First,
        Second
    };

    void set(int start_index);
    void leave();

    State state{ State::Inactive };
    std::reference_wrapper<std::array<Chip, 28u>> chips_ref;
    std::vector<int> indexes;
    sf::Sound place_chip;

    std::array<Helper, 2u> helpers;
    float startX;
    float startY;

    inline static bool computeIsFirst{ false };
    bool player_put{ false };
    int move_all_up{ -1 };
    bool first_turn{ true };
    int suitable{ 0 };
    int iterator{ 0 };
};