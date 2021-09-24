#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include "RandomNumberGenerator.h"
enum class SpecialRoom{ NONE, ENTRANCE, EXIT, BOSS, ARENA, ITEM };
enum class RoomTheme{ GENERIC, BONES, VAMP, JUNGLE};
/////////
//  n  //
//w + e// Compass lol
//  s  //
/////////
struct Room {
	bool m_enclosed	= false; //Var used in level generation, true when room is surrounded
	bool m_present	= false;


	SpecialRoom m_roomSpecial = SpecialRoom::NONE;
	RoomTheme m_roomTHeme = RoomTheme::GENERIC;

	//connection location
	uint32_t m_north	=    0;	//	___0 - ___9		 //	Straight	: 0101	: NS
	uint32_t m_east		=   00;	//	__0_ - __9_		 //	Bend		: 0011	: NE
	uint32_t m_south	=  000;	//	_0__ - _9__		 //	T junction	: 1011	: NEW
	uint32_t m_west		= 0000;	//	0___ - 9___		 //	Four way	: 1111	: NESW


	//Probabilities
	float m_baseChance	= 0;
	float m_dynamic1	= 0;
	float m_dynamic2	= 0;
	float m_dynamic3	= 0;
	float m_dynamic4	= 0;

	void rotateCW() { //Clockwise rotation
		uint32_t temp = m_north;
		m_north	= m_west;
		m_west	= m_south;
		m_south	= m_east;
		m_east	= temp;
		//TODO rotate associated model
	}
	void rotateCCW() { //count clockwise rotation
		uint32_t temp = m_north;
		m_north	= m_east;
		m_east	= m_south;
		m_south	= m_west;
		m_west	= temp;
		//TODO rotate associated model
	}
	void rotate180() {
		uint32_t temp = m_north;
		m_north	 = m_south;
		m_south	 = temp;

		temp	= m_east;
		m_east	= m_west;
		m_west	= temp;
		//TODO rotate associated model
	}

	Room();
	Room(const Room &p);
};

static std::vector<Room> levelentrances;
static std::vector<Room> levelexit;
static std::vector<Room> levelarena;

static std::vector<Room> straight;
static std::vector<Room> bend;
static std::vector<Room> threeway;
static std::vector<Room> fourway;

static const int mapSize = 8;
static Room map[mapSize][mapSize];


class LevelGenerator {
private:
	static Room RNGRoomFromVector(std::vector<Room>& roomVec);
	static Room RNGRoom(const uint32_t connectionDir);
	static void AlignRoom(Room* room, const uint32_t& connectionDir, unsigned char& type);
public:
	static Room* GenerateLevel();

	static Room* GenerationTestingFunction();

	static void AddRoomToGeneration(Room* room);

};