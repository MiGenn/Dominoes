#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const sf::VideoMode vMode{ sf::VideoMode::getDesktopMode() };
const float modeX{ (float)vMode.width };
const float modeY{ (float)vMode.height };
const float scaleX = modeX / 1920.0f;
const float scaleY = modeY / 1080.0f;

class Container
{
public:
    Container();

    sf::SoundBuffer menu_opened_sound;
    sf::SoundBuffer button_choosed_sound;
    sf::SoundBuffer place_chip_sound;
    sf::SoundBuffer end_of_round_sound;
    sf::SoundBuffer set_score_sound;
    sf::Sound set_score;

    sf::Font font;

    sf::Texture background_tex;
    sf::Texture main_menu_tex;
    sf::Texture chips_tex;
    sf::Texture special_chips_tex;
    sf::Texture helper_tex;
    sf::Texture strip_tex;
    sf::Texture table_tex;
    sf::Texture boneyard_tex;
    sf::Texture winning_menu_tex;
    sf::Texture esc_menu_tex;
    sf::Sprite background;
};