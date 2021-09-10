#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "RandomNumberGenerator.h"
enum class SpecialRoom{ NONE, ENTRANCE, EXIT, ARENA, SHOP };
//
//  n
//w + e
//  s
//
struct room {
	bool enclosed = false; //Var used in level generation, true when room is surrounded
	SpecialRoom roomSpecial = SpecialRoom::NONE;


	//connection location
	int north = 0;
	int east = 0;
	int south = 0;
	int west = 0;

	//Probabilities
	float baseChance = 0;
	float dynamic1 = 0;
	float dynamic2 = 0;
	float dynamic3 = 0;
	float dynamic4 = 0;

	void rotateCW() { //Clockwise rotation
		int temp = north;
		north = west;
		west = south;
		south = east;
		east = temp;
		//TODO rotate associated model
	}
	void rotateCCW() { //count clockwise rotation
		int temp = north;
		north = east;
		east = south;
		south = west;
		west = temp;
		//TODO rotate associated model
	}
	void rotate180() {
		int temp = north;
		north = south;
		south = temp;

		temp = east;
		east = west;
		west = temp;
		//TODO rotate associated model
	}
};
static std::vector<room> levelentrances;
static std::vector<room> levelexit;
static std::vector<room> levelarena;

static std::vector<room> straight;
static std::vector<room> bend;
static std::vector<room> threeway;
static std::vector<room> fourway;

static const int mapSize = 8;
static room map[mapSize][mapSize];


class LevelGenerator {
private:
	static room rngRoomFromVector(std::vector<room>& roomVec);
	static room rngRoom(int connectionDir);
public:
	static void generateLevel();

	static void GenerationTestingFunction();
};