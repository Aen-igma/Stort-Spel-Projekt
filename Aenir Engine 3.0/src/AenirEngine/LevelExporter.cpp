#include "PCH.h"
#include "LevelExporter.h"

namespace Aen {
	void LevelExporter::lightFunc(LightStruct*& lightStruct, 
		vector<LightStruct>& m_LightVector, Entity*& entity, unordered_map<size_t, Aen::Entity*>& lightMap)
	{
		size_t id = entity->GetID();
		LIGHTTYPES type = Type(entity);

		if (lightMap.find(id) != lightMap.end())
		{
			if (type == LIGHTTYPES::DIRECTIONAL)
			{
				strcpy(lightStruct->type, "Directional light");

				lightStruct->translation[0] = 0;
				lightStruct->translation[1] = 0;
				lightStruct->translation[2] = 0;

				lightStruct->rotation[0] = entity->GetRot().x;
				lightStruct->rotation[1] = entity->GetRot().y;
				lightStruct->rotation[2] = entity->GetRot().z;

				lightStruct->direction[0] = entity->GetRot().x;
				lightStruct->direction[1] = entity->GetRot().y;
				lightStruct->direction[2] = entity->GetRot().z;

				lightStruct->intensity = Aen::ComponentHandler::GetDirectionalLight(id).m_light.m_strength;

				lightStruct->color[0] = Aen::ComponentHandler::GetDirectionalLight(id).m_light.m_color.r;
				lightStruct->color[1] = Aen::ComponentHandler::GetDirectionalLight(id).m_light.m_color.g;
				lightStruct->color[2] = Aen::ComponentHandler::GetDirectionalLight(id).m_light.m_color.b;

				lightStruct->attenuation[0] = 0;
				lightStruct->attenuation[1] = 0;
				lightStruct->attenuation[2] = 0;
				lightStruct->range = 0;
				lightStruct->angle = 0;
			}

			if (type == LIGHTTYPES::POINT)
			{
				strcpy(lightStruct->type, "Point light");

				lightStruct->translation[0] = entity->GetPos().x;
				lightStruct->translation[1] = entity->GetPos().y;
				lightStruct->translation[2] = entity->GetPos().z;

				lightStruct->rotation[0] = 0;
				lightStruct->rotation[1] = 0;
				lightStruct->rotation[2] = 0;

				lightStruct->direction[0] = 0;
				lightStruct->direction[1] = 0;
				lightStruct->direction[2] = 0;

				lightStruct->intensity = Aen::ComponentHandler::GetPointLight(id).m_light.m_strength;

				lightStruct->color[0] = Aen::ComponentHandler::GetPointLight(id).m_light.m_color.r;
				lightStruct->color[1] = Aen::ComponentHandler::GetPointLight(id).m_light.m_color.g;
				lightStruct->color[2] = Aen::ComponentHandler::GetPointLight(id).m_light.m_color.b;

				lightStruct->attenuation[0] = Aen::ComponentHandler::GetPointLight(id).m_light.m_dist.x;
				lightStruct->attenuation[1] = Aen::ComponentHandler::GetPointLight(id).m_light.m_dist.y;
				lightStruct->attenuation[2] = Aen::ComponentHandler::GetPointLight(id).m_light.m_dist.z;

				lightStruct->range = Aen::ComponentHandler::GetPointLight(id).m_light.m_dist.w;
				lightStruct->angle = 0;
			}

			if (type == LIGHTTYPES::SPOT)
			{
				strcpy(lightStruct->type, "Spot light");
				lightStruct->translation[0] = entity->GetPos().x;
				lightStruct->translation[1] = entity->GetPos().y;
				lightStruct->translation[2] = entity->GetPos().z;

				lightStruct->rotation[0] = entity->GetRot().x;
				lightStruct->rotation[1] = entity->GetRot().y;
				lightStruct->rotation[2] = entity->GetRot().z;

				lightStruct->direction[0] = 0;
				lightStruct->direction[1] = 0;
				lightStruct->direction[2] = 0;

				lightStruct->intensity = Aen::ComponentHandler::GetSpotLight(id).m_light.m_strength;

				lightStruct->color[0] = Aen::ComponentHandler::GetSpotLight(id).m_light.m_color.r;
				lightStruct->color[1] = Aen::ComponentHandler::GetSpotLight(id).m_light.m_color.g;
				lightStruct->color[2] = Aen::ComponentHandler::GetSpotLight(id).m_light.m_color.b;

				lightStruct->range = Aen::ComponentHandler::GetSpotLight(id).m_light.m_dist.w;

				lightStruct->attenuation[0] = Aen::ComponentHandler::GetSpotLight(id).m_light.m_dist.x;
				lightStruct->attenuation[1] = Aen::ComponentHandler::GetSpotLight(id).m_light.m_dist.y;
				lightStruct->attenuation[2] = Aen::ComponentHandler::GetSpotLight(id).m_light.m_dist.z;

				lightStruct->angle = Aen::ComponentHandler::GetSpotLight(id).m_light.m_angle;
			}
			m_LightVector.push_back(*lightStruct);
		}
	}

