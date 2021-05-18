#include "Table.h"

Table::Table(int start_index, std::array<Chip, 28u>& chips, Container& container) :
    chips_ref(chips), helpers{ (container.helper_tex),(container.helper_tex) }
{
    place_chip.setBuffer(container.place_chip_sound);

    set(start_index);
}

void Table::show()
{
    state = State::Active;
}

void Table::hide()
{
    state = State::Inactive;
}

bool Table::findSuitable(Hand& hand)
{
    if (computeIsFirst)
    {
        computeIsFirst = false;
        return true;
    }

    bool is{ false };
    for (int in : hand.indexes)
        for (Helper& element : helpers)
        {
            if ((chips_ref.get()[in].getBottomV() == element.v) ||
                (chips_ref.get()[in].getTopV() == element.v))
            {
                hand.chips_ref.get()[in].state = Chip::State::Active;
                is = true;
                break;
            }
            else
                hand.chips_ref.get()[in].state = Chip::State::Inactive;
        }

    return is;
}

int Table::find_best_chip(Hand& hand_1, Hand& hand_2, Computer& computer)
{
    ///// Find biggest double /////
    int biggestV_1{ -1 };
    int bestIN_1{ -1 };
    for (int i{ 0 }; i < 7; ++i)
        if (hand_1.chips_ref.get()[hand_1.indexes[i]].isDouble())
            if (hand_1.chips_ref.get()[hand_1.indexes[i]].topV > biggestV_1)
            {
                biggestV_1 = hand_1.chips_ref.get()[hand_1.indexes[i]].topV;
                bestIN_1 = hand_1.indexes[i];
            }

    int biggestV_2{ -1 };
    int bestIN_2{ -1 };
    for (int i{ 0 }; i < 7; ++i)
        if (hand_2.chips_ref.get()[hand_2.indexes[i]].isDouble())
            if (hand_2.chips_ref.get()[hand_2.indexes[i]].topV > biggestV_2)
            {
                biggestV_2 = hand_2.chips_ref.get()[hand_2.indexes[i]].topV;
                bestIN_2 = hand_2.indexes[i];
            }

    if (biggestV_1 > biggestV_2)
    {
        if (hand_1.who == Hand::Who::Computer)
        {
            computer.wakeUp();
            computeIsFirst = true;
        }
        hand_1.chips_ref.get()[bestIN_1].state = Chip::State::Active;

        return bestIN_1;
    }
    else if (biggestV_1 < biggestV_2)
    {
        if (hand_2.who == Hand::Who::Computer)
        {
            computer.wakeUp();
            computeIsFirst = true;
        }
        hand_2.chips_ref.get()[bestIN_2].state = Chip::State::Active;

        return bestIN_2;
    }
       

    ///// Find biggest chip, if biggest double not found /////
    if (biggestV_1 < 0)
        for (int i{ 0 }; i < 7; ++i)
            if (hand_1.chips_ref.get()[hand_1.indexes[i]].getV() > biggestV_1)
            {
                biggestV_1 = hand_1.chips_ref.get()[hand_1.indexes[i]].getV();
                bestIN_1 = hand_1.indexes[i];
            }

    if (biggestV_2 < 0)
        for (int i{ 0 }; i < 7; ++i)
            if (hand_1.chips_ref.get()[hand_1.indexes[i]].getV() > biggestV_2)
            {
                biggestV_2 = hand_1.chips_ref.get()[hand_1.indexes[i]].getV();
                bestIN_2 = hand_1.indexes[i];
            }

    if (biggestV_1 > biggestV_2)
    {        
        if (hand_1.who == Hand::Who::Computer)
        {
            computer.wakeUp();
            computeIsFirst = true;
        }
        hand_1.chips_ref.get()[bestIN_1].state = Chip::State::Active;

        return bestIN_1;
    }
    else
    {
        if (hand_2.who == Hand::Who::Computer)
        {
            computer.wakeUp();
            computeIsFirst = true;
        }
        hand_2.chips_ref.get()[bestIN_2].state = Chip::State::Active;

        return bestIN_2;
    }
}

bool Table::isSuitable(int index)
{
    for (int i{ 0 }; i < (int)helpers.size(); ++i)
        if (((chips_ref.get()[index].getBottomV() == helpers[i].v) ||
            (chips_ref.get()[index].getTopV() == helpers[i].v)) &&
            helpers[i].parts[i].getGlobalBounds().intersects(chips_ref.get()[index].sprite.getGlobalBounds()))
        {
            suitable = i;
            player_put = true;
            return true;
        }

    return false;
}

