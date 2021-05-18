#include "Hand.h"

Hand::Hand(std::array<Chip, 28u>& chips, Container& container, Who who) :
	chips_ref(chips), who(who)
{
    take_chip.setBuffer(container.place_chip_sound);
    cover.setTexture(container.special_chips_tex);
    cover.setScale(scaleX, scaleY);
    if (who == Who::Computer)
    {
        startY = Chip::getHeight() / 2.0f;
        cover.setTextureRect(sf::IntRect(0, 0, 60, 120));
    }    
    else
    {
        startY = modeY - Chip::getHeight();
        cover.setTextureRect(sf::IntRect(60, 0, 60, 120));
    }

    set();
}

void Hand::draw(sf::RenderWindow& window)
{
    if (state == Hand::State::Inactive)
        return;

    if (who == Who::Player)
    {
        for (int element : indexes)
        {
            window.draw(chips_ref.get()[element].sprite);
            if ((chips_ref.get()[element].state == Chip::State::Inactive) && (!show_all))
            {
                cover.setPosition(chips_ref.get()[element].sprite.getPosition());
                window.draw(cover);
            }
        }
    }
    else
    {
        for (int element : indexes)
        {
            if (!show_all)
            {
                cover.setPosition(chips_ref.get()[element].sprite.getPosition());
                window.draw(cover);
            }
            else
                window.draw(chips_ref.get()[element].sprite);
        }
    }
}

bool Hand::isEmpty()
{
    if (indexes.empty())
        return true;
    else
        return false;
}

void Hand::coverAll()
{
    for (int index : indexes)
        chips_ref.get()[index].state = Chip::State::Inactive;
}

void Hand::show()
{
    state = State::Active;
}

void Hand::hide()
{
    state = State::Inactive;
}

void Hand::take(int index)
{
    indexes.push_back(index);
    sort();
    take_chip.play();
}

bool Hand::intersects(sf::Vector2i& mousePos)
{
    for (int in : indexes)
        if ((chips_ref.get()[in].state == Chip::State::Active) &&
            chips_ref.get()[in].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            moving = in;
            return true;
        }

    return false;
}

void Hand::set()
{
    indexes.reserve(7);
    for (int i{ 0 }, j{ 0 }; j < 7; ++i)
    {
        if (chips_ref.get()[i].state == Chip::State::Active)
        {
            chips_ref.get()[i].state = Chip::State::Inactive;
            indexes.push_back(i);
            ++j;
        }
    }

    int count{ 0 };
    for (int in : indexes)
    {
        chips_ref.get()[in].sprite.setPosition(startX + (Chip::getWidth() * count), startY);
        ++count;
    }
}

void Hand::leave()
{
    for (int in : indexes)
        chips_ref.get()[in].state = Chip::State::Active;
    show_all = false;

    indexes.clear();
}

int Hand::pass(int index)
{
    for (int i{ 0 }; i < (int)indexes.size(); ++i)
        if (indexes[i] == index)
            indexes.erase(indexes.begin() + i);
    sort();

    return index;
}

int Hand::pass()
{
    int temp = moving;
    moving = -1;

    return temp;
}

void Hand::sort()
{
    ///// Move all chips to each other in the top left corner
    int count{ 0 };
    for (int in : indexes)
    {
        chips_ref.get()[in].sprite.setPosition(0.0f + (Chip::getWidth() * count), startY);
        ++count;
    }

    ///// Set group of chips in the center of the x-axis
    for (int in : indexes)
        chips_ref.get()[in].sprite.move((modeX / 2.0f) - ((Chip::getWidth() / 2.0f) * count), 0.0f);
}