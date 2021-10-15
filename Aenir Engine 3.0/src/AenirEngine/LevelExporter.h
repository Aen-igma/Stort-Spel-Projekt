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

		string m_filePath = "../LevelFolder/";
		string m_fileName = "LevelFile.Level";
		ofstream m_outfile;

		RoomStruct m_RoomVector;
		vector<ModelStruct> m_ModelVector;
		vector<TextureStruct> m_TextureVector;
		vector<MaterialStruct> m_MaterialVector;
		vector<LightStruct> m_LightVector;
		vector<ParticleStruct> m_ParticleVector;

	public:

		LevelExporter();
		~LevelExporter();

		void OpenFile();
		void CloseFile();

		template<class T>
		void WriteToFile(T* whatToWrite, std::ofstream& outfile);

		void WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList,vector<string>& meshObjList, vector<string>& textureFileName, vector<string>& entityType);
		string Type(Aen::Entity* entity);


	};

	
}