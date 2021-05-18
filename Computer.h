#pragma once
#include <array>
#include <chrono>
#include <thread>
#include "Reloader.h"
#include "hand.h"
#include "Boneyard.h"
#include "Table.h"
#include "Score.h"

class Computer
{
public:
    friend class Reloader;

    Computer(Hand& hand);

    void wait();
    bool turn();
    bool show(sf::RenderWindow& window, bool show_all);
    void take(int index);
    int pass();
    bool isEmpty();
    void wakeUp();
    bool isWokeUp();
    void sleep();
    int choose(int range);
    bool choose();

private:
    enum class State
    {
        Active,
        Inactive
    };

    void leave();

    int passIN{ -1 };
    bool isFish{ false };
    bool let_end{ false };
    bool let_in{ false };
    bool first{ true };
    std::reference_wrapper<Hand> hand_ref;
    State state{ State::Inactive };
};