void Table::draw(sf::RenderWindow& window)
{
    if (state == Table::State::Inactive)
        return;

    for (int element : indexes)
        window.draw(chips_ref.get()[element].sprite);
    for (Helper& element : helpers)
        window.draw(element.parts[(int)element.current_part]);
}

void Table::set(int start_index)
{
    if (chips_ref.get()[start_index].isDouble())
    {
        startX = (modeX - Chip::getWidth()) / 2.0f;
        startY = (modeY - Chip::getHeight()) / 2.0f;
        helpers[First].setRotation(0.0f);
        helpers[First].isVertical = true;
        helpers[Second].setRotation(0.0f);
        helpers[Second].isVertical = true;
    }
    else
    {
        startX = (modeX + Chip::getHeight()) / 2.0f;
        startY = (modeY - Chip::getWidth()) / 2.0f;
        helpers[First].setRotation(90.0f);
        helpers[First].isVertical = false;
        helpers[Second].setRotation(90.0f);
        helpers[Second].isVertical = false;
    }

    helpers[First].setPosition(startX, startY);
    helpers[First].v = chips_ref.get()[start_index].getTopV();
    helpers[First].way = Helper::Way::Left;
    helpers[First].current_part = Helper::Part::Center;
    helpers[Second].setPosition(startX, startY);
    helpers[Second].v = chips_ref.get()[start_index].getBottomV();
    helpers[Second].way = Helper::Way::Right;
    helpers[Second].current_part = Helper::Part::Center;
}

void Table::leave()
{
    move_all_up = -1;
    first_turn = true;
    computeIsFirst = false;
    helpers[First].amount = 0;
    helpers[First].was_first_transition = false;
    helpers[Second].amount = 0;
    helpers[Second].was_first_transition = false;

    for (int in : indexes)
    {
        chips_ref.get()[in].state = Chip::State::Active;
        chips_ref.get()[in].sprite.setRotation(0.0f);
    }

    indexes.clear();
}

