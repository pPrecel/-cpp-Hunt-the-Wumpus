#include <iostream>

namespace map {
	enum RoomState {
		nothing = 0,
		monster = 1,
		catapulta = 2,
		player = 3,
		hole = 4
	};

	class Room {
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

			for (int j = 0; j != 30; j--) {
				if (tmp->number == i) return tmp;

				if (j > 8 && i > tmp->number && tmp->number % 2 == 1) tmp = tmp->adjoing_3;
				else if (j > 8 && i < tmp->number && tmp->number % 2 == 0) tmp = tmp->adjoing_3;
				else tmp = tmp->adjoing_2;
			}
			return this;
		}

		friend Room* map_generator();
		friend Room* create_circle(int start, int end);
		friend void merge_circles(Room* firCircle, Room* secCircle, Room* thiCircle);
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

		merge_circles(firCircle, secCircle, thiCircle);

		return firCircle;
	}
}


int main() {
	map::Room* map = map::map_generator();

	return 0;
}