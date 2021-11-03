#pragma once
#include"Graphics/Component/Resource.h"
#include<filesystem>


#include "LevelImporter.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/EntityHandler.h"
#include "LevelEditor/ImguiType.h"
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;
using std::unordered_map;

namespace Aen 
{

	class AEN_DECLSPEC ImGuiImporter
	{
	private:

		AenIMP::LevelImporter *m_levelImporter;

		Aen::EntityHandler* mp_entityHandlerPtr;
		vector<Aen::Entity*> *m_entityList;
		vector<string> *m_itemList;

		unordered_map< size_t, IGH::ModelContainer> *m_modelMap;
		unordered_map< size_t, Aen::Entity*> *m_lightMap;

		unsigned int m_entityCount = 0;
		unsigned int m_lightCount = 0;

		bool m_standAlone = false;

	public:

		ImGuiImporter();
		ImGuiImporter(vector<Aen::Entity*>* m_entityList, vector<string>* m_itemList, unordered_map< size_t, IGH::ModelContainer>* m_modelMap, unordered_map< size_t, Aen::Entity*>* m_lightMap, AenIMP::LevelImporter* m_levelImporter);
		~ImGuiImporter();

		bool import(string& levelPath);

		bool import(AenIMP::LevelImporter &m_levelImporter, string & levelPath, float* translation, float* rotation, float* scale);

		void GetFloatArray(float* inputArray, float& x, float& y, float& z);

		void ReadAllFilesFromResourceFolder();

		AenIMP::LevelImporter* GetImporterPtr();
		AenIF::Room GetRoom(size_t index);
		vector<Aen::Entity*>* GetEntityList();
		bool LoadLevel(int index);
		bool LoadLevel(AenIMP::CompleteRoom* roomPtr, Aen::Vec2f offset, float angle);


	public:
		// All add func here


		size_t AddBase(AenIF::Model& model, AenIF::Texture& texture);
		size_t AddBase(const string& meshName, const string& objName);

		void AddLight(Aen::Entity* entity);
		void AddLight(Aen::Entity* entity, string type);

		
		void AddEnemy(Aen::Entity* entity, AenIF::Model& model); // Write import code here

		size_t AddPointLight(AenIF::Light& input);
		void AddPointLight();

		size_t AddSpotLight(AenIF::Light& input);
		void AddSpotLight();

		size_t AddDirectional(AenIF::Light& input);
		void AddDirectional();

		void AddModel(Aen::Entity* entity, string name);
		void AddModel(Aen::Entity* entity);

		void AddBase(AenIF::Model& model, AenIF::Texture& texture, Aen::Vec2f offset, float angle);
		void AddPointLight(AenIF::Light& input, Aen::Vec2f offset, float angle);
		void AddSpotLight(AenIF::Light& input, Aen::Vec2f offset, float angle);
	};

}


