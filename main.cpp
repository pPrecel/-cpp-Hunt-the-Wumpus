#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

namespace map {
	enum RoomState {
		nothing = 0,
		monster = 1,
		catapult = 2,
		player = 3,
		hole = 4
	};

	class Room {
	private:
		int number;
		Room* adjoing_1;
		Room* adjoing_2;
		Room* adjoing_3;
		RoomState state;
	public:
		Room() {}
		Room(const int& n, const RoomState& rs = nothing, Room* r1 = nullptr, Room* r2 = nullptr, Room* r3 = nullptr)
			: number{ n }, state{ rs }, adjoing_1{ r1 }, adjoing_2{ r2 }, adjoing_3{ r3 } {}

		Room* find(int i) {
			Room* tmp = this;
			while (tmp->number != i) tmp = closer_room(i, tmp->adjoing_1, tmp->adjoing_2, tmp->adjoing_3);
			return tmp;
		}

		Room* find_in_circle(int number) {
			Room* tmp = this;
			do {
				tmp = tmp->adjoing_1;
			} while (tmp->number != number && tmp->number != this->number);
			return (tmp->number == number) ? tmp : nullptr;
		}

		void print_debugger() {
			Room* map = this;
			std::cout << "          Map" << std::endl;
			std::cout << "       ____1____" << std::endl;
			std::cout << "      /    |    \\" << std::endl;
			std::cout << "     /   __8__   \\" << std::endl;
			std::cout << "   _/   /     \\   \\_" << std::endl;
			std::cout << "  /    7        9   \\" << std::endl;
			std::cout << " /    / \\      / \\   \\" << std::endl;
			std::cout << " 5ÄÄÄ6   17ÄÄ18   10ÄÄ2" << std::endl;
			std::cout << " |   |   /    \\    |  |" << std::endl;
			std::cout << " |   15ÄÄ16   19ÄÄ11  |" << std::endl;
			std::cout << " |   |   \\    /    |  |" << std::endl;
			std::cout << " \\   \\     20     /   /" << std::endl;
			std::cout << "  \\   14    |   12   /" << std::endl;
			std::cout << "   \\ / ÀÄÄÄ13ÄÄÄÙ \\ /" << std::endl;
			std::cout << "    4ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ3" << std::endl << std::endl;

			for (int i = 1; i <= 20; i++) {
				std::cout << "Room " << i << ": ";
				map = map->find(i);
				if (map->state == 0) std::cout << "nothing" << std::endl;
				else if (map->state == 1) std::cout << "monster" << std::endl;
				else if (map->state == 2) std::cout << "catapult" << std::endl;
				else if (map->state == 3) std::cout << "player" << std::endl;
				else if (map->state == 4) std::cout << "hole" << std::endl;
			}
		}

		Room* move_player(int to) {
			if (this->adjoing_1->number != to && this->adjoing_2->number != to && this->adjoing_3->number != to) return this;

			Room* nextRoom = this->find(to);

			if (nextRoom->state == nothing) {

				if (this->state != catapult)
					this->state = nothing;
				nextRoom->state = player;

				std::cout << "Trafiles do komory nr: " << nextRoom->number << ". Zlo jest coraz blizej." << std::endl;

				nextRoom->show_dangerous();

				return nextRoom;
			}
			else if (nextRoom->state == catapult) {
				srand(time(NULL));

				if (this->state != catapult)
					this->state = nothing;

				int way = (rand() % 3) + 1;

				if (way == 1) return nextRoom->move_player(nextRoom->adjoing_1->number);
				if (way == 2) return nextRoom->move_player(nextRoom->adjoing_2->number);
				if (way == 3) return nextRoom->move_player(nextRoom->adjoing_3->number);
			}
			else if (nextRoom->state == monster) return nullptr;
			else if (nextRoom->state == hole) return nullptr;
			return this;
		}

		void show_dangerous() {
			if (this->adjoing_1->state == monster || this->adjoing_2->state == monster || this->adjoing_3->state == monster)
				std::cout << "	Cos tu smierdzi zgnilym miesem. Potwor jest blisko!" << std::endl;
			if (this->adjoing_1->state == hole || this->adjoing_2->state == hole || this->adjoing_3->state == hole)
				std::cout << "	Brrrr... Ale tu zimno, czuje przeciag!" << std::endl;
			if (this->adjoing_1->state == catapult || this->adjoing_2->state == catapult || this->adjoing_3->state == catapult)
				std::cout << "	Hmmm? Co to za odglos? Chyba gdzies blisko jest jakiej mechaniczne ustrojstwo!" << std::endl;
		}

