#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include "RandomNumberGenerator.h"
enum class SpecialRoom{ NONE, ENTRANCE, EXIT, ARENA, SHOP };
//
//  n
//w + e
//  s
//
struct Room {
	bool m_enclosed = false; //Var used in level generation, true when room is surrounded
	bool m_present = false;
	SpecialRoom m_roomSpecial = SpecialRoom::NONE;


	//connection location
	uint32_t m_north = 0;
	uint32_t m_east = 0;
	uint32_t m_south = 0;
	uint32_t m_west = 0;

	//Probabilities
	float m_baseChance = 0;
	float m_dynamic1 = 0;
	float m_dynamic2 = 0;
	float m_dynamic3 = 0;
	float m_dynamic4 = 0;

	void rotateCW() { //Clockwise rotation
		uint32_t temp = m_north;
		m_north = m_west;
		m_west = m_south;
		m_south = m_east;
		m_east = temp;
		//TODO rotate associated model
	}
	void rotateCCW() { //count clockwise rotation
		uint32_t temp = m_north;
		m_north = m_east;
		m_east = m_south;
		m_south = m_west;
		m_west = temp;
		//TODO rotate associated model
	}
	void rotate180() {
		uint32_t temp = m_north;
		m_north = m_south;
		m_south = temp;

		temp = m_east;
		m_east = m_west;
		m_west = temp;
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
	static const Room RNGRoomFromVector(const std::vector<Room>& roomVec);
	static const Room RNGRoom(const uint32_t connectionDir);
public:
	static void GenerateLevel();

	static void GenerationTestingFunction();
};