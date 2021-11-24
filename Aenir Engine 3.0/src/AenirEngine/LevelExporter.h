#pragma once
#include "LevelHeader.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/EntityHandler.h"
#include "LevelEditor/ImguiType.h"
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

	class LevelExporter
	{
	private:

		int typeValue[15] = {101,11,1011,1111,1,0,1,2,3,4,5,0,1,2,3};
		string validType[15] = { "Straight","Bend","T Junction","Four Way", "None", "Normal","Entrance", "Exit", "Boss", "Arena", "Item", "Normal Dungeon", "Skeleton", "Gothic", "Aztec"};

		IGH::RoomValues valid;
		ofstream m_outfile;

		vector<ModelStruct> m_ModelVector;
		vector<TextureStruct> m_TextureVector;
		vector<MaterialStruct> m_MaterialVector;
		vector<LightStruct> m_LightVector;
		vector<ParticleStruct> m_ParticleVector;

		void lightFunc(LightStruct *&lightStruct, vector<LightStruct> &m_LightVector, Entity*& entity, unordered_map<size_t, Aen::Entity*>& lightMap);
		void modelFunc(ModelStruct *& modelStruct, vector<Aen::Entity*>& entityList, unordered_map<size_t, IGH::ModelContainer>& modelMap,size_t& index);
		void roomFunc(RoomStruct*& roomStruct, string array[]);
		void textureFunc(TextureStruct*& textureStruct, string& textureFileName);
		void materialFunc(MaterialStruct*& materialStruct, vector<Aen::Entity*>& entityList, unordered_map<size_t, IGH::ModelContainer>& modelMap, size_t& index);


		int CheckValidType(string& check);

	public:

		LevelExporter();
		~LevelExporter();

		void OpenFile(string fileName);
		void CloseFile();

		template<class T>
		void WriteToFile(T* whatToWrite, std::ofstream& outfile);

		void WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, unordered_map<size_t, IGH::ModelContainer> &modelMap,  unordered_map<size_t, Aen::Entity*> & lightMap, string array[], string &fileName);
		IGH::LIGHTTYPES Type(Aen::Entity* entity);


	};

	
}