#pragma once
#include "Menu.h"
#include "Table.h"
#include "Computer.h"
#include "hand.h"
#include "Boneyard.h"
#include "Score.h"

class Reloader
{
public:
	friend class Computer;

	Reloader(Hand& hand_1, Hand& hand_2, Boneyard& boneyard, Table& table, Score& score, Computer& computer, Menu& menu);

	void reload(bool save_score);

private:
	std::reference_wrapper<Hand> hand_1_ref;
	std::reference_wrapper<Hand> hand_2_ref;
	std::reference_wrapper<Boneyard> boneyard_ref;
	std::reference_wrapper<Table> table_ref;
	std::reference_wrapper<Score> score_ref;
	std::reference_wrapper<Computer> computer_ref;
	std::reference_wrapper<Menu> menu_ref;
};