		Room* move_monster() {
			Room* monsterPos = this;

			srand(time(NULL));

			int flag = 20;

			do {
				int way = (rand() % 3) + 1;
				if (way == 1 && (monsterPos->adjoing_1->state == nothing || monsterPos->adjoing_1->state == player)) {
					if (monsterPos->adjoing_1->state == player) return nullptr;
					monsterPos->state = nothing;
					monsterPos->adjoing_1->state = monster;
					return monsterPos->adjoing_1;
				}
				if (way == 2 && (monsterPos->adjoing_2->state == nothing || monsterPos->adjoing_2->state == player)) {
					if (monsterPos->adjoing_2->state == player) return nullptr;
					monsterPos->state = nothing;
					monsterPos->adjoing_2->state = monster;
					return monsterPos->adjoing_2;
				}
				if (way == 3 && (monsterPos->adjoing_3->state == nothing || monsterPos->adjoing_3->state == player)) {
					if (monsterPos->adjoing_3->state == player) return nullptr;
					monsterPos->state = nothing;
					monsterPos->adjoing_3->state = monster;
					return monsterPos->adjoing_3;
				}
				flag--;
			} while (!flag);
			return this;
		}

		int room_number() {
			return this->number;
		}

		bool shot(int firRoom, int secRoom, int thiRoom) {
			Room* tmp = this;
			int table[3] = { firRoom,secRoom,thiRoom };

			for (int i = 0; i < 3; i++) {
				if (tmp->adjoing_1 == tmp->find(table[i])) {
					tmp = tmp->adjoing_1;
				}
				else if (tmp->adjoing_2 == tmp->find(table[i])) {
					tmp = tmp->adjoing_2;
				}
				else if (tmp->adjoing_3 == tmp->find(table[i])) {
					tmp = tmp->adjoing_3;
				}
				else return false;

				if (tmp->state == monster) return true;
			}
			return true;
		}

		bool is_monster(int pos) {
			return (this->find(pos)->state == monster) ? true : false;
		}

		friend Room* map_generator();
		friend Room* create_circle(int start, int end);
		friend void merge_circles(Room* firCircle, Room* secCircle, Room* thiCircle);
		friend Room* closer_room(int target, Room* firstRoom, Room* secondRoom, Room* thirdRoom);
		friend Room* random_states(Room* room, int howMany, RoomState state);
		friend bool check_randomer(Room* room);
	};

	Room* create_circle(int start, int end) {

		Room* firstRoom, *tmp;
		Room* newRoom = new Room(start);

		firstRoom = newRoom;

		for (int i = start + 1; i < end + 1; i++) {
			tmp = newRoom;
			newRoom = new Room(i, nothing, tmp);
			tmp->adjoing_2 = newRoom;
		}
		newRoom->adjoing_2 = firstRoom;
		firstRoom->adjoing_1 = newRoom;
		return firstRoom;
	}

	void merge_circles(Room* firCircle, Room* secCircle, Room* thiCircle) {
		for (int j = 0; j < 10; j++) {
			if (secCircle->number % 2 == 1) {
				thiCircle->adjoing_3 = secCircle;
				secCircle->adjoing_3 = thiCircle;
				thiCircle = thiCircle->adjoing_2;
			}
			else {
				firCircle->adjoing_3 = secCircle;
				secCircle->adjoing_3 = firCircle;
				firCircle = firCircle->adjoing_2;
			}
			secCircle = secCircle->adjoing_2;
		}
		return;
	}

	Room* map_generator() {
		Room* firCircle = create_circle(1, 5);
		Room* secCircle = create_circle(6, 15);
		Room* thiCircle = create_circle(16, 20);

		secCircle = secCircle->find_in_circle(8);
		thiCircle = thiCircle->find_in_circle(18);

		merge_circles(firCircle, secCircle, thiCircle);

		return firCircle;
	}

	Room* random_states(Room* room, int howMany, RoomState newState) {
		srand(time(NULL));

		for (int i = 0; i != howMany; i++) {
			int random;
			do {
				random = (rand() % 20) + 1;
				room = room->find(random);

				if (!(room->adjoing_1->state == nothing || room->adjoing_2->state == nothing || room->adjoing_3->state == nothing)) continue;
			} while (room->state != nothing);
			room->state = newState;
		}
		return room;
	}

