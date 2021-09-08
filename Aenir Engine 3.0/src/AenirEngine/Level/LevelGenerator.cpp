
#include "LevelGenerator.h"

room LevelGenerator::rngRoomFromVector(std::vector<room>& roomVec) {
	//Todo implement weigths
	return roomVec[LehmerInt() % roomVec.size()];
};

room LevelGenerator::rngRoom(int connectionDir) {

	room result;
	if (connectionDir < 1 || connectionDir > 9999) {
		return result; //Backup incase of invalid direction
	}


	uint32_t weightS = 15;
	uint32_t weightB = 25;
	uint32_t weightT = 25;
	uint32_t weightF = 10;

	uint32_t weightSum = weightS + weightB + weightT + weightF;

	uint32_t randNum = LehmerInt() % weightSum;
	unsigned char type = 0;

	if (randNum < weightS) {
		result = rngRoomFromVector(straight);
		type = 1;
	}
	else if (randNum < (weightB + weightS)) {
		result = rngRoomFromVector(bend);
		type = 2;
	}
	else if (randNum < (weightT + weightB + weightS)) {
		result = rngRoomFromVector(threeway);
		type = 3;
	}
	else if (randNum < (weightF + weightT + weightB + weightS)) {
		result = rngRoomFromVector(fourway);
		type = 4;
	}
	if (!(type == 4)) //Exludes 4 way junctions, for the time being
	{
		if (connectionDir % 0xA > 0) {
			//Connection going north, rotate 1 connection south
			switch (type)
			{
			case 1:
				break;
			case 2:
				if (LehmerInt() % 100 > 50) {
					result.rotateCW();
				}
				else {
					result.rotate180();
				}
				break;
			case 3:
				randNum = LehmerInt() % 99;
				if (randNum < 33)
					result.rotate180();
				else if (randNum < 66)
					result.rotateCW();
				else
					result.rotateCCW();
				break;
			default:
				break;
			}
		}
		else if (connectionDir % 0x64 > 0) {
			//Connection going east, rotate 1 connection west
			switch (type)
			{
			case 1:
				result.rotateCCW();
				break;
			case 2:
				if (LehmerInt() % 100 > 50) {
					result.rotateCCW();
				}
				else {
					result.rotate180();
				}
				break;
			case 3:
				if (LehmerInt() % 100 > 50) {
					result.rotateCCW();
				}
				else {
					result.rotate180();
				}
				break;
			default:
				break;
			}
		}
		else if (connectionDir % 0x3E8 > 0) {
			//Connection going south, rotate 1 connection north
			switch (type)
			{
			case 1:
				break;
			case 2:
				if (LehmerInt() % 100 > 50) {
					result.rotateCCW();
				}
				break;
			case 3:
				randNum = LehmerInt() % 99;
				if (randNum < 33)
					result.rotateCW();
				else if (randNum < 66)
					result.rotateCCW();
				break;
			default:
				break;
			}
		}
		else {
			//Connection going west, rotate 1 connection east
			switch (type)
			{
			case 1:
				result.rotateCW();
				break;
			case 2:
				if (LehmerInt() % 100 > 50) {
					result.rotateCW();
				}
				break;
			case 3:
				if (LehmerInt() % 100 > 50) {
					result.rotateCW();
				}
				else {
					result.rotate180();
				}
				break;
			default:
				break;
			}
		}
	}
	return result;
}


