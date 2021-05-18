#include "Score.h"

Score::Score(Hand& hand_1, Hand& hand_2, Container& container) :
    hand_1_ref(hand_1), hand_2_ref(hand_2)
{
    end_of_round.setBuffer(container.end_of_round_sound);

    strip.setTexture(container.strip_tex);
    strip.setScale(scaleX, scaleY);
    table.setTexture(container.table_tex);
    table.setScale(scaleX, scaleY);

    event_text.setFont(container.font);
    event_text.setCharacterSize((unsigned)((float)54u * scaleX));
    event_text.setFillColor(sf::Color::White);
    computer_score_text.setFont(container.font);
    computer_score_text.setCharacterSize((unsigned)((float)46u * scaleX));
    computer_score_text.setFillColor(sf::Color::White);
    player_score_text.setFont(container.font);
    player_score_text.setCharacterSize((unsigned)((float)46u * scaleX));
    player_score_text.setFillColor(sf::Color::White);

    reload_score();
    chooseMode(Mode::Strip);
}

void Score::show()
{
    state = State::Active;
}

void Score::hide()
{
    state = State::Inactive;
}

void Score::count()
{
    int total_amount_temp{ 0 };
    int computer_points_temp{ 0 };
    int player_points_temp{ 0 };

    int temp{ 0 };
    for (int in : hand_1_ref.get().indexes)
        temp += hand_1_ref.get().chips_ref.get()[in].getV();
    if (hand_1_ref.get().who == Hand::Who::Computer)
        computer_points_temp += temp;
    else
        player_points_temp += temp;

    temp = 0;
    for (int in : hand_2_ref.get().indexes)
        temp += hand_2_ref.get().chips_ref.get()[in].getV();
    if (hand_2_ref.get().who == Hand::Who::Computer)
        computer_points_temp += temp;
    else
        player_points_temp += temp;

    total_amount_temp = abs(computer_points_temp - player_points_temp);
    if (computer_points_temp < player_points_temp)
    {
        computer_score += total_amount_temp;
        event_text.setPosition(648.0f * scaleX, 20.0f * scaleY);
        event_text.setString("Computer win the round");
    }
    else if (computer_points_temp > player_points_temp)
    {
        player_score += total_amount_temp;
        event_text.setPosition(730.0f * scaleX, 20.0f * scaleY);
        event_text.setString("You win the round");
    }
    else
    {
        event_text.setPosition(890.0f * scaleX, 20.0f * scaleY);
        event_text.setString("Draw");
    }
    reload_score();

    if (computer_score >= max_score)
    {
        chooseMode(Mode::Table);
        event_text.setPosition(688.0f * scaleX, 412.0f * scaleY);
        event_text.setString("Computer is a winner");
    }
    else if (player_score >= max_score)
    {
        chooseMode(Mode::Table);
        event_text.setPosition(770.0f * scaleX, 412.0f * scaleY);
        event_text.setString("You is a winner");
    }

    showEvent = true;
    end_of_round.play();
}

void Score::set(int new_max_score)
{
    max_score = new_max_score;
}

void Score::draw(sf::RenderWindow& window)
{
    if (state == Score::State::Inactive)
        return;

    if (current_mode == Mode::Strip)
        window.draw(strip);
    else if (current_mode == Mode::Table)
        window.draw(table);

    if (showEvent)
        window.draw(event_text);
    window.draw(computer_score_text);
    window.draw(player_score_text);
}

void Score::reload_score()
{
    if (computer_score >= position_regulator)
    {
        computer_score_text.move(-(26.0f * scaleX), 0.0f);
        position_regulator *= 10;
    }
    computer_score_text.setString(std::to_string(computer_score));
    player_score_text.setString(std::to_string(player_score));
}

void Score::hideEvent()
{
    showEvent = false;
}

void Score::chooseMode(Mode mode)
{
    if (mode == Mode::Strip)
    {
        current_mode = Mode::Strip;

        computer_score_text.setPosition(1738.0f * scaleX, 30.0f * scaleY);
        player_score_text.setPosition(150.0f * scaleX, 30.0f * scaleY);
    }
    else if (mode == Mode::Table)
    {
        current_mode = Mode::Table;

        computer_score_text.setPosition(1135.0f * scaleX, 606.0f * scaleY);
        player_score_text.setPosition(757.0f * scaleX, 606.0f * scaleY);

        bool continue_cycle{ true };
        position_regulator = 10;
        while (continue_cycle)
        {
            continue_cycle = false;

            if (position_regulator <= computer_score)
            {
                computer_score_text.move(-(13.0f * scaleX), 0.0f);
                continue_cycle = true;
            }
            if (position_regulator <= player_score)
            {
                player_score_text.move(-(13.0f * scaleX), 0.0f);
                continue_cycle = true;
            }
            position_regulator *= 10;
        }
    }
}

void Score::reset()
{
    computer_score = 0;
    player_score = 0;

    position_regulator = 10;
    showEvent = false;

    chooseMode(Mode::Strip);
    reload_score();
}