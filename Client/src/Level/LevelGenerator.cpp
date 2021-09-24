
#include "LevelGenerator.h"

Room LevelGenerator::RNGRoomFromVector(std::vector<Room>& roomVec) {
	//Todo implement weigths

	return roomVec[LehmerInt() % roomVec.size()];
};

Room LevelGenerator::RNGRoom(const uint32_t connectionDir) {

	Room result;
	if (connectionDir < 1 || connectionDir > 9999) {
		return result; //Backup incase of invalid direction
	}


	uint32_t weightS = 15;
	uint32_t weightB = 15;
	uint32_t weightT = 25;
	uint32_t weightF = 10;

	uint32_t weightSum = weightS + weightB + weightT + weightF;

	uint32_t randNum = LehmerInt() % weightSum;
	unsigned char type = 0;

	if (randNum < weightS) {
		result = RNGRoomFromVector(straight);
		type = 1;
	}
	else if (randNum < (weightB + weightS)) {
		result = RNGRoomFromVector(bend);
		type = 2;
	}
	else if (randNum < (weightT + weightB + weightS)) {
		result = RNGRoomFromVector(threeway);
		type = 3;
	}
	else if (randNum < (weightF + weightT + weightB + weightS)) {
		result = RNGRoomFromVector(fourway);
		type = 4;
	}
	AlignRoom(&result, connectionDir, type);
	//result.m_present = true;
	return result;
}

void LevelGenerator::AlignRoom(Room* room, const uint32_t& connectionDir, unsigned char& type)
{
	uint32_t randNum;
	if (!(type == 4)) //Exludes 4 way junctions, for the time being //Rotates rooms to align connections
	{
		if (connectionDir % 10 > 0) {
			//Connection going north, rotate 1 connection south
			switch (type)
			{
			case 1:
				break;
			case 2:
				if (LehmerInt() % 0x80 > 0x40) {
					room->rotateCW();
				}
				else {
					room->rotate180();
				}
				break;
			case 3:
				randNum = LehmerInt() % 0x120;
				if (randNum < 0x60)
					room->rotate180();
				else if (randNum < 0xC0)
					room->rotateCW();
				else
					room->rotateCCW();
				break;
			default:
				break;
			}
		}
		else if (connectionDir % 100 > 0) {
			//Connection going east, rotate 1 connection west
			switch (type)
			{
			case 1:
				room->rotateCCW();
				break;
			case 2:
				if (LehmerInt() % 0x80 > 0x40) {
					room->rotateCCW();
				}
				else {
					room->rotate180();
				}
				break;
			case 3:
				if (LehmerInt() % 0x80 > 0x40) {
					room->rotateCCW();
				}
				else {
					room->rotate180();
				}
				break;
			default:
				break;
			}
		}
		else if (connectionDir % 1000 > 0) {
			//Connection going south, rotate 1 connection north
			switch (type)
			{
			case 1:
				break;
			case 2:
				if (LehmerInt() % 0x80 > 0x40) {
					room->rotateCCW();
				}
				break;
			case 3:
				randNum = LehmerInt() % 0x120;
				if (randNum < 0x60)
					room->rotateCW();
				else if (randNum < 0xC0)
					room->rotateCCW();
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
				room->rotateCW();
				break;
			case 2:
				if (LehmerInt() % 0x80 > 0x40) {
					room->rotateCW();
				}
				break;
			case 3:
				if (LehmerInt() % 0x80 > 0x40) {
					room->rotateCW();
				}
				else {
					room->rotate180();
				}
				break;
			default:
				break;
			}
		}
	}
}


