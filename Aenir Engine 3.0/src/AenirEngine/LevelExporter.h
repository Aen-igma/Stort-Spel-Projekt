#pragma once
#include "LevelHeader.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
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
		string m_fileName = "LevelFile.Level";
		ofstream m_outfile;

		vector<ModelStruct> m_ModelVector;
		vector<TextureStruct> m_TextureVector;
		vector<MaterialStruct> m_MaterialVector;
		vector<LightStruct> m_LightVector;
		vector<ParticleStruct> m_ParticleVector;

		void lightFunc(LightStruct *&lightStruct, vector<LightStruct> &m_LightVector, Entity*& entity);
		void modelFunc(ModelStruct *& modelStruct, vector<ModelStruct>& m_ModelVector, vector<Aen::Entity*>& entityList, vector<string>& entityType, vector<string>& itemList, vector<string>& meshObjList,size_t& index, int meshIndex);

	public:

		LevelExporter();
		~LevelExporter();

		void OpenFile();
		void CloseFile();

		template<class T>
		void WriteToFile(T* whatToWrite, std::ofstream& outfile);

		void WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList,vector<string>& meshObjList, vector<string>& textureFileName, vector<string>& entityType, string array[]);
		LIGHTTYPES Type(Aen::Entity* entity);


	};

	
}