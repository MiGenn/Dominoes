#include "Menu.h"

Menu::Menu(Container& container) :
    buttons{ (container.font), (container.font), (container.font), (container.font), (container.font), }
{
    textures[(int)Mode::Main_menu].setTexture(container.main_menu_tex);
    textures[(int)Mode::Main_menu].setScale(scaleX, scaleY);
    textures[(int)Mode::Esc_menu].setTexture(container.esc_menu_tex);
    textures[(int)Mode::Esc_menu].setScale(scaleX, scaleY);
    score_text.setFont(container.font);
    score_text.setFillColor(sf::Color::White);
    score_text.setCharacterSize((unsigned)((float)46u * scaleX));

    sounds[Button_choosed].setBuffer(container.button_choosed_sound);
    sounds[Menu_opended].setBuffer(container.menu_opened_sound);

    buttons[(int)Button::Set_score].text.setString("Set score");
    buttons[(int)Button::Play].text.setString("Play");
    buttons[(int)Button::Exit].text.setString("Exit");
    buttons[(int)Button::Exit_to_menu].text.setString("Exit to menu");
    buttons[(int)Button::Restart].text.setString("Restart");
}

Menu::Mode Menu::getMode()
{
    return current_mode;
}

int Menu::getScore()
{
    int count{ (int)new_score.size() };
    int temp{ 1 };
    while (count > 1)
    {
        --count;
        temp *= 10;
    }

    count = temp;
    int result{ 0 };
    for (char element : new_score)
    {
        result += ((int)element - (int)'0') * count;
        count /= 10;
    }

    new_score.clear();
    score_text.setString("");
    score_text.setPosition(958.0f * scaleX, 448.0f * scaleY);
    return result;
}

bool Menu::isZeroScore()
{
    if (!new_score.size())
        return true;
    return false;
}

bool Menu::isScoreMode()
{
    return deactivate_buttons;
}

void Menu::scoreMode(bool state)
{
    if (current_mode == Mode::Main_menu)
    {
        score_text.setPosition(958.0f * scaleX, 448.0f * scaleY);
        if (state)
        {
            buttons[(int)Button::Set_score].text.setString("Enter score");
            buttons[(int)Button::Set_score].text.setPosition(838.0f * scaleX, 448.0f * scaleY);
            deactivate_buttons = true;
        }
        else
        {
            buttons[(int)Button::Set_score].text.setString("Set score");
            buttons[(int)Button::Set_score].text.setPosition(862.0f * scaleX, 448.0f * scaleY);
            deactivate_buttons = false;
        }
    }
    else if (current_mode == Mode::Winnig_menu)
    {
        score_text.setPosition(958.0f * scaleX, 488.0f * scaleY);
        if (state)
        {
            buttons[(int)Button::Set_score].text.setString("Enter score");
            buttons[(int)Button::Set_score].text.setPosition(838.0f * scaleX, 488.0f * scaleY);
            deactivate_buttons = true;
        }
        else
        {
            buttons[(int)Button::Set_score].text.setString("Set score");
            buttons[(int)Button::Set_score].text.setPosition(862.0f * scaleX, 488.0f * scaleY);
            deactivate_buttons = false;
        }
    }
}

void Menu::eraseLast()
{
    if (!new_score.size())
        return;
    new_score.pop_back();
    score_text.setString(new_score);
    score_text.move(13.0f * scaleX, 0.0f);
}

void Menu::setScore(char symbol)
{
    if (((int)symbol < 48) || ((int)symbol > 57))
        return;
    if (new_score.size() == 7)
        return;

    new_score.push_back(symbol);
    if (new_score[0] == '0')
    {
        new_score.clear();
        return;
    }

    buttons[(int)Button::Set_score].text.setString("");
    score_text.setString(new_score);
    score_text.move(-(13.0f * scaleX), 0.0f);
}

void Menu::toggle()
{
    Menu::Mode temp{ current_mode };
    chooseMode(previous_mode);
    previous_mode = temp;
}

void Menu::contain(sf::Vector2i& mousePos)
{
    if (deactivate_buttons)
        return;

    static int choosed{ -1 };
    for (int i{ 0 }; i < 5; ++i)
        if ((buttons[i].state == MenuButton::State::Active) &&
            (buttons[i].text.getGlobalBounds().contains(mousePos.x, mousePos.y)))
        {
            if (i == choosed)
                return;
            if (choosed > -1)
                buttons[choosed].text.setFillColor(sf::Color::White);

            choosed = i;
            buttons[i].text.setFillColor(sf::Color(255, 255, 255, 128));
            sounds[Button_choosed].play();
            return;
        }

    if (choosed < 0)
        return;
    buttons[choosed].text.setFillColor(sf::Color::White);
    choosed = -1;
}