	int how_far(int first, int second) {
		return abs(first - second);
	}

	Room* closer_room(int target, Room* firstRoom, Room* secondRoom, Room* thirdRoom) {
		Room* tmp = (how_far(target, firstRoom->number) <= how_far(target, secondRoom->number)) ? firstRoom : secondRoom;
		return (how_far(target, tmp->number) <= how_far(target, thirdRoom->number)) ? tmp : thirdRoom;

	}

	bool check_randomer(Room* room) {
		if (room->adjoing_1->state == nothing || room->adjoing_2->state == nothing || room->adjoing_3->state == nothing) return true;
		return false;
	}
}

namespace game_engine {
	struct CommandContainer {
		char step;
		int firRoom;
		int secRoom;
		int thiRoom;
	};

	void greeting() {
		std::cout << "Witaj poszukiwaczu!" << std::endl;
		std::cout << "Polowanie na Wumpusa zawiodlo Cie do ciemnej pieczary potwora." << std::endl;
		std::cout << "Niestety, spadaj¹c z urwiska, usiad³eœ na strza³ach i pochodni, przez co, w totalnej, majac jedynie 5 strzal, musisz upolowac Wumpusa." << std::endl;
		std::cout << "Kazda strzala moze przeleciec przez trzy komnaty. Wystarczy jeden celny strzal, by ubic bestie! Sptrzelanie wykonuje sie poprzez podanie litery s,"
			<< " oraz trzech komnat, oddzielonych myslnikami." << std::endl;
		std::cout << "Pamietaj jednak, ze kazdorazowy strzal sprawia, ze potwor przechodzi do losowej komory, znajdujacej sie obok." << std::endl;
		std::cout << "Wejscie do komnaty, w ktorej znajduje sie potwor, skutuje smiercia. Tak samo dzieje sie, gdy potwor wejdzie do komnaty z graczej." << std::endl;
		std::cout << "Na planszy rozstawiona jest tez pewna ilosc katapult i komnat bez podlogi." << std::endl;
		std::cout << "Wejscie do pomoju bez katapulty skutkuje natychmiastowym zakonczeniem gry, natomiast wejscie na katapulte powoduje przerzucenie do losowego pokoju." << std::endl;
		std::cout << "Postacia kierujemy poprzez wpisanie literki m oraz podanie sasiedniego pokoju. Istnieje tez opcja podpowiedzi, czyli tak zwanego trybu debug."
			<< " Pokazuje ona komnaty, w ktorych znajduja sie przeszkody i gracz.";
		std::cout << "Przyklady koment: " << std::endl << "ruch -> m13" << std::endl << "strzal -> s13-14-15" << std::endl << "debug -> d" << std::endl << std::endl << std::endl;
	}

	//toNumber(text[n], text[n+1]);
	int toNumber(char fir, char sec = 0) {
		return (sec) ? (int(sec - 48) * 10 + int(fir - 48)) : int(fir - 48);
	}

	bool check_string(std::string& text) {
		if (text[0] != 'd' && text[0] != 'm' && text[0] != 's') return false;

		if (text[0] == 'm') {
			if (text.length() < 2 || text.length() > 3) return false;

			if (text[1] < 48 || text[1] > 57) return false;
			else if (text[2] != '\0' && (text[2] < 48 || text[2] > 57)) return false;
			return true;
		}
		else if (text[0] == 's') {
			if (text.length() < 6 || text.length() > 9) return false;

			int howManyDecimals = 0;
			int howManyDashes = 0;

			for (int i = 1; i < text.length(); i++) {
				if (text[i] >= 48 && text[i] <= 57) howManyDecimals += 1;
				else if (text[i] == '-') {
					howManyDashes += 1;
					howManyDecimals = 0;
				}
				else return false;
				if (howManyDashes > 2 || howManyDecimals > 3) return false;
			}
		}
		else if (text[0] == 'd' && text.length() == 1) return true;
		else return false;

		return true;
	}

