#include <iostream>
#include <time.h>
#include <vector>

namespace optionvalues
{
	bool is_singleplayer{ true }; // else Multiplayer
	unsigned short difficulty{ 3 };
}

namespace highscore
{
	struct Item
	{
		char player_name;
		int player_turns;
	};
	std::vector<Item> list;
}

void gotoHighscores()
{
	system("cls");
	std::cout << "___Highscores___" << std::endl << std::endl;

	for (int i = 0; i < highscore::list.size(); i++)
		std::cout << "Player " << highscore::list[i].player_name << ":   "
		<< highscore::list[i].player_turns << " turns!" << std::endl;

	std::cout << std::endl;
	std::cout << "Press 'ENTER' to continue";
	std::cin.ignore(80, '\n');
	std::cin.get();
}

void gotoOptions()
{
	bool is_running{ true };
	while (is_running) {
		char* mode = (optionvalues::is_singleplayer) ? "Singleplayer" : "Multiplayer ";

		char* difficulty;
		switch (optionvalues::difficulty) {
		case 1: difficulty = "Easy        "; break;
		case 2: difficulty = "Medium      "; break;
		case 3: difficulty = "Hard        "; break;
		default: difficulty = "            "; break;
		}

		system("cls");
		std::cout << "___Options___" << std::endl << std::endl;
		std::cout << "Toggle Single-/Multiplayer[now: " << mode << "]  (press '1')" << std::endl;
		std::cout << "Toggle Difficulty         [now: " << difficulty << "]  (press '2')" << std::endl;;
		std::cout << "---DONE---  (press '0')" << std::endl << std::endl;
		std::cout << "Decision: ";

		int input;
		std::cin >> input;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(80, '\n');
		}
		if (input == 1)
			optionvalues::is_singleplayer = (optionvalues::is_singleplayer) ? false : true;
		if (input == 2)
			switch (optionvalues::difficulty) {
			case 1: optionvalues::difficulty = 2; break;
			case 2: optionvalues::difficulty = 3; break;
			case 3: optionvalues::difficulty = 1; break;
			}
		else if (input == 0)
			is_running = false;
	}
}

void drawField(const char field[], const int turn)
{
	system("cls");
	std::cout << "___Turn " << turn <<"___" << std::endl << std::endl;

	for (int i = 0; i < 3; i++) { // rows
		for (int j = 0; j < 3; j++) { // colomns
			std::cout << " ";
			std::cout << field[3*i + j];
		}
		std::cout << std::endl;
	}
}

bool checkInput(const int input, const char player, char field[])
{
	// only numbers
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(80, '\n');
		return false;
	}

	// only valide numbers
	if (input < 1 || input > 9)
		return false;

	// free space for player
	if (field[input - 1] == ' ')
		field[input - 1] = player;
	else return false;

	return true;
}

bool checkForWin(const char field[], char player)
{
	// Win horizontally
	if (field[0] == player && field[1] == player && field[2] == player) return true;
	else if (field[3] == player && field[4] == player && field[5] == player) return true;
	else if (field[6] == player && field[7] == player && field[8] == player) return true;

	// Win vertically
	else if (field[0] == player && field[3] == player && field[6] == player) return true;
	else if (field[1] == player && field[4] == player && field[7] == player) return true;
	else if (field[2] == player && field[5] == player && field[8] == player) return true;

	// Win diagonally
	else if (field[0] == player && field[4] == player && field[8] == player) return true;
	else if (field[2] == player && field[4] == player && field[6] == player) return true;

	else return false;
}

void startRound()
{
	srand(time(nullptr));
	char player{ 'X' };
	int turn{ 0 };
	int input;
	unsigned short error;

	char field[9];
	for (int i = 0; i < 9; i++)
		field[i] = ' ';
	drawField(field, turn);

	bool is_set{ false };
	bool is_runnig{ true };
	while (is_runnig) {
		turn++;

		// Input
		if (optionvalues::is_singleplayer && turn % 2 == 0) {
			is_set = false;
			error = rand() % 4;

			// Check for own Win (Easy: 0.75%, Medium: 0.75%, Hard)
			if (!is_set)
				if ((optionvalues::difficulty == 1 && error != 0) ||
					 optionvalues::difficulty == 2 && error != 0 ||
					 optionvalues::difficulty == 3)

					for (int i = 0; i < 9; i++)
						if (checkInput(i + 1, 'O', field))
							if (checkForWin(field, player))
								is_set = true;
							else
								field[i] = ' ';


			// Check for opponent Win (Medium: 0.75%, Hard)
			if (!is_set)
				if ((optionvalues::difficulty == 2 && error != 0) ||
					 optionvalues::difficulty == 3)

					for (int i = 0; i < 9; i++)
						if (checkInput(i + 1, 'X', field))
							if (checkForWin(field, player))
								is_set = true;
							else
								field[i] = ' ';


			// Try to set in middle (Medium: 0.75%, Hard)
			if (!is_set)
				if ((optionvalues::difficulty == 2 && error != 0) ||
					 optionvalues::difficulty == 3)

					if (checkInput(5, 'O', field))
						is_set = true;
					else
						field[5] = ' ';


			// Random (Easy, Medium, Hard)
			if (!is_set) {
				do {
					input = rand() % 9 + 1;
				} while (!checkInput(input, player, field));
				is_set = true;
			}
		}
		else {
			do {
				std::cout << std::endl;
				std::cout << "Player " << player << ": Choose your next move (1 -> 9): ";
				std::cin >> input;
			} while (!checkInput(input, player, field));
		}

		// Draw
		drawField(field, turn);

		// Win
		if (checkForWin(field, player)) {
			is_runnig = false;

			highscore::Item item;
			item.player_name = player;
			item.player_turns = turn;
			highscore::list.push_back(item);

			std::cout << std::endl;
			std::cout << "Player " << player << " wins in " << turn << ". turn!" << std::endl;
		}
		// no Win
		else if (turn > 8) {
			is_runnig = false;
			std::cout << std::endl;
			std::cout << "Nobody wins!" << std::endl;
		}
		// next player
		else player = (player == 'X') ? 'O' : 'X';
	}
	std::cout << "Press 'ENTER' to continue";
	std::cin.ignore(80, '\n');
	std::cin.get();
}

int main()
{
	bool is_running{ true };
	while (is_running) {
		system("cls");

		std::cout << "___This is a Tic-Tac-Toe Game!___" << std::endl << std::endl;
		std::cout << " Start     (press 'S')" << std::endl;
		std::cout << " Highscore (press 'H')" << std::endl;
		std::cout << " Options   (press 'O')" << std::endl;
		std::cout << " Close     (press 'C')" << std::endl << std::endl;
		std::cout << "Decition: ";

		char input;
		std::cin >> input;

		if (input == 's' || input == 'S')
			startRound();
		else if (input == 'h' || input == 'H')
			gotoHighscores();
		else if (input == 'o' || input == 'O')
			gotoOptions();
		else if (input == 'c' || input == 'C')
			is_running = false;
	}
}