void Table::place(int index)
{
    if (player_put)
        ;
    else
    {
        for (; iterator < (int)helpers.size(); ++iterator)
            if ((chips_ref.get()[index].getBottomV() == helpers[iterator].v) ||
                (chips_ref.get()[index].getTopV() == helpers[iterator].v))
            {
                suitable = iterator;
                break;
            }
        iterator = 0;
    }

    if (helpers[suitable].way == Helper::Way::Left)
    {
        if (helpers[suitable].isVertical)
        {
            if (chips_ref.get()[index].isDouble())
            {
                chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                // Make horizontal from vertical with offset
                helpers[suitable].setRotation(90.0f);
                helpers[suitable].move(0.0f, (Chip::getHeight() - Chip::getWidth()) / 2.0f);
                helpers[suitable].isVertical = false;

                helpers[suitable].v = chips_ref.get()[index].getTopV();
            }
            else
            {
                if (helpers[suitable].v == chips_ref.get()[index].getTopV())
                {
                    chips_ref.get()[index].sprite.setRotation(90.0f);

                    // Make horizontal from vertical without offset
                    helpers[suitable].setRotation(90.0f);
                    helpers[suitable].move(Chip::getWidth(), (Chip::getHeight() - Chip::getWidth()) / 2.0f);

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(-(Chip::getHeight() + Chip::getWidth()), -(Chip::getHeight() - Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getBottomV();
                }
                else
                {
                    chips_ref.get()[index].sprite.setRotation(270.0f);

                    // Make inverted horizontal from vertical without offset
                    helpers[suitable].setRotation(270.0f);
                    helpers[suitable].move(-(Chip::getHeight() - Chip::getWidth()), (Chip::getHeight() + Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = false;

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from inverted horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(-Chip::getWidth(), -(Chip::getHeight() + Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getTopV();
                }
            }
        }
        else
        {
            if (chips_ref.get()[index].isDouble()) // Set double (This case doesn't exist)
                ;
            else // Set non-Double
            {
                if (helpers[suitable].v == chips_ref.get()[index].getTopV())
                {
                    chips_ref.get()[index].sprite.setRotation(90.0f);
                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(-(Chip::getHeight() + Chip::getWidth()), -(Chip::getHeight() - Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getBottomV();
                }
                else
                {
                    chips_ref.get()[index].sprite.setRotation(270.0f);

                    // Make inverted horizontal from horizontal without offset
                    helpers[suitable].setRotation(270.0f);
                    helpers[suitable].move(-Chip::getHeight(), Chip::getWidth());
                    helpers[suitable].isVertical = false;

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from inverted horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(-Chip::getWidth(), -(Chip::getHeight() + Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getTopV();
                }
            }
        }
    }
    else if (helpers[suitable].way == Helper::Way::Right)
    {
        if (helpers[suitable].isVertical)
        {
            if (chips_ref.get()[index].isDouble()) // Set double
            {
                chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                // Make horizontal from vertical with offset
                helpers[suitable].setRotation(90.0f);
                helpers[suitable].move(Chip::getHeight() + Chip::getWidth(), (Chip::getHeight() - Chip::getWidth()) / 2.0f);
                helpers[suitable].isVertical = false;

                helpers[suitable].v = chips_ref.get()[index].getTopV();
            }
            else // Set non-Double
            {
                if (helpers[suitable].v == chips_ref.get()[index].getTopV())
                {
                    chips_ref.get()[index].sprite.setRotation(270.0f);

                    // Make inverted horizontal from vertical without offset
                    helpers[suitable].setRotation(270.0f);
                    helpers[suitable].move(0.0f, (Chip::getHeight() + Chip::getWidth()) / 2.0f);

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from inverted horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(Chip::getHeight(), -(Chip::getHeight() + Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getBottomV();
                }
                else
                {
                    chips_ref.get()[index].sprite.setRotation(90.0f);

                    // Make horizontal from vertical without offset
                    helpers[suitable].setRotation(90.0f);
                    helpers[suitable].move(Chip::getHeight(), (Chip::getHeight() - Chip::getWidth()) / 2.0f);

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(0.0f, -(Chip::getHeight() - Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getTopV();
                }
            }
        }
        else
        {
            if (chips_ref.get()[index].isDouble()) // Set double (This case doesn't exist)
                ;
            else // Set non-Double
            {
                if (helpers[suitable].v == chips_ref.get()[index].getTopV())
                {
                    chips_ref.get()[index].sprite.setRotation(270.0f);

                    // Make inverted horizontal from horizontal without offset
                    helpers[suitable].setRotation(270.0f);
                    helpers[suitable].move(-Chip::getHeight(), Chip::getWidth());

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from inverted horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(Chip::getHeight(), -(Chip::getHeight() + Chip::getWidth()) / 2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getBottomV();
                }
                else
                {
                    chips_ref.get()[index].sprite.setRotation(90.0f);
                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make vertical from horizontal with offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(0.0f, (Chip::getHeight() - Chip::getWidth()) / -2.0f);
                    helpers[suitable].isVertical = true;

                    helpers[suitable].v = chips_ref.get()[index].getTopV();
                }
            }
        }
    }
    else if (helpers[suitable].way == Helper::Way::Down)
    {
        if (helpers[suitable].isVertical)
        {
            if (chips_ref.get()[index].isDouble()) // Set double (This case doesn't exist)
                ;
            else // Set non-Double
            {
                if (helpers[suitable].v == chips_ref.get()[index].getTopV())
                {
                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make horizontal from vertical with offset
                    helpers[suitable].setRotation(90.0f);
                    helpers[suitable].move((Chip::getHeight() + Chip::getWidth()) / 2.0f, Chip::getHeight());
                    helpers[suitable].isVertical = false;

                    helpers[suitable].v = chips_ref.get()[index].getBottomV();
                }
                else
                {
                    chips_ref.get()[index].sprite.setRotation(180.0f);

                    // Make inverted vertical from vertical without offset
                    helpers[suitable].setRotation(180.0f);
                    helpers[suitable].move(Chip::getWidth(), Chip::getHeight());

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make horizontal from inverted vertical with offset
                    helpers[suitable].setRotation(90.0f);
                    helpers[suitable].move((Chip::getHeight() - Chip::getWidth()) / 2.0f, 0.0f);
                    helpers[suitable].isVertical = false;

                    helpers[suitable].v = chips_ref.get()[index].getTopV();
                }
            }
        }
        else
        {
            if (chips_ref.get()[index].isDouble()) // Set double
            {
                chips_ref.get()[index].sprite.setRotation(90.0f);
                chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                // Make vertical from horizontal with offset
                helpers[suitable].setRotation(0.0f);
                helpers[suitable].move(-(Chip::getHeight() + Chip::getWidth()) / 2.0f, Chip::getWidth());
                helpers[suitable].isVertical = true;

                helpers[suitable].v = chips_ref.get()[index].getTopV();
            }
            else // Set non-Double
            {
                if (helpers[suitable].v == chips_ref.get()[index].getTopV())
                {
                    // Make vertical from horizontal without offset
                    helpers[suitable].setRotation(0.0f);
                    helpers[suitable].move(-(Chip::getHeight() + Chip::getWidth()) / 2.0f, 0.0f);

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make horizontal from vertical with offset
                    helpers[suitable].setRotation(90.0f);
                    helpers[suitable].move((Chip::getHeight() + Chip::getWidth()) / 2.0f, Chip::getHeight());
                    helpers[suitable].isVertical = false;

                    helpers[suitable].v = chips_ref.get()[index].getBottomV();
                }
                else
                {
                    chips_ref.get()[index].sprite.setRotation(180.0f);

                    // Make inverted vertical from horizontal without offset
                    helpers[suitable].setRotation(180.0f);
                    helpers[suitable].move(-(Chip::getHeight() - Chip::getWidth()) / 2.0f, Chip::getHeight());

                    chips_ref.get()[index].sprite.setPosition(helpers[suitable].getPosition());

                    // Make horizontal from inverted vertical with offset
                    helpers[suitable].setRotation(90.0f);
                    helpers[suitable].move((Chip::getHeight() - Chip::getWidth()) / 2.0f, 0.0f);
                    helpers[suitable].isVertical = false;

                    helpers[suitable].v = chips_ref.get()[index].getTopV();
                }
            }
        }
    }
    ++helpers[suitable].amount;

    ///// Transition: down
    if (helpers[suitable].amount > 7)
    {
        ++move_all_up;
        helpers[suitable].was_first_transition = true;
        helpers[suitable].amount = 0;
        if (helpers[suitable].isVertical)
        {
            helpers[suitable].setRotation(90.0f);
            if (helpers[suitable].way == Helper::Way::Left)
                helpers[suitable].move(((Chip::getHeight() - Chip::getWidth()) / 2.0f) + Chip::getHeight(), (Chip::getHeight() + Chip::getWidth()) / 2.0f);
            else
                helpers[suitable].move((Chip::getHeight() - Chip::getWidth()) / 2.0f, (Chip::getHeight() + Chip::getWidth()) / 2.0f);
            helpers[suitable].isVertical = false;
        }
        else
        {
            helpers[suitable].setRotation(0.0f);
            if (helpers[suitable].way == Helper::Way::Left)
                helpers[suitable].move(0.0f, (Chip::getHeight() + Chip::getWidth()) / 2.0f);
            else
                helpers[suitable].move(-(Chip::getHeight() + Chip::getWidth()), (Chip::getHeight() + Chip::getWidth()) / 2.0f);
            helpers[suitable].isVertical = true;
        }
        helpers[suitable].way = Helper::Way::Down;
    }
    ///// Transition: return and inverse way
    else if ((helpers[suitable].amount > 4) && 
        helpers[suitable].was_first_transition)
    {
        helpers[suitable].amount = -100;
        if (helpers[suitable].isVertical)
        {
            helpers[suitable].setRotation(90.0f);
            if (suitable == Second)
            {
                helpers[suitable].move(-((Chip::getHeight() - Chip::getWidth()) / 2.0f), -Chip::getWidth());
                helpers[suitable].way = Helper::Way::Left;
            }
            else
            {
                helpers[suitable].move(((Chip::getHeight() + Chip::getWidth()) / 2.0f) + Chip::getHeight(), -Chip::getWidth());
                helpers[suitable].way = Helper::Way::Right;
            }
            helpers[suitable].isVertical = false;
        }
        else
        {
            helpers[suitable].setRotation(0.0f);
            if (suitable == Second)
            {
                helpers[suitable].move(-(Chip::getHeight() + ((Chip::getHeight() - Chip::getWidth()) / 2.0f)), -(Chip::getHeight() - ((Chip::getHeight() - Chip::getWidth()) / 2.0f)));
                helpers[suitable].way = Helper::Way::Left;
            }
            else
            {
                helpers[suitable].move(-((Chip::getHeight() - Chip::getWidth()) / 2.0f), -(Chip::getHeight() - ((Chip::getHeight() - Chip::getWidth()) / 2.0f)));
                helpers[suitable].way = Helper::Way::Right;
            }
            helpers[suitable].isVertical = true;
        }
    }
    helpers[suitable].changePart();

    if (first_turn)
    {
        first_turn = false;
        suitable = 1;
        iterator = 1;
        place(index);
        return;
    }

    player_put = false;
    indexes.push_back(index);
    suitable = 0;

    ///// Move all chips up
    if (!move_all_up)
    {
        ++move_all_up;
        for (int in : indexes)
            chips_ref.get()[in].sprite.move(0.0f, -modeY / 4.0f);
        for (Helper& element : helpers)
            element.move(0.0f, -modeY / 4.0f);
    }

    place_chip.play();
}