	bool fill_Container(CommandContainer& container, std::string& text) {
		if (!check_string(text)) return false;
		char move = text[0];

		if (move == 'd') {
			container.step = move;
			container.firRoom = NULL;
			container.secRoom = NULL;
			container.thiRoom = NULL;
			return true;
		}

		if (move == 'm') {
			int number;
			if (text[2] == '\0' || text[2] == '\n') {
				if (text[1] < 48 || text[1] > 57) return false;
				number = toNumber(text[1]);
			}
			else if (text[3] == '\0' || text[3] == '\n') {
				if (text[1] < 48 || text[1] > 57 || text[2] < 48 || text[2] > 57) return false;
				number = toNumber(text[2], text[1]);
			}
			else return false;

			if (number < 0 || number > 20) return false;

			container.step = move;
			container.firRoom = number;
			container.secRoom = NULL;
			container.thiRoom = NULL;
			return true;
		}

		if (move == 's') {
			int lastDash;
			int firRoom, secRoom, thiRoom;

			if (text[2] == '-') {
				lastDash = 2;
				firRoom = toNumber(text[1]);
			}
			else {
				lastDash = 3;
				firRoom = toNumber(text[2], text[1]);
			}

			if (text[lastDash + 2] == '-') {
				secRoom = toNumber(text[lastDash + 1]);
				lastDash += 2;
			}
			else {
				secRoom = toNumber(text[lastDash + 2], text[lastDash + 1]);
				lastDash += 3;
			}

			if (text[lastDash + 2] == '\0' || text[lastDash + 2] == '\n') {
				thiRoom = toNumber(text[lastDash + 1]);
			}
			else {
				thiRoom = toNumber(text[lastDash + 2], text[lastDash + 1]);
			}

			if (firRoom < 0 || firRoom > 20) return false;
			if (secRoom < 0 || secRoom > 20 || secRoom == firRoom) return false;
			if (thiRoom < 0 || thiRoom > 20 || thiRoom == firRoom || thiRoom == secRoom) return false;

			container.step = move;
			container.firRoom = firRoom;
			container.secRoom = secRoom;
			container.thiRoom = thiRoom;
		}

		return true;
	}

	CommandContainer get_move() {
		std::string nextMove;
		CommandContainer tmp;
		bool flag = true;
		do {
			if (!flag) std::cout << "Zle wprowadzone dane" << std::endl;

			std::cout << "Co chcesz robic?" << std::endl;
			std::cin >> nextMove;
			flag = fill_Container(tmp, nextMove);
		} while (!flag);
		return tmp;
	}

	bool main_engine() {
		greeting();

		map::Room* map = map::map_generator();
		map::random_states(map, 4, map::catapult);
		map::random_states(map, 4, map::hole);
		map::Room* monsterPos = map::random_states(map, 1, map::monster);
		map::Room* playerPos = map::random_states(map, 1, map::player);



		int playerArrows = 5;

		CommandContainer commands;
		bool endingFlag = false;

		std::cout << "Zaczynasz gre w komorze nr: " << playerPos->room_number() << ". Powodzenia!" << std::endl;
		playerPos->show_dangerous();

		do {
			//map->print_debugger();
			std::cout << std::endl;
			commands = get_move();

			if (commands.step == 'm') {
				if (!(playerPos = playerPos->move_player(commands.firRoom))) {
					std::cout << "Koniec gry! Przegrales!" << std::endl << std::endl << std::endl;
					endingFlag = true;
				}
			}
			else if (commands.step == 's' && playerArrows != 0) {
				if (playerPos->shot(commands.firRoom, commands.secRoom, commands.thiRoom)) {
					if (playerPos->is_monster(commands.firRoom) || playerPos->is_monster(commands.secRoom) || playerPos->is_monster(commands.thiRoom)) {
						std::cout << "Pokonales potwora! Gratulacje!" << std::endl << std::endl << std::endl;
						endingFlag = true;
					}
					else {
						std::cout << "Slychac swist powietrza, przecinanego strzala, ale nic poza tym. Potwor nadal zyje! Zostalo Ci " << playerArrows << " strzal." << std::endl;
					}
					if (!(--playerArrows)) std::cout << "O nie! Kolczan jest pusty! Teraz moge juz tylko uciekac, albo dac sie zabic!" << std::endl;

					if (!(monsterPos = monsterPos->move_monster())) {
						std::cout << "Przegrales! Potwor Cie zjadl!" << std::endl << std::endl << std::endl;
						endingFlag = true;
					}
				}
			}
			else if (commands.step == 'd') map->print_debugger();

		} while (!endingFlag);

		std::cout << "W tym miejscu gra sie konczy. Ale najpierw nacisnij dowolny klawisz.";

		system("PAUSE");
		return true;
	}
}

int main() {

	game_engine::main_engine();
	return 0;
}