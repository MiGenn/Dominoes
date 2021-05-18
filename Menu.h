#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Container.h"

class Menu
{
public:
    friend class Reloader;

    enum class Mode
    {
        Main_menu,
        Esc_menu,
        Winnig_menu,
        Off,
    };

    enum class Button
    {
        Exit,
        Exit_to_menu,
        Restart,
        Play,
        Set_score,
    };

    Menu(Container& container);

    Mode getMode();

    int getScore();
    bool isZeroScore();
    bool isScoreMode();
    void scoreMode(bool state);
    void eraseLast();
    void setScore(char symbol);
    void toggle();
    void contain(sf::Vector2i& mousePos);
    bool pressed(sf::Vector2i& mousePos);
    const Button& wasPressed();
    void chooseMode(Mode mode);
    void draw(sf::RenderWindow& window);
    void esc_sound();

private:
    class MenuButton
    {
    public:
        enum class State
        {
            Active,
            Inactive
        };

        MenuButton(sf::Font& font)
        {
            text.setFont(font);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize((unsigned)((float)46u * scaleX));
        }

        sf::Text text;
        State state{ State::Inactive };
    };

    enum Sound
    {
        Button_choosed,
        Menu_opended
    };

    Mode current_mode{ Mode::Off };
    Mode previous_mode{ Mode::Off };
    Button was_pressed{ Button::Exit };

    std::array<MenuButton, 5u> buttons;
    std::array<sf::Sprite, 3u> textures;
    std::array<sf::Sound, 2u> sounds;

    sf::Text score_text;
    std::string new_score;
    bool deactivate_buttons{ false };
};