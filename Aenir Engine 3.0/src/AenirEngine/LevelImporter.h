// LevelImporter

#pragma once
#include "LevelHeader.h"
#include "RoomFormat.h"
#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;

namespace AenIMP
{

	struct Room
	{
		Aen::RoomHeader roomHeader;
		Aen::RoomStruct* room;

		void readFromFile(ifstream& infile);
		void printRoom();

		Room();
		~Room();
	};

	struct Model
	{
		Aen::ModelHeader modelHeader;
		Aen::ModelStruct* model;

		void readFromFile(ifstream& infile);
		void printModel();

		Model();
		~Model();
	};

	struct Texture
	{
		Aen::TextureHeader textureHeader;
		Aen::TextureStruct* texture;

		void readFromFile(ifstream& infile);
		void printTexture();

		Texture();
		~Texture();
	};

	struct Material
	{
		Aen::MaterialHeader materialHeader;
		Aen::MaterialStruct* material;

		void readFromFile(ifstream& infile);
		void printMaterial();

		Material();
		~Material();
	};

	struct Light
	{
		Aen::LightHeader lightHeader;
		Aen::LightStruct* light;

		void readFromFile(ifstream& infile);
		void printLight();

		Light();
		~Light();
	};

	struct Particle
	{
		Aen::ParticleHeader particleHeader;
		Aen::ParticleStruct* particle;

		void readFromFile(ifstream& infile);
		void printParticle();

		Particle();
		~Particle();
	};

	class CompleteRoom
	{
	private:

		AenIF::Room room;
		vector<AenIF::Model> modelVector;
		vector<AenIF::Texture> textureVector;
		vector<AenIF::Material> materialVector;
		vector<AenIF::Light> lightVector;
		vector<AenIF::Particle> particleVector;
	public:

		CompleteRoom();
		~CompleteRoom();

		void print();

		void addRoom(Room* input);
		void addModel(Model* input);
		void addTexture(Texture* input);
		void addMaterial(Material* input);
		void addLight(Light* input);
		void addParticle(Particle* input);

		AenIF::Room & GetRoom();
		vector<AenIF::Model>& GetModelVector();
		vector<AenIF::Texture>& GetTextureVector();
		vector<AenIF::Material>& GetMaterialVector();
		vector<AenIF::Light>& GetLightVector();
		vector<AenIF::Particle>& GetParticleVector();
	};

	class LevelImporter
	{
	private:
		ifstream infile;
		std::vector<CompleteRoom> roomVector;

	public:
		LevelImporter();
		~LevelImporter();
  
		void ReadFromFile(string filePath);

		// Opens a binary file at input [filepath].
		void OpenFile(string filePath);

		// Closes the currently opened binary file.
		void CloseFile();
		void printArray();

		vector<CompleteRoom>& GetRoomVector();
	};
}
