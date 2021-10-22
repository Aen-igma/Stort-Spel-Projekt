#pragma once
#include "LevelHeader.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/EntityHandler.h"
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

	struct ModelInfo
	{
		string m_name = ""; // Editor name
		string m_meshName = ""; // Obj name

	};

	struct MaterialInfo 
	{
		string m_materialTextureName;
		string m_materialName;
	};

	struct AnimationInfo 
	{
		string m_animationName;
	};

	struct TextureInfo 
	{
		string m_textureName;
		//string m_textureType;
	};

	struct ModelContainer
	{
		ModelInfo m_model;
		string m_type = "";
		MaterialInfo m_material;
		TextureInfo m_texture;
		AnimationInfo m_animation;
		string m_sound = "Sound";

		ModelContainer(string materialTextureName, string materialName, string textureName, string modelName, string meshName) {
			this->m_material.m_materialTextureName = materialTextureName;
			this->m_material.m_materialName = materialName;
			this->m_texture.m_textureName = textureName;
			this->m_model.m_name = modelName;
			this->m_model.m_meshName = meshName;

			cout << "MeshName " << this->m_model.m_meshName << endl;
			cout << "name " << this->m_model.m_name << endl;
		}

		void update(string &materialTextureName, string &materialName, string &textureName, string &modelName)
		{

			if (materialTextureName  != "")
			{
				this->m_material.m_materialTextureName = materialTextureName;
			}
			
			if (materialName != "")
			{
				this->m_material.m_materialName = materialName;
			}

			if (textureName != "")
			{
				this->m_texture.m_textureName = textureName;
			}
						
			if (modelName != "")
			{
				this->m_model.m_name = modelName;
			}

			cout << "MeshName " << this->m_model.m_meshName << endl;
			cout << "name " << this->m_model.m_name << endl;
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

		void lightFunc(LightStruct *&lightStruct, vector<LightStruct> &m_LightVector, Entity*& entity, unordered_map<size_t, Aen::Entity*>& lightMap);
		void modelFunc(ModelStruct *& modelStruct, vector<Aen::Entity*>& entityList, unordered_map<size_t, ModelContainer>& modelMap,size_t& index);
		void roomFunc(RoomStruct*& roomStruct, string array[]);
		void textureFunc(TextureStruct*& textureStruct, string& textureFileName);


	public:

		LevelExporter();
		~LevelExporter();

		void OpenFile(string fileName);
		void CloseFile();

		template<class T>
		void WriteToFile(T* whatToWrite, std::ofstream& outfile);

		void WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, unordered_map<size_t, ModelContainer> &modelMap,  unordered_map<size_t, Aen::Entity*> & lightMap, string array[], string &fileName);
		LIGHTTYPES Type(Aen::Entity* entity);


	};

	
}