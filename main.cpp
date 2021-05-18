#include "Core.h"

int main()
{
    using namespace sf;
    srand((unsigned)time(NULL));

    Container container;
    RenderWindow window(vMode, "Domino", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    Menu menu(container);
    menu.chooseMode(Menu::Mode::Main_menu);

    std::array<Chip, 28u> chips
    {
            Chip(0, 0),
            Chip(0, 1),
            Chip(0, 2),
            Chip(0, 3),
            Chip(0, 4),
            Chip(0, 5),
            Chip(0, 6),
            Chip(1, 1),
            Chip(1, 2),
            Chip(1, 3),
            Chip(1, 4),
            Chip(1, 5),
            Chip(1, 6),
            Chip(2, 2),
            Chip(2, 3),
            Chip(2, 4),
            Chip(2, 5),
            Chip(2, 6),
            Chip(3, 3),
            Chip(3, 4),
            Chip(3, 5),
            Chip(3, 6),
            Chip(4, 4),
            Chip(4, 5),
            Chip(4, 6),
            Chip(5, 5),
            Chip(5, 6),
            Chip(6, 6)
    };
    Chip::setTextures(chips, container);
    Chip::shuffle(chips);
    Hand computer_hand(chips, container, Hand::Who::Computer);
    Hand player_hand(chips, container, Hand::Who::Player);
    Boneyard boneyard(chips, container);
    Computer computer(computer_hand);
    Table table(Table::find_best_chip(computer_hand, player_hand, computer), chips, container);
    Score score(computer_hand, player_hand, container);
    Reloader reloader(computer_hand, player_hand, boneyard, table, score, computer, menu);
    Movement movement(chips);
    //FPS fps(container);
    Vector2i mousePos;;

    //fps.start();

    while (window.isOpen())
    {
        ///// Event section /////
        mousePos = Mouse::getPosition(window);
        menu.contain(mousePos);

        Event event;
        while (window.pollEvent(event))
        {
            if (menu.isScoreMode())
            {
                if (event.type == Event::TextEntered)
                {
                    menu.setScore(event.text.unicode);
                    container.set_score.play();
                }
            }
            if (event.type == Event::KeyPressed)
            {
                if (menu.isScoreMode())
                {
                    if (event.key.code == Keyboard::Enter)
                    {
                        if (!menu.isZeroScore())
                        {
                            menu.scoreMode(false);
                            score.set(menu.getScore());
                        }
                    }
                    else if (event.key.code == Keyboard::Backspace)
                        menu.eraseLast();
                    container.set_score.play();
                }
                else if (event.key.code == Keyboard::Escape)
                    if ((menu.getMode() == Menu::Mode::Off) ||
                        (menu.getMode() == Menu::Mode::Esc_menu))
                    {
                        menu.toggle();
                        menu.esc_sound();
                    }
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (menu.pressed(mousePos))
                    {
                        if (menu.wasPressed() == Menu::Button::Set_score)
                            menu.scoreMode(true);
                        else if (menu.wasPressed() == Menu::Button::Play)
                        {
                            menu.chooseMode(Menu::Mode::Esc_menu);
                            menu.toggle();
                            table.show();
                            score.show();
                            computer_hand.show();
                            player_hand.show();
                        }
                        else if (menu.wasPressed() == Menu::Button::Exit)
                            window.close();
                        else if (menu.wasPressed() == Menu::Button::Exit_to_menu)
                        {
                            table.hide();
                            computer_hand.hide();
                            player_hand.hide();
                            boneyard.hide();
                            score.hide();
                            menu.chooseMode(Menu::Mode::Main_menu);
                            reloader.reload(false);
                        }
                        else if (menu.wasPressed() == Menu::Button::Restart)
                        {
                            menu.chooseMode(Menu::Mode::Esc_menu);
                            menu.toggle();
                            boneyard.hide();
                            table.show();
                            score.show();
                            computer_hand.show();
                            player_hand.show();
                            reloader.reload(false);
                        }
                    }
                    else if (boneyard.isActive() &&
                        (menu.getMode() == Menu::Mode::Off))
                    {
                        if (boneyard.intersects(mousePos))
                        {
                            score.hideEvent();
                            player_hand.take(boneyard.pass());
                            if (table.findSuitable(player_hand))
                                boneyard.hide();
                            else if (boneyard.isEmpty())
                            {
                                boneyard.hide();
                                computer.wakeUp();
                            }
                        }
                    }
                    else if (player_hand.intersects(mousePos) && 
                        (menu.getMode() == Menu::Mode::Off) &&
                        (!computer.isWokeUp()))
                        movement.take(player_hand.pass(), mousePos);
                }
            }
            if (event.type == Event::MouseButtonReleased)
            {
                if (movement.isActive() && 
                    table.isSuitable(movement.pass()))
                {
                    score.hideEvent();
                    table.place(player_hand.pass(movement.pass()));
                    movement.leave();
                    player_hand.coverAll();

                    if (player_hand.isEmpty()) // Player win
                    {
                        score.count();
                        reloader.reload(true);
                    }
                    else 
                    {
                        table.findSuitable(computer_hand);
                        computer.wakeUp();
                    }
                }
                else
                    movement.giveBack();
            }
        }
        movement.move(mousePos);

        ///// Computer mind /////
        if ((menu.getMode() == Menu::Mode::Off) &&
            computer.isWokeUp())
        {
            computer.wait();
        }
        if (computer.turn())
        {
            table.findSuitable(computer_hand);
            if (computer.choose())
            {
                if (computer.show(window, false))
                    continue;

                table.place(computer.pass());
                computer.sleep();

                if (computer.isEmpty()) // Computer win
                {
                    score.count();
                    reloader.reload(true);
                }
                else if (table.findSuitable(player_hand))
                    ;
                else
                {
                    if (boneyard.isEmpty()) // Computer make a turn again
                        computer.wakeUp();
                    else
                        boneyard.show();
                }
            }
            else
            {
                if (!boneyard.isEmpty())
                    computer.take(boneyard.pass(computer.choose(boneyard.size())));
                else
                {
                    if (table.findSuitable(player_hand)) // Player make a turn again
                        computer.sleep();
                    else // Fish
                    {
                        if (computer.show(window, true))
                            continue;
                        computer.sleep();
                        score.count();
                        reloader.reload(true);
                    }
                }
            }
        }

        ///// Draw section /////
        window.clear();
        window.draw(container.background);
        table.draw(window);
        computer_hand.draw(window);
        score.draw(window);
        player_hand.draw(window);
        movement.draw(window);
        boneyard.draw(window);
        menu.draw(window);
        //fps.compute();
        //fps.draw(window);

        window.display();
    }
    return 0;
}