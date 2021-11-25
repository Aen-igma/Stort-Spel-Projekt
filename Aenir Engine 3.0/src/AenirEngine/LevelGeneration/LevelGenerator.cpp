
#include "PCH.h"
#include "LevelGenerator.h"

namespace Aen 
{

	Room LevelGenerator::RNGRoomFromVector(std::vector<uint16_t>* roomVec) {
		//Todo implement weigths

		//Om du inte �r Andreas eller Filip och du f�r en div by zero krash h�r, s� saknar du .Level filer
		auto temp = levelRoom[(*roomVec)[LehmerInt() % roomVec->size()]];
		temp.m_present = true;
		return temp;
	};

	Room LevelGenerator::RNGRoom(const uint16_t& connectionDir, const uint16_t& roomIndex) {

		Room result = Room();
		if (connectionDir < 1 || connectionDir > 9999) {
			return result; //Backup incase of invalid direction
		}
		static int rerolls;

		uint32_t weightS = 15;
		uint32_t weightB = 15;
		uint32_t weightT = 25;
		uint32_t weightF = 15;

		uint32_t weightSum = weightS + weightB + weightT + weightF;

		uint32_t randNum = LehmerInt() % weightSum;
		static unsigned char type = 0;

		if (randNum < weightS) {
			result = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::NONE, 101));
			type = 1;
		}
		else if (randNum < (weightB + weightS)) {
			result = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::NONE, 11));
			type = 2;
		}
		else if (randNum < (weightT + weightB + weightS)) {
			result = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::NONE, 1011));
			type = 3;
		}
		else if (randNum < (weightF + weightT + weightB + weightS)) {
			result = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::NONE, 1111));
			type = 4;
		}
		//if (result.m_roomIndex == roomIndex && rerolls < 4) {
		//	rerolls++;
		//	result = RNGRoom(connectionDir, roomIndex);
		//}
		assert(result.m_roomIndex < levelRoom.size());

		AlignRoom(&result, connectionDir, type);
		result.m_present = true;
		if (result.connectionDirections > 1111 && rerolls < 8) {
			rerolls++;
			result = Room();
			result = RNGRoom(connectionDir, roomIndex);
		}
		rerolls = 0;
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


	std::vector<uint16_t>* LevelGenerator::GetIndexVector(RoomTheme theme, SpecialRoom special, std::uint16_t connectionDir)
	{
		return &masterRoomMap[theme][special][connectionDir];
	}

	Room* LevelGenerator::GenerateLevel() {

		for (int y = 0; y < mapSize; y++) {
			for (int x = 0; x < mapSize; x++) {
				map[x][y] = Room();
			}
		}
		srand(time(0));
		SetLehmerConstSeed(rand());
		int r = LehmerInt() % 4;
		switch (r)
		{
		case 0:
			map[3][3] = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::ENTRANCE, 1));
			map[3][3].m_present = true;
			break;
		case 1:
			map[3][4] = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::ENTRANCE, 1));
			map[3][4].m_present = true;
			break;
		case 2:
			map[4][4] = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::ENTRANCE, 1));
			map[4][4].m_present = true;
			break;
		case 3:
			map[4][3] = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::ENTRANCE, 1));
			map[4][3].m_present = true;
			break;
		default:
			map[3][4] = RNGRoomFromVector(GetIndexVector(m_mapTheme, SpecialRoom::ENTRANCE, 1));
			map[3][4].m_present = true;
			break;
		}


		r = LehmerInt() % 4;
		switch (r)
		{
		case 1:
			map[3][3].rotateCW();
			map[3][4].rotateCW();
			map[4][4].rotateCW();
			map[4][3].rotateCW();
			break;
		case 2:
			map[3][3].rotateCCW();
			map[3][4].rotateCCW();
			map[4][4].rotateCCW();
			map[4][3].rotateCCW();
			break;
		case 3:
			map[3][3].rotate180();
			map[3][4].rotate180();
			map[4][4].rotate180();
			map[4][3].rotate180();
			break;
		}

		bool openConnections = true;
		int maxRooms = 16; //Soft limit

		while (openConnections && maxRooms > 0) {
			int numOpenConnections = 0;		//Tracks how many connections are open
			for (int y = 0; y < mapSize; y++) {

				for (int x = 0; x < mapSize; x++) {
					SetLehmerSeed(x + y * mapSize);
					if (map[x][y].m_present) {

						if (y - 1 >= 0 && x + 1 < mapSize && y + 1 < mapSize && x - 1 >= 0)
						{																				//Prevents out of bounds
							if ((map[x][y].connectionDirections / 1u) % 10u > 0 && !map[x][y - 1].m_present) {	//Checks if region is clear
								map[x][y - 1] = RNGRoom(0x001u, map[x][y].m_roomIndex);					//Insert random room (Pass along direction)
								maxRooms--;																//reduce maxRooms
								break;																	//Break to go generate off other rooms (Experimental)
								//North
							}
							else if ((map[x][y].connectionDirections / 10u) % 10u > 0 && !map[x + 1][y].m_present) {
								map[x + 1][y] = RNGRoom(0x00Au, map[x][y].m_roomIndex);
								maxRooms--;
								break;
								//East
							}
							else if ((map[x][y].connectionDirections / 100u) % 10u > 0 && !map[x][y + 1].m_present) {
								map[x][y + 1] = RNGRoom(0x064u, map[x][y].m_roomIndex);
								maxRooms--;
								break;
								//South
							}
							else if ((map[x][y].connectionDirections / 1000u) % 10u > 0 && !map[x - 1][y].m_present) {
								map[x - 1][y] = RNGRoom(0x3E8u, map[x][y].m_roomIndex);
								maxRooms--;
								break;
								//West
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

		
		placeBossRoom();

		
		return *map;
	}
	inline Room* LevelGenerator::GenerationTestingFunction()
	{
		char cmap[mapSize * 3][1 + mapSize * 3];

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
						cmap[3 * y + 1][3 * x + 1] = ((int)'0' + (int)map[x][y].m_roomSpecial);
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
					cmap[3 * y + 0][3 * mapSize] = 0;
					cmap[3 * y + 1][3 * mapSize] = 0;
					cmap[3 * y + 2][3 * mapSize] = 0;
				}
			}
			OutputDebugStringA(LPCSTR("\n"));
			OutputDebugStringA(LPCSTR("///////////////////////////////////"));
			OutputDebugStringA(LPCSTR("\n"));
			for (int i = 0; i < 3 * mapSize; i++) {
				//for (int j = 0; j < 3 * mapSize; j++) {
				//	std::cout << cmap[i][j];
				//}
				LPCSTR out = cmap[i];
				OutputDebugStringA(out);
				OutputDebugStringA(LPCSTR("\n"));
				//std::cout << std::endl;
			}
			//std::cout << std::endl;
			OutputDebugStringA(LPCSTR("\n"));
			OutputDebugStringA(LPCSTR("///////////////////////////////////"));
			OutputDebugStringA(LPCSTR("\n"));
		}
		CleanMap();
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
					cmap[3 * y + 1][3 * x + 1] = ((int)'0' + (int)map[x][y].m_roomSpecial);
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
				cmap[3 * y + 0][3 * mapSize] = 0;
				cmap[3 * y + 1][3 * mapSize] = 0;
				cmap[3 * y + 2][3 * mapSize] = 0;
			}
		}
		OutputDebugStringA(LPCSTR("\n"));
		OutputDebugStringA(LPCSTR("///////////////////////////////////"));
		OutputDebugStringA(LPCSTR("\n"));
		for (int i = 0; i < 3 * mapSize; i++) {
			//for (int j = 0; j < 3 * mapSize; j++) {
			//	std::cout << cmap[i][j];
			//}
			LPCSTR out = cmap[i];
			OutputDebugStringA(out);
			OutputDebugStringA(LPCSTR("\n"));
			//std::cout << std::endl;
		}
		//std::cout << std::endl;
		OutputDebugStringA(LPCSTR("\n"));
		OutputDebugStringA(LPCSTR("///////////////////////////////////"));
		OutputDebugStringA(LPCSTR("\n"));
	
		return *map;
	}

	void LevelGenerator::AddRoomToGeneration(Room room)
	{

		room.m_roomIndex = levelRoom.size();

		levelRoom.push_back(Room(room));

		masterRoomMap[room.m_roomTheme][room.m_roomSpecial][room.connectionDirections].push_back(room.m_roomIndex);

	}

	Aen::ImGuiImporter* LevelGenerator::GetHandlerPtr()
	{
		return &m_handler;
	}

	const float& LevelGenerator::GetRoomDimension()
	{
		return roomDimension;
	}

	void LevelGenerator::SetRoomDimension(float dimension)
	{
		roomDimension = dimension;
	}

	void LevelGenerator::GetRoomPos(const int& x, const int& y, float* xf, float* yf)
	{
		*yf = x * roomDimension;
		*xf = y * roomDimension;
	}

	//Floors each coordinates and calculate index for the room most closely matched
	const uint16_t LevelGenerator::GetClosestRoomIndex(const float& xf, const float& yf)
	{
		uint16_t x = std::floorf(xf * (1.f / roomDimension));
		uint16_t y = std::floorf(yf * (1.f / roomDimension));
		return uint16_t(x + y * mapSize);
	}

	void LevelGenerator::placeBossRoom()
	{
		bool bossRoomPlaced = false;
		Vec2i entrancePos;

		for (int y = 0; y < mapSize; y++) {
			for (int x = 0; x < mapSize; x++) {
				if (map[x][y].m_present && map[x][y].m_roomSpecial == SpecialRoom::ENTRANCE) {
					entrancePos = Vec2i(x, y);
				}
			}
		}

		for (int y = 0; y < mapSize; y++) {

			for (int x = 0; x < mapSize; x++) {
				if (map[x][y].m_present && !bossRoomPlaced) {

					if (y - 1 >= 0 && x + 1 < mapSize && y + 1 < mapSize && x - 1 >= 0)
					{																				//Prevents out of bounds
						if ((map[x][y].connectionDirections / 1u) % 10u > 0 && !map[x][y - 1].m_present) {	//Checks if region is clear
							map[x][y - 1] = RNGRoomFromVector(&masterRoomMap[m_mapTheme][SpecialRoom::BOSS][1]);	//
							map[x][y - 1].rotate180();
							bossRoomPlaced = true;
							break;																	
							//North
						}
						else if ((map[x][y].connectionDirections / 10u) % 10u > 0 && !map[x + 1][y].m_present) {
							map[x + 1][y] = RNGRoomFromVector(&masterRoomMap[m_mapTheme][SpecialRoom::BOSS][1]);
							map[x + 1][y].rotateCCW();
							bossRoomPlaced = true;
							break;
							//East
						}
						else if ((map[x][y].connectionDirections / 100u) % 10u > 0 && !map[x][y + 1].m_present) {
							map[x][y + 1] = RNGRoomFromVector(&masterRoomMap[m_mapTheme][SpecialRoom::BOSS][1]);
							bossRoomPlaced = true;
							break;
							//South
						}
						else if ((map[x][y].connectionDirections / 1000u) % 10u > 0 && !map[x - 1][y].m_present) {
							map[x - 1][y] = RNGRoomFromVector(&masterRoomMap[m_mapTheme][SpecialRoom::BOSS][1]);
							map[x - 1][y].rotateCW();
							bossRoomPlaced = true;
							break;
							//West
						}
					}
				}
			}
		}
	}

	LevelGenerator::~LevelGenerator()
	{

	}

	const Room* LevelGenerator::GetMapPointer()
	{
		return *map;
	}

	void LevelGenerator::InitPlaceholderRooms()
	{
		Aen::Mesh& cube = Aen::Resource::CreateMesh("Cube");
		cube.Load(AEN_RESOURCE_DIR("Cube.obj"));
		m_mapTheme = RoomTheme::PLACEHOLDER;
		Room temp;
		temp.m_baseChance = 15;
		temp.m_present = true;
		temp.m_roomSpecial = SpecialRoom::NONE;
		temp.m_roomTheme = RoomTheme::PLACEHOLDER;
		temp.connectionDirections = 101;
		//temp.mptr_modelVector = &cube;
		AddRoomToGeneration(temp);
		temp.connectionDirections = 11;
		AddRoomToGeneration(temp);
		temp.connectionDirections = 1011;
		AddRoomToGeneration(temp);
		temp.connectionDirections = 1111;
		AddRoomToGeneration(temp);

		temp.connectionDirections = 1;
		temp.m_roomSpecial = SpecialRoom::ENTRANCE;
		AddRoomToGeneration(temp);

	}


	void LevelGenerator::constructRoom(Vec2i pos)
	{
		m_handler.LoadLevel(map[pos.x][pos.y].mptr_parent, (Vec2f(pos) * roomDimension) - m_mapOrigin, map[pos.x][pos.y].rotation);
		map[pos.x][pos.y].mptr_modelVector = &map[pos.x][pos.y].mptr_parent->GetModelVector();
	}


	void LevelGenerator::SpawnRoom(Vec2i pos)
	{
		if (map[pos.x][pos.y].m_present) {
			constructRoom(pos);
		}
	}

	void LevelGenerator::LoadRoomFiles(const string& filePath)
	{
		//m_handler.GetImporterPtr()->ReadFromFile(filePath);
	}

	inline void LevelGenerator::LoadMutipleRoomFiles()
	{
		m_handler.ReadAllFilesFromResourceFolder();
	}

	void LevelGenerator::AddLoadedToGeneration()
	{
		Room temp;
		int index = 0;
		for (auto i = 0; i < m_handler.GetImporterPtr()->GetRoomVector().size(); i++) {
			auto strRoom = &m_handler.GetImporterPtr()->GetRoomVector()[i];
			temp.connectionDirections	=	strRoom->GetRoom().type;
			temp.m_roomSpecial	=	(SpecialRoom)strRoom->GetRoom().special;
			temp.m_roomTheme	=	(RoomTheme)strRoom->GetRoom().theme;
			temp.m_baseChance	=	strRoom->GetRoom().probability;
			temp.mptr_parent	=	strRoom;
			temp.m_CRIndex		=	i;
			temp.m_present		= true;

			temp.rotation = 3.14159265;
			if (temp.connectionDirections == 11) {
				temp.rotation += 1.57079633;
			}
			this->AddRoomToGeneration(temp);
		}
	}

	void LevelGenerator::SetMapTheme(RoomTheme theme)
	{
		m_mapTheme = theme;
	}

	void LevelGenerator::CleanMap()
	{
		for (int y = 0; y < mapSize; y++) {
			for (int x = 0; x < mapSize; x++) {
				if (!map[x][y].m_present) {
					continue;
				}
				bool
					on = (map[x][y].connectionDirections / 1u) % 10u > 0,
					oe = (map[x][y].connectionDirections / 10u) % 10u > 0,
					os = (map[x][y].connectionDirections / 100u) % 10u > 0,
					ow = (map[x][y].connectionDirections / 1000u) % 10u > 0;


				bool 
					n = (map[x][y].connectionDirections / 1u) % 10u > 0,
					e = (map[x][y].connectionDirections / 10u) % 10u > 0,
					s = (map[x][y].connectionDirections / 100u) % 10u > 0,
					w = (map[x][y].connectionDirections / 1000u) % 10u > 0;

				if(y - 1 >= 0){
					if ((map[x][y].connectionDirections / 1u) % 10u > 0) {
						if (!map[x][y - 1].m_present) {
							//North facing unconnected
							n = false;
						}
						else {
							if (!((map[x][y - 1].connectionDirections / 100u) % 10u) > 0) {
								n = false;
							}
						}
					}
				}
				else{
					n = false;
				}
				if(x + 1 < mapSize){
					if ((map[x][y].connectionDirections / 10u) % 10u > 0) {
						if (!map[x + 1][y].m_present) {
							//East facing unconnected
							e = false;
						}
						else {
							if (!((map[x + 1][y].connectionDirections / 1000u) % 10u > 0)) {
								e = false;
							}
						}
					}
				}
				else {
					e = false;
				}
				
				if(y + 1 < mapSize){
					if ((map[x][y].connectionDirections / 100u) % 10u > 0) {
						if (!map[x][y + 1].m_present) {
							//South facing unconnected
							s = false;
						}
						else {
							if (!((map[x][y + 1].connectionDirections / 1u) % 10u > 0)) {
								s = false;
							}
						}
					}
				}
				else {
					s = false;
				}
				if(x - 1 >= 0)
				{
					if ((map[x][y].connectionDirections / 1000u) % 10u > 0) {
						if (!map[x - 1][y].m_present) {
							//West facing unconnected
							w = false;
						}
						else {
							if (!((map[x - 1][y].connectionDirections / 10u) % 10u > 0)) {
								w = false;
							}
						}
					}
				}
				else {
					w = false;
				}
				UINT32 numCon = (UINT32)(on & n) + (UINT32)(oe & e) + (UINT32)(os & s) + (UINT32)(ow & w);
				UINT32 numUnCon = (UINT32)(on & !n) + (UINT32)(oe & !e) + (UINT32)(os & !s) + (UINT32)(ow & !w);
				UINT32 conDir = n + 10 * e + 100 * s + 1000 * w;
				UINT32 unConDir = (on & !n) + 10 * (oe & !e) + 100 * (os & !s) + 1000 * (ow & !w);
				if (numUnCon == 0) {
					continue; //Early exit; no unconnected exits to remove
				}
				
				switch (numCon)
				{
				case 1:
					//Reduce to dead end
					switch (conDir)
					{
					case 1:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1));
						break;
					case 10:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1));
						map[x][y].rotateCW();
						break;
					case 100:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1));
						map[x][y].rotate180();
						break;
					case 1000:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1));
						map[x][y].rotateCCW();
						break;
					}
					break;
				case 2:
					//reduce to corridor or bend
					if (n & s || e & w) {
						//Corridor
						if (n) {
							//North-South
							map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 101));
						}
						else {
							//East-West
							map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 101));
							map[x][y].rotateCCW();
						}
					}
					else {
						//Bend
						switch (conDir)
						{
						case 11:
							//North-East
							map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
							break;
						case 110:
							//South-East
							map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
							map[x][y].rotateCW();
							break;
						case 1100:
							//South-West
							map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
							map[x][y].rotate180();
							break;
						case 1001:
							//North-West
							map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
							map[x][y].rotateCCW();
							break;
						}
					}
					break;
				case 3:
					//Replace with T-junction
					switch (unConDir)
					{
					case 1:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1011));
						map[x][y].rotate180();
						break;
					case 10:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1011));
						map[x][y].rotateCCW();
						break;
					case 100:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1011));
						break;
					case 1000:
						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 1011));
						map[x][y].rotateCW();
						break;
					default:
						break;
					}
					break;
				case 4:
					OutputDebugStringA(LPCSTR("What, a room with 4 unconnected directions?!?"));
					break;
				}
			}
		}

		//		//Replace room
		//		switch(numCon) {
		//		case 2:
		//		break;
		//		case 3:
		//			switch (numUnCon)
		//			{
		//			case 1:
		//				break;
		//			case 2:
		//				//Replace with dead end
		//				break;
		//			}
		//		break;
		//		case 4:
		//				break;
		//			case 2:
		//				//Replace with corridor/bend
		//				switch (unConDir)
		//				{
		//				//Straight through
		//				case 101:
		//				case 1010:
		//					if (n) {
		//						//North-South
		//						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 101));
		//					}
		//					else {
		//						//East-West
		//						map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 101));
		//						map[x][y].rotateCCW();
		//					}
		//					break;
		//				//Bend
		//				case 11:
		//					//North-East
		//					map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
		//					break;
		//				case 110:
		//					//South-East
		//					map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
		//					map[x][y].rotateCW();
		//					break;
		//				case 1100:
		//					//South-West
		//					map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
		//					map[x][y].rotate180();
		//					break;
		//				case 1001:
		//					//North-West
		//					map[x][y] = RNGRoomFromVector(GetIndexVector(map[x][y].m_roomTheme, map[x][y].m_roomSpecial, 11));
		//					map[x][y].rotateCCW();
		//					break;
		//				}
		//				break;
		//			case 3:
		//				//Replace with dead end
		//				break;
		//			}
		//		}
		//	}
		//}
	}


	Room::Room()
	{
		m_enclosed = false; //Var used in level generation, true when room is surrounded
		m_present = false;
		m_roomSpecial = SpecialRoom::NONE;
		m_roomIndex = MAXUINT16;

		mptr_modelVector = nullptr;
		mptr_parent = nullptr;

		//connection location
		connectionDirections = 0;

		//Probabilities
		m_baseChance = 0;
		m_dynamic1 = 0;
		m_dynamic2 = 0;
		m_dynamic3 = 0;
		m_dynamic4 = 0;
	}

	//Room::Room(const Room& p)
	//{
	//	m_enclosed = p.m_enclosed; //Var used in level generation, true when room is surrounded
	//	m_present = p.m_present;
	//	m_roomSpecial = p.m_roomSpecial;
	//	m_roomTheme = p.m_roomTheme;

	//	mptr_mesh = p.mptr_mesh;
	//	mptr_parent = p.mptr_parent;
	//	this->m_roomIndex = p.m_roomIndex;
	//	m_CRIndex = p.m_CRIndex;

	//	//connection location
	//	connectionDirections = p.connectionDirections;

	//	//Probabilities
	//	m_baseChance = p.m_baseChance;
	//	m_dynamic1 = p.m_dynamic1;
	//	m_dynamic2 = p.m_dynamic2;
	//	m_dynamic3 = p.m_dynamic3;
	//	m_dynamic4 = p.m_dynamic4;
	//}
}
