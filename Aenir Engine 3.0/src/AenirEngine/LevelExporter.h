#pragma once
#include "LevelHeader.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Graphics/Component/Light/Light.h"
#include <unordered_map>


using std::string;
using std::vector;
using std::ofstream;
using std::strcpy;
using std::cout;
using std::endl;
using std::stoi;
using std::unordered_map;

namespace Aen {

	enum LIGHTTYPES
	{
		NOLIGHT,
		SPOT,
		DIRECTIONAL,
		POINT
	};

	struct MatTexContainer
	{
		string tex;
		string mat;
		string m_meshName;
		string m_textureName;

		MatTexContainer(string inTex, string inMat, string m_meshName, string m_textureName) {
			this->tex = inTex;
			this->mat = inMat;
			this->m_meshName = m_meshName;
			this->m_textureName = m_textureName;

		}
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

		void lightFunc(LightStruct *&lightStruct, vector<LightStruct> &m_LightVector, Entity*& entity);
		void modelFunc(ModelStruct *& modelStruct, vector<Aen::Entity*>& entityList, vector<string>& itemList, vector<string>& meshObjList,size_t& index, int meshIndex);
		void roomFunc(RoomStruct*& roomStruct, string array[]);
		void textureFunc(TextureStruct*& textureStruct, string& textureFileName);


	public:

		LevelExporter();
		~LevelExporter();

		void OpenFile(string fileName);
		void CloseFile();

		template<class T>
		void WriteToFile(T* whatToWrite, std::ofstream& outfile);

		void WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList,vector<string>& meshObjList, unordered_map<unsigned int, MatTexContainer> &textureMap, string array[], string &fileName);
		LIGHTTYPES Type(Aen::Entity* entity);


	};

	
}