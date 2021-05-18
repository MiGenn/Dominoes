#include "Boneyard.h"

Boneyard::Boneyard(std::array<Chip, 28u>& chips, Container& container) :
    chips_ref(chips)
{
    startX = (modeX - (7.0f * Chip::getWidth())) / 2.0f;
    startY = (modeY / 2.0f) - Chip::getHeight();

    cover.setTexture(container.special_chips_tex);
    cover.setScale(scaleX, scaleY);
    cover.setTextureRect(sf::IntRect(0, 0, 60, 120));
    boneyard.setTexture(container.boneyard_tex);
    boneyard.setScale(scaleX, scaleY);
    set();
}

int Boneyard::size()
{
    return indexes.size();
}

void Boneyard::show()
{
    state = State::Active;
}

void Boneyard::hide()
{
    state = State::Inactive;
}

bool Boneyard::isActive()
{
    return state == State::Active;
}

bool Boneyard::isEmpty()
{
    if (indexes.empty())
    {
        state == State::Inactive;
        return true;
    }      

    return false;
}

bool Boneyard::intersects(sf::Vector2i mousePos)
{
    for (int in : indexes)
        if (chips_ref.get()[in].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            passIN = in;
            return true;
        }   

    return false;
}

int Boneyard::pass(int index)
{
    int temp{ indexes[index] };
    indexes.erase(indexes.begin() + index);

    return temp;
}

void Boneyard::set()
{
    indexes.reserve(14);
    for (int i{ 0 }, j{ 0 }; j < 14; ++i)
    {
        if (chips_ref.get()[i].state == Chip::State::Active)
        {
            chips_ref.get()[i].state = Chip::State::Inactive;
            indexes.push_back(i);
            ++j;
        }
    }

    int count{ 0 };
    float next_row{ 0.0f };
    for (int in : indexes)
    {
        if (count == 7)
        {
            next_row = Chip::getHeight();
            count = 0;
        }
        chips_ref.get()[in].sprite.setPosition(startX + (Chip::getWidth() * (float)count), startY + next_row);
        ++count;
    }
}

void Boneyard::leave()
{
    for (int in : indexes)
        chips_ref.get()[in].state = Chip::State::Active;

    indexes.clear();
}

int Boneyard::pass()
{
    for (int i{ 0 }; i < indexes.size(); ++i)
        if (indexes[i] == passIN)
            indexes.erase(indexes.begin() + i);

    int index{ passIN };
    passIN = -1;

    return index;
}

void Boneyard::draw(sf::RenderWindow& window)
{
    if (!isActive())
        return;

    window.draw(boneyard);
    for (int element : indexes)
    {
        cover.setPosition(chips_ref.get()[element].sprite.getPosition());
        window.draw(cover);
    }
}