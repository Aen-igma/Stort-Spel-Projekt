#pragma once
#include "LevelHeader.h";
#include "Graphics/Component/Entity.h"
//#include "Graphics/Component/ComponentHandler.h"
#include <iostream>
#include <fstream>
#include <vector>


using std::string;
using std::vector;
using std::ofstream;
using std::strcpy;
using std::cout;
using std::endl;

namespace Aen {

	class LevelExporter
	{
	private:

		string filePath = "../LevelFolder/";
		string fileName = "LevelFile.Level";
		ofstream outfile;

		RoomStruct m_RoomVector;
		vector<ModelStruct> m_ModelVector;
		vector<TextureStruct> m_TextureVector;
		vector<MaterialStruct> m_MaterialVector;
		vector<LightStruct> m_LightVector;
		vector<ParticleStruct> m_ParticleVector;

	public:

		LevelExporter();
		void openFile();
		void closeFile();

		template<class T>
		void writeToFile(T* whatToWrite, std::ofstream& outfile);

		void writeInto(vector<Aen::Entity*>& entityList, vector<string>& itemList,vector<string>& meshObjList, vector<string>& textureFileName, vector<string>& entityType);
		string Type(Aen::Entity* entity);


	};

	template<class T>
	inline void LevelExporter::writeToFile(T* whatToWrite, std::ofstream& outfile)
	{
		outfile.write((const char*)&*whatToWrite, sizeof(T));
	}
}