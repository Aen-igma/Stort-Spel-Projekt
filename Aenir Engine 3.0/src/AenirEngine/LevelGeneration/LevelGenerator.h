#pragma once
//#include "AenirEngine/Graphics/DirectX11/DX11Core.h"
//#include "AenirEngine/BuritoMath.h"
//#include "AenirEngine/Graphics/Component/Resource.h"
//#include "AenirEngine/Graphics/Component/ComponentHandler.h"
//#include "AenirEngine/Graphics/Component/Entity.h"

#include <iostream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "RandomNumberGenerator.h"

#include "../Graphics/Component/Entity.h"
#include "../Graphics/Component/EntityHandler.h"
#include "../AenDefines.h"
#include "../LevelImporter.h"
#include "../ImGuiHandler.h"

#include <assert.h>


#define NORTH 1
#define EAST 10
#define SOUTH 100
#define WEST 1000

#define STRAIGHTHROU NORTH+SOUTH
#define CORNERBEND NORTH+EAST
#define THREEWAY NORTH+EAST+WEST
#define FOURWAY NORTH+EAST+SOUTH+WEST

namespace Aen {
	enum class SpecialRoom { NONE = 0, ENTRANCE = 1, EXIT = 2, BOSS = 3, ARENA = 4, ITEM = 5};
	enum class RoomTheme { GENERIC = 0, BONES, VAMP, JUNGLE, PLACEHOLDER };
	enum class RoomDirections{END = 1, STRAIGHT = 101, BEND = 11, THREE = 1011, FOUR = 1111};

	/*
	|/////////|
	|//  n  //|
	|//w + e//| Compass lol
	|//  s  //|
	|/////////|
			 */

	struct AEN_DECLSPEC Room {
	public:
		bool m_enclosed = false; //Var used in level generation, true when room is surrounded
		bool m_present = false;

		std::vector<AenIF::Model>* mptr_modelVector;
		AenIMP::CompleteRoom* mptr_parent;
		int m_CRIndex = 0;

		SpecialRoom m_roomSpecial = SpecialRoom::NONE;
		RoomTheme m_roomTheme = RoomTheme::GENERIC;

		////connection location
		//m_north		=    0;		|//|	___0 - ___9		|//|	Straight	:  101	: _S_N
		//m_east		=   00;		|//|	__0_ - __9_		|//|	Bend		:   11	: __EN
		//m_south		=  000;		|//|	_0__ - _9__		|//|	T junction	: 1011	: W_EN
		//m_west		= 0000;		|//|	0___ - 9___		|//|	Four way	: 1111	: NESW
		int connectionDirections;

		//Read only, internal use
		uint16_t m_roomIndex;

		//Probabilities
		float m_baseChance = 0;
		float m_dynamic1 = 0;
		float m_dynamic2 = 0;
		float m_dynamic3 = 0;
		float m_dynamic4 = 0;

		double rotation =  0; //3.14159265;

		void rotateCW() { //Clockwise rotation
			rotate180();
			rotateCCW();
			//rotation += -1.57079633;
			//connectionDirections *= 10;
			//if (connectionDirections > 1111)
			//	connectionDirections += connectionDirections - 10000;
			//TODO rotate associated model
		}
		void rotateCCW() { //count clockwise rotation
			rotation += 90;
			int temp = connectionDirections % 10;
			connectionDirections /= 10;
			connectionDirections += 1000 * temp;
			//TODO rotate associated model
		}
		void rotate180() {
			rotateCCW();
			rotateCCW();
			//rotation += 3.14159265;
			//int temp = connectionDirections % 10;
			//connectionDirections /= 10;
			//connectionDirections += 1000 * temp;
			//temp = connectionDirections % 10;
			//connectionDirections /= 10;
			//connectionDirections += 1000 * temp;
			//TODO rotate associated model
		}


		Room();
		//Room(const Room& p);
	};



	static const int mapSize = 12;
	static float roomDimension = 80.f;

	static RoomTheme m_mapTheme = RoomTheme::PLACEHOLDER;

	
	class AEN_DECLSPEC LevelGenerator {
	private:
	protected:
		Room RNGRoomFromVector(std::vector<uint16_t>* roomVec);
		Room RNGRoom(const uint16_t& connectionDir, const uint16_t& roomIndex);
		static void AlignRoom(Room* room, const uint16_t& connectionDir, unsigned char& type);

		Room map[mapSize][mapSize];
		std::vector<Room> levelRoom;
		Aen::ImGuiImporter m_handler;

		std::unordered_map< RoomTheme, std::unordered_map< SpecialRoom, std::unordered_map< int, std::vector<uint16_t> > > > masterRoomMap;

		void constructRoom(Vec2i pos);
		std::vector<uint16_t>* GetIndexVector(RoomTheme theme, SpecialRoom special, std::uint16_t connectionDir);

		void placeBossRoom();
		void placeLootRoom();
	public:
		~LevelGenerator();
		LevelGenerator();

		Vec2f m_mapOrigin;

		Aen::ImGuiImporter* GetHandlerPtr();

		Room* GenerateLevel();

		inline Room* GenerationTestingFunction();

		void AddRoomToGeneration(Room room);

		static void SetRoomDimension(float dimension);

		static const float& GetRoomDimension();
		static void GetRoomPos(const int& x, const int& y, float* xf, float* yf);

		static const uint16_t GetClosestRoomIndex(const float& xf, const float& yf);

		const Room* GetMapPointer();

		void InitPlaceholderRooms();

		inline void SpawnRoom(const Vec2i pos);

		void LoadRoomFiles(const string& filePath);
		inline void LoadMutipleRoomFiles();

		void AddLoadedToGeneration();

		void SetMapTheme(RoomTheme theme);

		void CleanMap();
	};
}
