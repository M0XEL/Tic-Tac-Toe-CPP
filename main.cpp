#include <iostream>
#include <time.h>
#include <vector>
#include <Windows.h>
#include <conio.h>

struct Vec2
{
	int x, y;
};

namespace optionvalues
{
	bool is_endless_border{ true };
	unsigned short difficulty{ 3 };
}

namespace highscore
{
	struct Item
	{
		char* player_name;
		int player_score;
	};
	std::vector<Item> list;
}

void gotoHighscores()
{
	system("cls");
	std::cout << "___Highscores___" << std::endl << std::endl;

	for (int i = 0; i < highscore::list.size(); i++)
		std::cout << "Player '" << highscore::list[i].player_name << "', "
		<< "Score:   " << highscore::list[i].player_score << std::endl;

	std::cout << std::endl;
	std::cout << "Press 'ENTER' to continue";
	std::cin.ignore(80, '\n');
	std::cin.get();
}

void gotoOptions()
{
	bool is_running{ true };
	while (is_running) {
		char* border = (optionvalues::is_endless_border) ? "Endless" : "Death  ";

		char* difficulty;
		switch (optionvalues::difficulty)
		{
		case 1: difficulty = "Easy   "; break;
		case 2: difficulty = "Medium "; break;
		case 3: difficulty = "Hard   "; break;
		default: difficulty = "       "; break;
		}

		system("cls");
		std::cout << "___Options___" << std::endl << std::endl;
		std::cout << "Toggle Endless/Death Border [now: " << border << "]  (press '1')" << std::endl;
		std::cout << "Toggle Difficulty           [now: " << difficulty << "]  (press '2')" << std::endl;
		std::cout << "---DONE---  (press '0')" << std::endl << std::endl;
		std::cout << "Decision: ";

		int input;
		std::cin >> input;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(80, '\n');
		}

		switch (input)
		{
		case 0: is_running = false; break;
		case 1: optionvalues::is_endless_border = (optionvalues::is_endless_border) ? false : true; break;
		case 2:
			switch (optionvalues::difficulty)
			{
			case 1: optionvalues::difficulty = 2; break;
			case 2: optionvalues::difficulty = 3; break;
			case 3: optionvalues::difficulty = 1; break;
			}
			break;
		}	
	}
}

void drawField(char field[12][12],const int score)
{
	system("cls");
	std::cout << "___Score " << score << "___" << std::endl << std::endl;

	// Left/Right border
	for (int i = 0; i < 12; i++) {
		field[i][0] = '#';
		field[i][11] = '#';
	}
	// Up/Down border
	for (int j = 0; j < 12; j++) {
		field[0][j] = '#';
		field[11][j] = '#';
	}

	// Draw field
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << field[i][j];
		std::cout << std::endl;
	}	
}

void startRound()
{
	srand(time(nullptr));
	char input;
	char field[12][12];
	int score{ 0 };
	Vec2 player_head{ 5, 5 };
	Vec2 previous_player_head;
	Vec2 player_tail_end;
	Vec2 previous_player_tail;
	Vec2 saved_player_tail;
	Vec2 fruit{ rand() % 10 + 1, rand() % 10 + 1 };
	Vec2 fruit2{ rand() % 10 + 1, rand() % 10 + 1 };
	std::vector<Vec2> player_tail;
	bool has_tail{ false };

	enum Direction
	{
		INVALID,
		UP,
		LEFT,
		DOWN,
		RIGHT
	} player_direction;
	player_direction = INVALID;

	// clear field
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 12; j++)
			field[i][j] = ' ';

	field[player_head.y][player_head.x] = 'O';
	field[fruit.y][fruit.x] = '+';
	if (optionvalues::difficulty == 3)
		field[fruit2.y][fruit2.x] = '-';

	drawField(field, score);
	
	bool is_runnig{ true };
	while (is_runnig) {
		
		// Input
		if (_kbhit()) {
			switch (_getch())
			{
			case 'w': player_direction = UP; break;
			case 'W': player_direction = UP; break;

			case 'a': player_direction = LEFT; break;
			case 'A': player_direction = LEFT; break;

			case 's': player_direction = DOWN; break;
			case 'S': player_direction = DOWN; break;

			case 'd': player_direction = RIGHT; break;
			case 'D': player_direction = RIGHT; break;
			}
		}

		previous_player_head = player_head;
		if (has_tail)
			player_tail_end = player_tail[player_tail.size() - 1];
		else 
			player_tail_end = player_head;

		switch (player_direction)
		{
		case UP: player_head.y--; break;
		case LEFT: player_head.x--; break;
		case DOWN: player_head.y++; break;
		case RIGHT: player_head.x++; break;
		}

		for (int i = 0; i < 12; i++)
			for (int j = 0; j < 12; j++)
				field[i][j] = ' ';

		// Eat fruit
		if (player_head.x == fruit.x && player_head.y == fruit.y) {
			player_tail.push_back(player_tail_end);
			fruit.x = rand() % 10 + 1;
			fruit.y = rand() % 10 + 1;
			score++;
		}
		if (optionvalues::difficulty == 3)
			if (player_head.x == fruit2.x && player_head.y == fruit2.y) {
				fruit2.x = rand() % 10 + 1;
				fruit2.y = rand() % 10 + 1;
				if (score > 0) {
					player_tail.pop_back();
					score--;
				}
			}

		// Game over
		for (int i = 0; i < player_tail.size(); i++)
			if (player_head.x == player_tail[i].x && player_head.y == player_tail[i].y) {
				is_runnig = false;

				highscore::Item item;
				item.player_name = "BestSnakePlayer99";
				item.player_score = score;
				highscore::list.push_back(item);
			}

		// Border
		if (optionvalues::is_endless_border) {
			if (player_head.x < 1)
				player_head.x = 10;

			else if (player_head.x > 10)
				player_head.x = 1;

			else if (player_head.y < 1)
				player_head.y = 10;

			else if (player_head.y > 10)
				player_head.y = 1;
		}
		else {
			if (player_head.x < 1 || player_head.x > 11 ||
				player_head.y < 1 || player_head.y > 11) {
				is_runnig = false;

				highscore::Item item;
				item.player_name = "BestSnakePlayer99";
				item.player_score = score;
				highscore::list.push_back(item);
			}
		}
		
		// Move Tail
		previous_player_tail = previous_player_head;
		for (int i = 0; i < player_tail.size(); i++) {
			saved_player_tail = player_tail[i];
			player_tail[i] = previous_player_tail;
			previous_player_tail = saved_player_tail;
		}

		// Fill Field
		field[fruit.y][fruit.x] = '+';
		if (optionvalues::difficulty == 3)
			field[fruit2.y][fruit2.x] = '-';

		field[player_head.y][player_head.x] = '0';
		for (int i = 0; i < player_tail.size(); i++)
			field[player_tail[i].y][player_tail[i].x] = 'o';
		
		drawField(field, score);

		switch (optionvalues::difficulty)
		{
		case 1: Sleep(250); break;
		case 2: Sleep(150); break;
		case 3: Sleep(100); break;
		}
	}

	std::cout << std::endl;
	std::cout << "Press 'ENTER' to continue";
	std::cin.ignore(80, '\n');
	std::cin.get();
}


int main()
{
	bool is_running{ true };
	while (is_running) {
		system("cls");

		std::cout << "___This is a Snake Game!___" << std::endl << std::endl;
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