void LevelGenerator::generateLevel() {
	bool presence[mapSize][mapSize] = { 0 }; 

	for (int y = 0; y < mapSize; y++) {
		for (int x = 0; x < mapSize; x++) {
			map[x][y] = room();
		}
	}
	presence[3][4] = true;
	map[3][4] = rngRoomFromVector(levelentrances);

	bool openConnections = true;
	int maxRooms = 24; //Soft limit

	while (openConnections && maxRooms > 0) {
		int numOpenConnections = 0;		//Tracks how many connections are open
		for (int y = 0; y < mapSize; y++) {

			for (int x = 0; x < mapSize; x++) {

				if (presence[x][y]) {

					if (y + 1 < mapSize) {											//Prevents out of bounds
						if (map[x][y].south && !presence[x][y + 1]) {					//Checks if region is clear
							presence[x][y + 1] = true;											//Sets region to not clear
							map[x][y + 1] = rngRoom(map[x][y].south * 0x64);//Insert random room (Pass along direction)
							maxRooms--;															//reduce maxRooms
							break;																//Break to go generate off other rooms (Experimental)
						}
					}

					if (y - 1 >= 0) {
						if (map[x][y].north && !presence[x][y - 1]) {
							presence[x][y - 1] = true;
							map[x][y - 1] = rngRoom(map[x][y].north * 0x1);
							maxRooms--;
							break;
						}
					}
					if (x + 1 < mapSize) {
						if (map[x][y].east && !presence[x + 1][y]) {
							presence[x + 1][y] = true;
							map[x + 1][y] = rngRoom(map[x][y].east * 0xA);
							maxRooms--;
							break;
						}
					}
					if (x - 1 >= 0) {
						if (map[x][y].west && !presence[x - 1][y]) {
							presence[x - 1][y] = true;
							map[x - 1][y] = rngRoom(map[x][y].west * 0x3E8);
							maxRooms--;
							break;
						}
					}
				}
			}
		}
		for (int y = 0; y < mapSize; y++) {

			for (int x = 0; x < mapSize; x++) {
				if (presence[x][y]) {

					if (y - 1 >= 0) {
						if (map[x][y].north && !presence[x][y - 1]) {
							numOpenConnections++;
						}
					}
					if (x + 1 < mapSize) {
						if (map[x][y].east && !presence[x + 1][y]) {
							numOpenConnections++;
						}
					}
					if (y + 1 < mapSize) {
						if (map[x][y].south && !presence[x][y + 1]) {
							numOpenConnections++;
						}
					}
					if (x - 1 >= 0) {
						if (map[x][y].west && !presence[x - 1][y]) {
							numOpenConnections++;
						}
					}
				}
			}
		}
		if (numOpenConnections <= 0) {
			openConnections = false;
		}
	}

}
void LevelGenerator::GenerationTestingFunction()
{
	char cmap[mapSize * 3][mapSize * 3];

	setLehmerSeed(0);

	//Straight corridors
	room a;
	a.north = 1;
	a.south = 1;

	straight.push_back(a);

	//90 degree corners
	room c;
	c.north = 1;
	c.east = 1;

	bend.push_back(c);


	//T junction
	room l;

	l.north = 1;
	l.east = 1;
	l.west = 1;

	threeway.push_back(l);

	//4-way junction
	room p;
	p.north = 1;
	p.east = 1;
	p.south = 1;
	p.west = 1;

	fourway.push_back(p);

	//entrance
	room q;
	q.north = 1;
	levelentrances.push_back(q);


	for (int k = 0; k < 10; k++) {
		LevelGenerator::generateLevel();

		for (int i = 0; i < 3 * mapSize; i++) {
			for (int j = 0; j < 3 * mapSize; j++) {
				cmap[i][j] = ' ';
				if (1 == (i % 3) && 1 == (j % 3)) {
					cmap[i][j] = '.';
				}
			}
		}

		for (int y = 0; y < mapSize; y++) {
			for (int x = 0; x < mapSize; x++) {
				if (map[x][y].north)
				{
					cmap[3 * y + 0][3 * x + 1] = '|';
					cmap[3 * y + 1][3 * x + 1] = '0';
				}
				if (map[x][y].south)
				{
					cmap[3 * y + 2][3 * x + 1] = '|';
					cmap[3 * y + 1][3 * x + 1] = '0';
				}
				if (map[x][y].east)
				{
					cmap[3 * y + 1][3 * x + 2] = '-';
					cmap[3 * y + 1][3 * x + 1] = '0';
				}
				if (map[x][y].west)
				{
					cmap[3 * y + 1][3 * x] = '-';
					cmap[3 * y + 1][3 * x + 1] = '0';

				}
			}
		}
		cmap[3 * 4 + 1][3 * 3 + 1] = 'X';
		for (int i = 0; i < 3 * mapSize; i++) {
			for (int j = 0; j < 3 * mapSize; j++) {
				std::cout << cmap[i][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
};