	void LevelExporter::modelFunc(ModelStruct*& modelStruct, vector<Aen::Entity*>& entityList, unordered_map<size_t, ModelContainer>& modelMap, size_t& index)
	{
		size_t id = entityList[index]->GetID();

		unordered_map<size_t, ModelContainer>::iterator it = modelMap.find(id);

		if (modelMap.find(id) != modelMap.end())
		{
			strcpy(modelStruct->name, it->second.m_model.m_name.c_str());
			strcpy(modelStruct->mesh, it->second.m_model.m_meshName.c_str());

			modelStruct->translation[0] = entityList[index]->GetPos().x;
			modelStruct->translation[1] = entityList[index]->GetPos().y;
			modelStruct->translation[2] = entityList[index]->GetPos().z;

			modelStruct->rotation[0] = entityList[index]->GetRot().x;
			modelStruct->rotation[1] = entityList[index]->GetRot().y;
			modelStruct->rotation[2] = entityList[index]->GetRot().z;

			modelStruct->scale[0] = entityList[index]->GetScale().x;
			modelStruct->scale[1] = entityList[index]->GetScale().y;
			modelStruct->scale[2] = entityList[index]->GetScale().z;

			strcpy(modelStruct->type, it->second.m_type.c_str());
			strcpy(modelStruct->sound, it->second.m_sound.c_str());

			m_ModelVector.push_back(*modelStruct);
		}
	}

	void LevelExporter::roomFunc(RoomStruct*& roomStruct, string array[])
	{
		
		roomStruct->type = CheckValidType(array[0]);
		roomStruct->special = CheckValidType(array[1]);
		roomStruct->theme = CheckValidType(array[2]);
		roomStruct->probability = stoi(array[3].c_str());

		cout << roomStruct->type << endl;
		cout << roomStruct->special << endl;
		cout << roomStruct->theme << endl;
		cout << roomStruct->probability << endl;
	}

	void LevelExporter::textureFunc(TextureStruct*& textureStruct, string& textureFileName)
	{
		strcpy(textureStruct->name, textureFileName.c_str());
		strcpy(textureStruct->textureType, "test");
		m_TextureVector.push_back(*textureStruct);
	}

	int LevelExporter::CheckValidType(string& check)
	{

		for (std::pair<std::string, int> element : valid.map)
		{
			

			if (element.first == check)
			{
				return element.second;
			}
		}
		return -1;
	}

	LevelExporter::LevelExporter()
	{
		valid.insert(validType, typeValue, 14);
	}

	LevelExporter::~LevelExporter()
	{
		CloseFile();
	}

	void LevelExporter::OpenFile(string fileName)
	{
		m_outfile.open(m_filePath + fileName + ".Level", std::ofstream::out | std::ofstream::binary);

	}

	void LevelExporter::CloseFile()
	{
		if (m_outfile.is_open())
		{
			m_outfile.close();
		}
	}

	template<class T>
	inline void LevelExporter::WriteToFile(T* whatToWrite, std::ofstream& outfile)
	{
		outfile.write((const char*)&*whatToWrite, sizeof(T));
	}