Room* LevelGenerator::GenerateLevel() {

	for (int y = 0; y < mapSize; y++) {
		for (int x = 0; x < mapSize; x++) {
			map[x][y] = Room();
		}
	}
	map[3][4] = RNGRoomFromVector(levelentrances);

	bool openConnections = true;
	int maxRooms = 24; //Soft limit

	while (openConnections && maxRooms > 0) {
		int numOpenConnections = 0;		//Tracks how many connections are open
		for (int y = 0; y < mapSize; y++) {

			for (int x = 0; x < mapSize; x++) {
				if (map[x][y].m_present) {

					if (y + 1 < mapSize) {										//Prevents out of bounds
						if (map[x][y].m_south && !map[x][y + 1].m_present) {	//Checks if region is clear
							map[x][y + 1] = RNGRoom(map[x][y].m_south * 0x064);	//Insert random room (Pass along direction)
							maxRooms--;											//reduce maxRooms
							break;												//Break to go generate off other rooms (Experimental)
						}
					}

					if (y - 1 >= 0) {
						if (map[x][y].m_north && !map[x][y - 1].m_present) {
							map[x][y - 1] = RNGRoom(map[x][y].m_north * 0x001);
							maxRooms--;
							break;
						}
					}
					if (x + 1 < mapSize) {
						if (map[x][y].m_east && !map[x + 1][y].m_present) {
							map[x + 1][y] = RNGRoom(map[x][y].m_east * 0x00A);
							maxRooms--;
							break;
						}
					}
					if (x - 1 >= 0) {
						if (map[x][y].m_west && !map[x - 1][y].m_present) {
							map[x - 1][y] = RNGRoom(map[x][y].m_west * 0x3E8);
							maxRooms--;
							break;
						}
					}
				}
			}
		}
		for (int y = 0; y < mapSize; y++) {

			for (int x = 0; x < mapSize; x++) {
				if (map[x][y].m_present) {

					if (y - 1 >= 0) {
						if (map[x][y].m_north && !map[x][y - 1].m_present) {
							numOpenConnections++;
						}
					}
					if (x + 1 < mapSize) {
						if (map[x][y].m_east && !map[x + 1][y].m_present) {
							numOpenConnections++;
						}
					}
					if (y + 1 < mapSize) {
						if (map[x][y].m_south && !map[x][y + 1].m_present) {
							numOpenConnections++;
						}
					}
					if (x - 1 >= 0) {
						if (map[x][y].m_west && !map[x - 1][y].m_present) {
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

	return *map;
}
Room* LevelGenerator::GenerationTestingFunction()
{
	char cmap[mapSize * 3][mapSize * 3];


	//Straight corridors
	Room a;
	a.m_baseChance = 0xf;
	a.m_north = 1;
	a.m_south = 1;
	a.m_present = true;

	straight.push_back(a);

	//90 degree corners
	Room b;
	b.m_baseChance = 0xf;
	b.m_north = 1;
	b.m_east = 1;
	b.m_present = true;
	bend.push_back(b);


	//T junction
	Room c;

	c.m_baseChance = 0xf;
	c.m_north = 1;
	c.m_east = 1;
	c.m_west = 1;
	c.m_present = true;

	threeway.push_back(c);

	//4-way junction
	Room d;
	d.m_baseChance = 0xf;
	d.m_north = 1;
	d.m_east = 1;
	d.m_south = 1;
	d.m_west = 1;
	d.m_present = true;

	fourway.push_back(d);

	//entrance
	Room e;
	e.m_baseChance = 0xf;
	e.m_north = 1;
	e.m_present = true;
	levelentrances.push_back(e);


	for (int k = 0; k < 1; k++) {
		LevelGenerator::GenerateLevel();

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
				if(map[x][y].m_present){
					if (map[x][y].m_north)
					{
						cmap[3 * y + 0][3 * x + 1] = '|';
						cmap[3 * y + 1][3 * x + 1] = '0';
					}
					if (map[x][y].m_south)
					{
						cmap[3 * y + 2][3 * x + 1] = '|';
						cmap[3 * y + 1][3 * x + 1] = '0';
					}
					if (map[x][y].m_east)
					{
						cmap[3 * y + 1][3 * x + 2] = '-';
						cmap[3 * y + 1][3 * x + 1] = '0';
					}
					if (map[x][y].m_west)
					{
						cmap[3 * y + 1][3 * x] = '-';
						cmap[3 * y + 1][3 * x + 1] = '0';
					}
				}
			}
		}
		cmap[3 * 4 + 1][3 * 3 + 1] = 'X';
		for (int i = 0; i < 3 * mapSize; i++) {
			std::cout << (char)9;
			for (int j = 0; j < 3 * mapSize; j++) {
				std::cout << cmap[i][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	return *map;
}

void LevelGenerator::AddRoomToGeneration(Room* room)
{
	uint32_t connectionDir;

	connectionDir = room->m_north + room->m_east *10 + room->m_south *100 + room->m_west *1000;

	switch (room->m_roomSpecial)
	{
	case SpecialRoom::NONE:
		break;
	case SpecialRoom::ENTRANCE:
		levelentrances.push_back(*room);
		break;
	case SpecialRoom::EXIT:
		levelexit.push_back(*room);
		break;
	case SpecialRoom::BOSS:
		break;
	case SpecialRoom::ARENA:
		levelarena.push_back(*room);
		break;
	case SpecialRoom::ITEM:
		break;
	default:
		break;
	}
	if (room->m_roomSpecial == SpecialRoom::NONE) {
		switch (connectionDir)
		{
		case 0101:
			straight.push_back(*room);
			break;
		case 0011:
			bend.push_back(*room);
			break;
		case 1011:
			threeway.push_back(*room);
			break;
		case 1111:
			fourway.push_back(*room);
			break;
		default:
			break;
		}
	}
}


Room::Room()
{
	m_enclosed = false; //Var used in level generation, true when room is surrounded
	m_present = false;
	m_roomSpecial = SpecialRoom::NONE;


	//connection location
	m_north = 0;
	m_east = 0;
	m_south = 0;
	m_west = 0;

	//Probabilities
	m_baseChance = 0;
	m_dynamic1 = 0;
	m_dynamic2 = 0;
	m_dynamic3 = 0;
	m_dynamic4 = 0;
}

Room::Room(const Room& p)
{
	m_enclosed = p.m_enclosed; //Var used in level generation, true when room is surrounded
	m_present = p.m_present;
	m_roomSpecial = p.m_roomSpecial;


	//connection location
	m_north = p.m_north;
	m_east = p.m_east;
	m_south = p.m_south;
	m_west = p.m_west;

	//Probabilities
	m_baseChance = p.m_baseChance;
	m_dynamic1 = p.m_dynamic1;
	m_dynamic2 = p.m_dynamic2;
	m_dynamic3 = p.m_dynamic3;
	m_dynamic4 = p.m_dynamic4;
	
}
