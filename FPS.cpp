#include "FPS.h"

FPS::FPS(Container& container)
{
    framerate.setFont(container.font);
    framerate.setCharacterSize((unsigned)((float)32u * scaleX));
    framerate.setFillColor(sf::Color(50u, 168u, 56u, 255u));
    framerate.setPosition(0.0f, 1010.0f * scaleY);
    average_fps.setFont(container.font);
    average_fps.setCharacterSize((unsigned)((float)32u * scaleX));
    average_fps.setFillColor(sf::Color(50u, 168u, 56u, 255u));
    average_fps.setPosition(0.0f, (1015.0f + ((float)25 * scaleY)) * scaleY);
}

void FPS::start()
{
    clock.restart();
}

void FPS::compute()
{
    elapsedTime = clock.getElapsedTime().asSeconds();
    time += elapsedTime;
    clock.restart();
    if (time < interval)
        return;
    fps = (int)(1.0f / elapsedTime);
    ++counter;
    amount += fps;
    framerate.setString(std::to_string(fps) + " fps");
    average_fps.setString(std::to_string(amount / counter) + " avg");
    time = 0.0f;
}

void FPS::draw(sf::RenderWindow& window)
{
    window.draw(framerate);
    window.draw(average_fps);
}