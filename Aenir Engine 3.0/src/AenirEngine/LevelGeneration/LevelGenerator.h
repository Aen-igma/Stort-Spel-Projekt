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
#include "Core/GlobalSettings.h"
#include "AenDefines.h"
#include "RandomNumberGenerator.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/EntityHandler.h"



#define NORTH 1
#define EAST 10
#define SOUTH 100
#define WEST 1000

#define STRAIGHTHROU NORTH+SOUTH
#define CORNERBEND NORTH+EAST
#define THREEWAY NORTH+EAST+WEST
#define FOURWAY NORTH+EAST+SOUTH+WEST

namespace Aen {
	enum class SpecialRoom { NONE, ENTRANCE, EXIT, BOSS, ARENA, ITEM };
	enum class RoomTheme { GENERIC, BONES, VAMP, JUNGLE, PLACEHOLDER };

	/*
	|/////////|
	|//  n  //|
	|//w + e//| Compass lol
	|//  s  //|
	|/////////|
			 */

	struct Room {
	public:
		bool m_enclosed = false; //Var used in level generation, true when room is surrounded
		bool m_present = false;

		Mesh* mptr_mesh;

		SpecialRoom m_roomSpecial = SpecialRoom::NONE;
		RoomTheme m_roomTheme = RoomTheme::GENERIC;

		////connection location
		//uint32_t m_north		=    0;	|//|	___0 - ___9		 //	Straight	: 0101	: _S_N
		//uint32_t m_east		=   00;	|//|	__0_ - __9_		 //	Bend		: 0011	: __EN
		//uint32_t m_south		=  000;	|//|	_0__ - _9__		 //	T junction	: 1011	: W_EN
		//uint32_t m_west		= 0000;	|//|	0___ - 9___		 //	Four way	: 1111	: NESW
		uint16_t connectionDirections = 0000;
		//Read only
		uint16_t m_roomIndex;

		//Probabilities
		float m_baseChance = 0;
		float m_dynamic1 = 0;
		float m_dynamic2 = 0;
		float m_dynamic3 = 0;
		float m_dynamic4 = 0;

		void rotateCW() { //Clockwise rotation
			connectionDirections *= 10u;
			if (connectionDirections >= 10000u)
				connectionDirections += connectionDirections - 9999u;
			//TODO rotate associated model
		}
		void rotateCCW() { //count clockwise rotation
			uint16_t temp = connectionDirections % 10u;
			connectionDirections /= 10u;
			connectionDirections += 1000u * temp;
			//TODO rotate associated model
		}
		void rotate180() {
			uint16_t temp = connectionDirections % 10u;
			connectionDirections /= 10u;
			connectionDirections += 1000u * temp;
			temp = connectionDirections % 10u;
			connectionDirections /= 10u;
			connectionDirections += 1000u * temp;
			//TODO rotate associated model
		}

		Room();
		Room(const Room& p);
	};

	static std::vector<Room> levelRoom;

	static std::unordered_map< RoomTheme, std::unordered_map< SpecialRoom, std::unordered_map< std::uint16_t, std::vector<uint16_t> > > > masterRoomMap;


	static const int mapSize = 8;
	static float roomDimension = 3;
	static Room map[mapSize][mapSize];

	static RoomTheme m_mapTheme = RoomTheme::PLACEHOLDER;

	
	class AEN_DECLSPEC LevelGenerator {
	private:
		Room RNGRoomFromVector(std::vector<uint16_t>* roomVec);
		Room RNGRoom(const uint16_t& connectionDir, const uint16_t& roomIndex);
		void AlignRoom(Room* room, const uint16_t& connectionDir, unsigned char& type);

	protected:
		std::vector<uint16_t>* GetIndexVector(RoomTheme theme, SpecialRoom special, std::uint16_t connectionDir);
	public:
		Room* GenerateLevel();

		Room* GenerationTestingFunction();

		void AddRoomToGeneration(Room* room);

		void SetRoomDimension(float dimension);

		const float& GetRoomDimension();
		void GetRoomPos(const uint16_t& x, const uint16_t& y, float* xf, float* yf);

		uint16_t GetClosestRoomIndex(const float& xf, const float& yf);

		const Room* GetMapPointer();
	};
}
