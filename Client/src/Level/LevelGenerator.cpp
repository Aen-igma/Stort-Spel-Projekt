
#include "PCH/PCH.h"
#include "LevelGenerator.h"

namespace Aen 
{

	Room LevelGenerator::RNGRoomFromVector(std::vector<uint16_t>& roomVec) {
		//Todo implement weigths

		return levelRoom[roomVec[LehmerInt() % roomVec.size()]];
	};

	Room LevelGenerator::RNGRoom(const uint16_t& connectionDir, const uint16_t& roomIndex) {

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
		if (result.m_roomIndex == roomIndex) {
			RNGRoom(connectionDir, roomIndex);
		}
		AlignRoom(&result, connectionDir, type);
		//result.m_present = true;
		return result;
	}

	void LevelGenerator::AlignRoom(Room* room, const uint16_t& connectionDir, unsigned char& type)
	{
		uint32_t randNum;
		if (!(type == 4)) //Exludes 4 way junctions, for the time being //Rotates rooms to align connections
		{
			if ((connectionDir / 1u) % 10u > 0) {
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
			else if ((connectionDir / 10u) % 10u > 0) {
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
			else if ((connectionDir / 100u) % 10u > 0) {
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
			else if ((connectionDir / 1000u) % 10u > 0) {
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

		int r = LehmerInt() % 4;
		map[3][4] = RNGRoomFromVector(levelEntrances);

		bool openConnections = true;
		int maxRooms = 24; //Soft limit

		while (openConnections && maxRooms > 0) {
			int numOpenConnections = 0;		//Tracks how many connections are open
			for (int y = 0; y < mapSize; y++) {

				for (int x = 0; x < mapSize; x++) {
					if (map[x][y].m_present) {

						if (y - 1 >= 0 && x + 1 < mapSize && y + 1 < mapSize && x - 1 >= 0)
						{																				//Prevents out of bounds
							if ((map[x][y].connectionDirections / 1u) % 10u > 0 && !map[x][y - 1].m_present) {	//Checks if region is clear
								map[x][y - 1] = RNGRoom(0x001u, map[x][y].m_roomIndex);										//Insert random room (Pass along direction)
								maxRooms--;																//reduce maxRooms
								break;																	//Break to go generate off other rooms (Experimental)
							}
							else if ((map[x][y].connectionDirections / 10u) % 10u > 0 && !map[x + 1][y].m_present) {
								map[x + 1][y] = RNGRoom(0x00Au, map[x][y].m_roomIndex);
								maxRooms--;
								break;
							}
							else if ((map[x][y].connectionDirections / 100u) % 10u > 0 && !map[x][y + 1].m_present) {
								map[x][y + 1] = RNGRoom(0x064u, map[x][y].m_roomIndex);
								maxRooms--;
								break;
							}
							else if ((map[x][y].connectionDirections / 1000u) % 10u > 0 && !map[x - 1][y].m_present) {
								map[x - 1][y] = RNGRoom(0x3E8u, map[x][y].m_roomIndex);
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
						if (y - 1 >= 0 && x + 1 < mapSize && y + 1 < mapSize && x - 1 >= 0)
						{
							if ((map[x][y].connectionDirections / 1u) % 10u > 0 && !map[x][y - 1].m_present) {
								numOpenConnections++;
							}
							if ((map[x][y].connectionDirections / 10u) % 10u > 0 && !map[x + 1][y].m_present) {
								numOpenConnections++;
							}
							if ((map[x][y].connectionDirections / 100u) % 10u > 0 && !map[x][y + 1].m_present) {
								numOpenConnections++;
							}
							if ((map[x][y].connectionDirections / 1000u) % 10u && !map[x - 1][y].m_present) {
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
		a.connectionDirections = 101;
		a.m_present = true;


		//90 degree corners
		Room b;
		b.m_baseChance = 0xf;
		b.m_present = true;
		b.connectionDirections = 11;


		//T junction
		Room c;

		c.m_baseChance = 0xf;
		c.connectionDirections = 1011;
		c.m_present = true;


		//4-way junction
		Room d;
		d.m_baseChance = 0xf;
		d.connectionDirections = 1111;
		d.m_present = true;


		for (int i = 0; i < 4; i++) {
			a.m_roomTheme = (RoomTheme)i;
			b.m_roomTheme = (RoomTheme)i;
			c.m_roomTheme = (RoomTheme)i;
			d.m_roomTheme = (RoomTheme)i;
			AddRoomToGeneration(&a);
			AddRoomToGeneration(&b);
			AddRoomToGeneration(&c);
			AddRoomToGeneration(&d);
		}

		//entrance
		Room e;
		e.m_baseChance = 0xf;
		e.m_present = true;
		e.m_roomSpecial = SpecialRoom::ENTRANCE;
		for (int x = 1; x < 1001; x *= 10) {
			e.connectionDirections = x;
			AddRoomToGeneration(&e);
		}


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
					if (map[x][y].m_present) {
						cmap[3 * y + 1][3 * x + 1] = ((int)'0' + (int)map[x][y].m_roomTheme);
						if (map[x][y].connectionDirections % 10u)
						{
							cmap[3 * y + 0][3 * x + 1] = '|'; //North
						}
						if ((map[x][y].connectionDirections / 10u) % 10u)
						{
							cmap[3 * y + 1][3 * x + 2] = '-'; //East
						}
						if ((map[x][y].connectionDirections / 100u) % 10u)
						{
							cmap[3 * y + 2][3 * x + 1] = '|'; //South
						}
						if ((map[x][y].connectionDirections / 1000u) % 10u)
						{
							cmap[3 * y + 1][3 * x + 0] = '-'; //West
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
		//connectionDir = room->m_north + room->m_east *10 + room->m_south *100 + room->m_west *1000;

		room->m_roomIndex = levelRoom.size();

		levelRoom.push_back(*room);

		//room->m_roomIndex = roomMap[std::make_tuple(room->connectionDirections, (uint16_t)room->m_roomSpecial, (uint16_t)room->m_roomTheme)].size();
		//roomMap[std::make_tuple(room->connectionDirections, (uint16_t)room->m_roomSpecial, (uint16_t)room->m_roomTheme)].push_back(*room);

		switch (room->m_roomSpecial)
		{
		case SpecialRoom::NONE:
			switch (room->connectionDirections)
			{
			case 101:
				straight.push_back(room->m_roomIndex);
				break;
			case 11:
				bend.push_back(room->m_roomIndex);
				break;
			case 1011:
				threeway.push_back(room->m_roomIndex);
				break;
			case 1111:
				fourway.push_back(room->m_roomIndex);
				break;
			default:
				break;
			}
			break;
		case SpecialRoom::ENTRANCE:
			levelEntrances.push_back(room->m_roomIndex);
			break;
		case SpecialRoom::EXIT:
			levelExit.push_back(room->m_roomIndex);
			break;
		case SpecialRoom::BOSS:
			levelBoss.push_back(room->m_roomIndex);
			break;
		case SpecialRoom::ARENA:
			levelArena.push_back(room->m_roomIndex);
			break;
		case SpecialRoom::ITEM:
			levelItem.push_back(room->m_roomIndex);
			break;
		default:
			break;
		}
	}

	const float& LevelGenerator::GetRoomDimension()
	{
		return roomDimension;
	}

	void LevelGenerator::SetRoomDimension(float dimension)
	{
		roomDimension = dimension;
	}

	void LevelGenerator::GetRoomPos(const uint16_t& x, const uint16_t& y, float* xf, float* yf)
	{
		*xf = x * roomDimension;
		*yf = y * roomDimension;
	}

	//Floors each coordinates and calculate index for the room most closely matched
	uint16_t LevelGenerator::GetClosestRoomIndex(const float& xf, const float& yf)
	{
		uint16_t x = std::floorf(xf * (1.f / roomDimension));
		uint16_t y = std::floorf(yf * (1.f / roomDimension));
		return uint16_t(x + y * mapSize);
	}

	const Room* LevelGenerator::GetMapPointer()
	{
		return *map;
	}


	Room::Room()
	{
		m_enclosed = false; //Var used in level generation, true when room is surrounded
		m_present = false;
		m_roomSpecial = SpecialRoom::NONE;


		//connection location
		connectionDirections = 0;

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
		connectionDirections = p.connectionDirections;

		//Probabilities
		m_baseChance = p.m_baseChance;
		m_dynamic1 = p.m_dynamic1;
		m_dynamic2 = p.m_dynamic2;
		m_dynamic3 = p.m_dynamic3;
		m_dynamic4 = p.m_dynamic4;

	}

}
