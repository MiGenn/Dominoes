#pragma once
#include <SFML/Graphics.hpp>
#include "Container.h"

class FPS
{
public:
    FPS(Container& container);

    void start();
    void compute();
    void draw(sf::RenderWindow& window);

private:
    sf::Text framerate;
    sf::Text average_fps;
    sf::Clock clock;
    float interval{ 1.0f };
    float elapsedTime{ 0.0f };
    float time{ interval };
    unsigned long amount{ 0u };
    unsigned int counter{ 0u };
    int fps{ 0 };
};
