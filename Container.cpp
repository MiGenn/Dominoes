#include "Container.h"

Container::Container()
{

    font.loadFromFile("Textures\\font.ttf");
    background_tex.loadFromFile("Textures\\background.png");
    main_menu_tex.loadFromFile("Textures\\main_menu.png");
    chips_tex.loadFromFile("Textures\\chips.png");
    special_chips_tex.loadFromFile("Textures\\special_chips.png");
    helper_tex.loadFromFile("Textures\\helper.png");
    strip_tex.loadFromFile("Textures\\strip.png");
    table_tex.loadFromFile("Textures\\table.png");
    boneyard_tex.loadFromFile("Textures\\boneyard.png");
    esc_menu_tex.loadFromFile("Textures\\esc_menu.png");

    set_score_sound.loadFromFile("Sounds\\set_score.wav");
    set_score.setBuffer(set_score_sound);

    end_of_round_sound.loadFromFile("Sounds\\end_of_round_sound.wav");
    place_chip_sound.loadFromFile("Sounds\\place_chip.wav");
    menu_opened_sound.loadFromFile("Sounds\\menu_opened.wav");
    button_choosed_sound.loadFromFile("Sounds\\button_click_choosed.wav");

    background.setTexture(background_tex);
    background.setScale(scaleX, scaleY);
}