bool Menu::pressed(sf::Vector2i& mousePos)
{
    if (deactivate_buttons)
        return false;

    for (int i{ 0 }; i < 5; ++i)
        if ((buttons[i].state == MenuButton::State::Active) &&
            (buttons[i].text.getGlobalBounds().contains(mousePos.x, mousePos.y)))
        {
            was_pressed = (Button)i;
            return true;
        }

    return false;
}

const Menu::Button& Menu::wasPressed()
{
    return was_pressed;
}

void Menu::chooseMode(Mode mode)
{
    if (mode == Mode::Off)
    {
        current_mode = Mode::Off;

        buttons[(int)Button::Restart].state = MenuButton::State::Inactive;
        buttons[(int)Button::Exit_to_menu].state = MenuButton::State::Inactive;
        buttons[(int)Button::Set_score].state = MenuButton::State::Inactive;
        buttons[(int)Button::Play].state = MenuButton::State::Inactive;
        buttons[(int)Button::Exit].state = MenuButton::State::Inactive;
    }
    else if (mode == Mode::Main_menu)
    {
        current_mode = Mode::Main_menu;
        deactivate_buttons = false;

        buttons[(int)Button::Restart].state = MenuButton::State::Inactive;
        buttons[(int)Button::Exit_to_menu].state = MenuButton::State::Inactive;

        buttons[(int)Button::Set_score].state = MenuButton::State::Active;
        buttons[(int)Button::Set_score].text.setPosition(862.0f * scaleX, 448.0f * scaleY);
        buttons[(int)Button::Play].state = MenuButton::State::Active;
        buttons[(int)Button::Play].text.setPosition(914.0f * scaleX, 508.0f * scaleY);
        buttons[(int)Button::Exit].state = MenuButton::State::Active;
        buttons[(int)Button::Exit].text.setPosition(918.0f * scaleX, 570.0f * scaleY);
    }
    else if (mode == Mode::Winnig_menu)
    {
        current_mode = Mode::Winnig_menu;
        deactivate_buttons = false;

        buttons[(int)Button::Play].state = MenuButton::State::Inactive;
        buttons[(int)Button::Exit_to_menu].state = MenuButton::State::Inactive;

        buttons[(int)Button::Set_score].state = MenuButton::State::Active;
        buttons[(int)Button::Set_score].text.setPosition(862.0f * scaleX, 488.0f * scaleY);
        buttons[(int)Button::Restart].state = MenuButton::State::Active;
        buttons[(int)Button::Restart].text.setPosition(882.0f * scaleX, 546.0f * scaleY);
        buttons[(int)Button::Exit].state = MenuButton::State::Active;
        buttons[(int)Button::Exit].text.setPosition(918.0f * scaleX, 606.0f * scaleY);
    }
    else if (mode == Mode::Esc_menu)
    {
        current_mode = Mode::Esc_menu;
        deactivate_buttons = false;

        buttons[(int)Button::Set_score].state = MenuButton::State::Inactive;
        buttons[(int)Button::Play].state = MenuButton::State::Inactive;
        buttons[(int)Button::Exit].state = MenuButton::State::Inactive;

        buttons[(int)Button::Restart].state = MenuButton::State::Active;
        buttons[(int)Button::Restart].text.setPosition(884.0f * scaleX, 482.0f * scaleY);
        buttons[(int)Button::Exit_to_menu].state = MenuButton::State::Active;
        buttons[(int)Button::Exit_to_menu].text.setPosition(824.0f * scaleX, 540.0f * scaleY);
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    if (current_mode == Mode::Main_menu)
        window.draw(textures[(int)Mode::Main_menu]);
    else if (current_mode == Mode::Winnig_menu)
        ;
    else if (current_mode == Mode::Esc_menu)
        window.draw(textures[(int)Mode::Esc_menu]);
    else if (current_mode == Mode::Off)
        return;

    for (MenuButton& element : buttons)
        if (element.state == MenuButton::State::Active)
            window.draw(element.text);

    if (deactivate_buttons)
        window.draw(score_text);
}

void Menu::esc_sound()
{
    sounds[Menu_opended].play();
}