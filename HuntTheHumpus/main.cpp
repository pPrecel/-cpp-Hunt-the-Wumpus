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
		Room(){}
		Room(const int& n, const RoomState& rs = nothing, Room* r1 = nullptr, Room* r2 = nullptr, Room* r3 = nullptr)
			: number{ n }, state{ rs }, adjoing_1{ r1 }, adjoing_2{ r2 }, adjoing_3{ r3 } {}

		Room* find(int i) {
			Room* tmp = this;
			while(tmp->number != i) tmp = closer_room(i, tmp->adjoing_1, tmp->adjoing_2, tmp->adjoing_3);
			return tmp;
		}

		Room* find_in_circle(int number) {
			Room* tmp = this;
			do {
				tmp = tmp->adjoing_1;
			} while (tmp->number != number && tmp->number != this->number);
			return (tmp->number == number) ? tmp : nullptr;
		}

		friend Room* map_generator();
		friend Room* create_circle(int start, int end);
		friend void merge_circles(Room* firCircle, Room* secCircle, Room* thiCircle);
		friend Room* closer_room(int target, Room* firstRoom, Room* secondRoom, Room* thirdRoom);
		friend Room* random_states(Room* room, int howMany, RoomState state);
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
			do{ 
				random = (rand() % 20) +1; 
				room = room->find(random);
			} while (room->state != 0);
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
		std::cout << "Polowanie na Humpusa zawiodlo Cie do ciemnej pieczary potwora." << std::endl;
		std::cout << "Niestety, spadaj¹c z urwiska, usiad³eœ na strza³ach i pochodni, przez co, w totalnej, majac jedynie 5 strzal, musisz upolowac Humpusa." << std::endl;
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
			else if(text[2] != '\0' && (text[2] < 48 || text[2] > 57)) return false;
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

	bool execute_move(CommandContainer& container) {
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
			
			//pkt testowy
			std::cout << "tmp.step: " << tmp.step << " tmp.firRoom: " << tmp.firRoom << " tmp.secRoom: " << tmp.secRoom << " tmp.thiRoom : " << tmp.thiRoom << std::endl;
		} while (!flag);
		return tmp;
	}

	bool main_engine() {
		greeting();

		map::Room* map = map::map_generator();
		map::Room* monsterPos = map::random_states(map, 1, map::monster);
		map::Room* playerPos = map::random_states(map, 1, map::player);
		map::random_states(map, 4, map::catapult);
		map::random_states(map, 4, map::hole);
		
		CommandContainer commands;

		
		bool endingFlag = 0;
		std::string nextMove = "";
		do {
			commands = get_move();
			execute_move(commands);

			std::cout << "STOP" << std::endl << std::endl;
		} while (!endingFlag);

		return true;
	}
}

int main() {
	
	while (game_engine::main_engine());
	return 0;
}