	void LevelExporter::WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, unordered_map<size_t, ModelContainer>& modelMap, unordered_map<size_t, Aen::Entity*>& lightMap, string array[], string& fileName)
	{
		OpenFile(fileName);
		//cout << "writeInto" << endl;
		//cout << "entityList " << entityList.size() << endl;
		//cout << "itemList " << itemList.size() << endl;
		//cout << "meshObjList " << meshObjList.size() << endl;
		//cout << "textureFileName " << textureFileName.size() << endl;

		RoomHeader* roomHeader = AEN_NEW(RoomHeader);
		ModelHeader* modelHeader = AEN_NEW(ModelHeader);
		TextureHeader* textureHeader = AEN_NEW(TextureHeader);
		MaterialHeader* materialHeader = AEN_NEW(MaterialHeader);
		LightHeader* lightHeader = AEN_NEW(LightHeader);
		ParticleHeader* particleHeader = AEN_NEW(ParticleHeader);

		RoomStruct* roomStruct = AEN_NEW(RoomStruct);
		ModelStruct* modelStruct = AEN_NEW(ModelStruct);
		TextureStruct* textureStruct = AEN_NEW(TextureStruct);
		LightStruct* lightStruct = AEN_NEW(LightStruct);

		MaterialStruct* materialStruct = AEN_NEW(MaterialStruct);
		ParticleStruct* particleStruct = AEN_NEW(ParticleStruct);

		unordered_map <size_t, ModelContainer>::iterator it;

		m_ModelVector.reserve(modelMap.size());
		m_TextureVector.reserve(modelMap.size());
		m_LightVector.reserve(lightMap.size());

		for (size_t i = 0; i < entityList.size(); i++)
		{
			size_t id = entityList[i]->GetID();

			if (Aen::ComponentHandler::MeshInstanceExist(id))
			{
				modelFunc(modelStruct, entityList, modelMap, i);
			}
			else if ((Aen::ComponentHandler::DirectionalLightExist(id) || Aen::ComponentHandler::SpotLightExist(id) || Aen::ComponentHandler::PointLightExist(id)))
			{
				lightFunc(lightStruct, m_LightVector, entityList[i],lightMap);
			}
			cout << endl;
		}

		roomFunc(roomStruct, array);
		
		for (it = modelMap.begin(); it != modelMap.end(); it++)
		{
			textureFunc(textureStruct, it->second.m_texture.m_textureName);
		}


		cout << endl << "start" << endl;
		
		WriteToFile(roomHeader, m_outfile);
		WriteToFile(roomStruct, m_outfile);


		for (size_t i = 0; i < m_ModelVector.size(); i++)
		{
			cout << m_ModelVector[i].name << endl;
			cout << m_ModelVector[i].mesh << endl;
			cout << m_ModelVector[i].translation[0] << " " << m_ModelVector[i].translation[1] << " " << m_ModelVector[i].translation[2] << endl;
			cout << m_ModelVector[i].rotation[0] << " " << m_ModelVector[i].rotation[1] << " " << m_ModelVector[i].rotation[2] << endl;
			cout << m_ModelVector[i].scale[0] << " " << m_ModelVector[i].scale[1] << " " << m_ModelVector[i].scale[2] << endl;
			cout << m_ModelVector[i].type << endl;
			cout << m_ModelVector[i].sound << endl;

			WriteToFile(modelHeader, m_outfile);
			*modelStruct = m_ModelVector[i];
			WriteToFile(modelStruct, m_outfile);
		}

		for (size_t i = 0; i < m_TextureVector.size(); i++)
		{
			cout << m_TextureVector[i].name << endl;
			cout << m_TextureVector[i].textureType << endl;

			WriteToFile(textureHeader, m_outfile);
			*textureStruct = m_TextureVector[i];
			WriteToFile(textureStruct, m_outfile);
		}

		for (size_t i = 0; i < m_LightVector.size(); i++)
		{
			cout << m_LightVector[i].type << endl;
			cout << m_LightVector[i].translation[0] << " " << m_LightVector[i].translation[1] << " " << m_LightVector[i].translation[2] << endl;
			cout << m_LightVector[i].rotation[0] << " " << m_LightVector[i].rotation[1] << " " << m_LightVector[i].rotation[2] << endl;
			cout << m_LightVector[i].direction[0] << " " << m_LightVector[i].direction[1] << " " << m_LightVector[i].direction[2] << endl;
			cout << m_LightVector[i].intensity << endl;
			cout << m_LightVector[i].color[0] << " " << m_LightVector[i].color[1] << " " << m_LightVector[i].color[2] << endl;
			cout << m_LightVector[i].range << endl;
			cout << "att " << m_LightVector[i].attenuation[0] << " " << m_LightVector[i].attenuation[1] << " " << m_LightVector[i].attenuation[2] << endl;
			cout << m_LightVector[i].angle;

			WriteToFile(lightHeader, m_outfile);
			*lightStruct = m_LightVector[i];
			WriteToFile(lightStruct, m_outfile);
		}

		m_ModelVector.clear();
		m_TextureVector.clear();
		m_MaterialVector.clear();
		m_LightVector.clear();
		m_ParticleVector.clear();
		m_outfile.close();
		
		delete roomHeader;
		delete roomStruct;

		delete modelHeader;
		delete modelStruct;

		delete textureHeader;
		delete textureStruct;

		delete lightHeader;
		delete lightStruct;
		
		delete materialHeader;
		delete materialStruct;

		delete particleHeader;
		delete particleStruct;

		CloseFile();
	}

	LIGHTTYPES LevelExporter::Type(Aen::Entity* entity)
	{
		size_t id = entity->GetID();

		LIGHTTYPES type;
		
		if (Aen::ComponentHandler::DirectionalLightExist(id))
		{
			type = LIGHTTYPES::DIRECTIONAL;
		}
		else if (Aen::ComponentHandler::SpotLightExist(id))
		{
			type = LIGHTTYPES::SPOT;
		}
		else if (Aen::ComponentHandler::PointLightExist(id))
		{
			type = LIGHTTYPES::POINT;
		}
		else
		{
			type = LIGHTTYPES::NOLIGHT;
		}

		return type;
	}
}
