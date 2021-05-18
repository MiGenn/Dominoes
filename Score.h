#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Container.h"
#include "Hand.h"

class Score
{
public:
    friend class Reloader;

	Score(Hand& hand_1, Hand& hand_2, Container& container);
	
    void show();
    void hide();
	void count();
    void set(int new_max_score);
    void draw(sf::RenderWindow& window);
    void hideEvent();

private:
    enum class Mode
    {
        Strip,
        Table
    };

    enum class State
    {
        Active,
        Inactive
    };

    void chooseMode(Mode mode);
	void reset();
    void reload_score();

    std::reference_wrapper<Hand> hand_1_ref;
    std::reference_wrapper<Hand> hand_2_ref;

    int max_score{ 100 };
    sf::Sound end_of_round;
    sf::Text computer_score_text;
    sf::Text player_score_text;
    sf::Text event_text;
    sf::Sprite strip;
    sf::Sprite table;
    State state{ State::Inactive };
    Mode current_mode{ Mode::Strip };

    int computer_score{ 0 };
    int player_score{ 0 };

    bool showEvent{ false };
    int position_regulator{ 10 };
};