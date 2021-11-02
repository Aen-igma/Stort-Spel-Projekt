#pragma once
#include "LevelHeader.h";
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/EntityHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Graphics/Component/Light/Light.h"

using std::string;
using std::vector;
using std::ofstream;
using std::strcpy;
using std::cout;
using std::endl;
using std::stoi;

namespace Aen {

	enum LIGHTTYPES
	{
		NOLIGHT,
		SPOT,
		DIRECTIONAL,
		POINT
	};

	class LevelExporter
	{
	private:

		string m_filePath = "../LevelFolder/";
		//string m_fileName = "LevelFile.Level";
		ofstream m_outfile;

		vector<ModelStruct> m_ModelVector;
		vector<TextureStruct> m_TextureVector;
		vector<MaterialStruct> m_MaterialVector;
		vector<LightStruct> m_LightVector;
		vector<ParticleStruct> m_ParticleVector;

		void lightFunc(LightStruct*& lightStruct, vector<LightStruct>& m_LightVector, Entity*& entity);
		void modelFunc(ModelStruct*& modelStruct, vector<Aen::Entity*>& entityList, vector<string>& itemList, vector<string>& meshObjList, size_t& index, int meshIndex);
		void roomFunc(RoomStruct*& roomStruct, string array[]);
		void textureFunc(TextureStruct*& textureStruct, vector<string>& textureFileName, size_t& index);


	public:

		LevelExporter();
		~LevelExporter();

		void OpenFile(string fileName);
		void CloseFile();

		template<class T>
		void WriteToFile(T* whatToWrite, std::ofstream& outfile);

		void WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, vector<string>& meshObjList, vector<string>& textureFileName, string array[], string& fileName);
		LIGHTTYPES Type(Aen::Entity* entity);


	};
}