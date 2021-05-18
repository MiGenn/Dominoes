#include "Computer.h"

Computer::Computer(Hand& hand) :
    hand_ref(hand)
{

}

void Computer::wait()
{
    static bool unlocked{ true };
    if (unlocked)
    {
        unlocked = false;
        std::thread thread1([&]()
            {
                if (isFish)
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                else
                    std::this_thread::sleep_for(std::chrono::milliseconds(400));

                let_in = true;
                while (!let_end)
                {

                }
                let_end = false;
                unlocked = true;
            });
        thread1.detach();
    }
}

bool Computer::turn()
{
    if ((state == State::Active) && let_in)
    {
        let_in = false;
        let_end = true;
        return true;
    }
    return false;
}

bool Computer::show(sf::RenderWindow& window, bool all)
{
    if (first)
    {
        if (all)
        {
            isFish = true;
            for (int in : hand_ref.get().indexes)
                hand_ref.get().chips_ref.get()[in].sprite.move(0.0f, Chip::getHeight() / 2);
            hand_ref.get().show_all = true;
        }
        else
            hand_ref.get().chips_ref.get()[passIN].sprite.move(0.0f, Chip::getHeight() / 2);
        first = false;
        return true;
    }
    first = true;
    return false;
}

void Computer::take(int index)
{
    hand_ref.get().take(index);
}

int Computer::pass()
{
    int index{ passIN };
    passIN = -1;

    return hand_ref.get().pass(index);
}

bool Computer::isEmpty()
{
    return hand_ref.get().isEmpty();
}

void Computer::wakeUp()
{
    state = State::Active;   
}

void Computer::sleep()
{
    state = State::Inactive;
}

bool Computer::isWokeUp()
{
    return state == State::Active;
}

int Computer::choose(int range)
{
    return rand() % range;
}

bool Computer::choose()
{
    if (passIN >= 0)
        return true;

    int biggestV{ -1 };
    int bestIN{ -1 };
    for (int in : hand_ref.get().indexes)
        if ((hand_ref.get().chips_ref.get()[in].state == Chip::State::Active) && 
            (hand_ref.get().chips_ref.get()[in].getV() > biggestV))
        {
            biggestV = hand_ref.get().chips_ref.get()[in].getV();
            bestIN = in;
        }

    if (bestIN < 0)
        return false;

    passIN = bestIN;
    return true;
}

void Computer::leave()
{
    passIN = -1;
    isFish = false;
    let_end = false;
    let_in = false;
    first = true;
}
