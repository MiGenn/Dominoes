#include "Reloader.h"

Reloader::Reloader(Hand& hand_1, Hand& hand_2, Boneyard& boneyard, Table& table, Score& score, Computer& computer, Menu& menu) : 
    hand_1_ref(hand_1), hand_2_ref(hand_2), boneyard_ref(boneyard), table_ref(table), score_ref(score), computer_ref(computer), menu_ref(menu)
{

}

void Reloader::reload(bool save_score)
{
    if (!save_score)
        score_ref.get().reset();
    else if (score_ref.get().current_mode == Score::Mode::Table)
    {
        table_ref.get().hide();
        hand_1_ref.get().hide();
        hand_2_ref.get().hide();
        boneyard_ref.get().hide();
        menu_ref.get().chooseMode(Menu::Mode::Winnig_menu);
        menu_ref.get().previous_mode = Menu::Mode::Off;
        return;
    }

    hand_1_ref.get().leave();
    hand_2_ref.get().leave();
    boneyard_ref.get().leave();
    table_ref.get().leave();
    computer_ref.get().leave();

    Chip::shuffle(boneyard_ref.get().chips_ref.get());

    hand_1_ref.get().set();
    hand_2_ref.get().set();
    boneyard_ref.get().set();
    table_ref.get().set(Table::find_best_chip(hand_1_ref.get(), hand_2_ref.get(), computer_ref.get()));
}