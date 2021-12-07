#include "PCH.h"
#include "LevelExporter.h"

namespace Aen {
	void LevelExporter::lightFunc(LightStruct*& lightStruct, 
		vector<LightStruct>& m_LightVector, Entity*& entity, unordered_map<size_t, Aen::Entity*>& lightMap)
	{
		size_t id = entity->GetID();
		IGH::LIGHTTYPES type = Type(entity);

		if (lightMap.find(id) != lightMap.end())
		{
			if (type == IGH::LIGHTTYPES::DIRECTIONAL)
			{
				strcpy(lightStruct->type, IGH::DIRECTIONALLIGHT.c_str());

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

			if (type == IGH::LIGHTTYPES::POINT)
			{
				strcpy(lightStruct->type, IGH::POINTLIGHT.c_str());

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

			if (type == IGH::LIGHTTYPES::SPOT)
			{
				strcpy(lightStruct->type, IGH::SPOTLIGHT.c_str());
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

	void LevelExporter::modelFunc(ModelStruct*& modelStruct, vector<Aen::Entity*>& entityList, unordered_map<size_t, IGH::ModelContainer>& modelMap, size_t& index, ParticleStruct*& particleStruct)
	{
		size_t id = entityList[index]->GetID();

		unordered_map<size_t, IGH::ModelContainer>::iterator it = modelMap.find(id);

		if (modelMap.find(id) != modelMap.end())
		{
			IGH::ImguiTypes temp;
			if (temp.GetParticleTag(it->second.m_type) == false)
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
				modelStruct->rigidBody = it->second.m_model.rigidBody;
				strcpy(modelStruct->rigidBodyType, it->second.m_model.rigidBodyType.c_str());
				modelStruct->castShadow = it->second.m_model.m_castShadow;

				m_ModelVector.push_back(*modelStruct);
			}
			else
			{
				particleStruct->translation[0] = entityList[index]->GetPos().x;
				particleStruct->translation[1] = entityList[index]->GetPos().y;
				particleStruct->translation[2] = entityList[index]->GetPos().z;
				strcpy(particleStruct->type, it->second.m_type.c_str());
				m_ParticleVector.push_back(*particleStruct);
			}
		}
	}

	void LevelExporter::roomFunc(RoomStruct*& roomStruct, string array[])
	{
		
		roomStruct->type = CheckValidType(array[0]);
		roomStruct->special = CheckValidType(array[1]);
		roomStruct->theme = CheckValidType(array[2]);
		roomStruct->probability = stoi(array[3].c_str());
		roomStruct->size = stoi(array[4].c_str());

	}

	void LevelExporter::textureFunc(TextureStruct*& textureStruct, string& textureFileName, string& normalTextureFileName)
	{
		strcpy(textureStruct->texture, textureFileName.c_str());
		strcpy(textureStruct->normalTexture, normalTextureFileName.c_str());
		m_TextureVector.push_back(*textureStruct);
	}

	void LevelExporter::materialFunc(MaterialStruct*& materialStruct, vector<Aen::Entity*>& entityList, unordered_map<size_t, IGH::ModelContainer>& modelMap, size_t& index)
	{
		size_t id = entityList[index]->GetID();

		unordered_map<size_t, IGH::ModelContainer>::iterator it = modelMap.find(id);

		if (modelMap.find(id) != modelMap.end())
		{
			strcpy(materialStruct->materialName, it->second.m_material.m_materialName.c_str());
			strcpy(materialStruct->materialTextureName, it->second.m_material.m_materialTextureName.c_str());

			for (int i = 0; i < 4; i++)
			{
				materialStruct->baseColor[i] = it->second.m_material.m_baseColor[i];
				materialStruct->shadowColor[i] = it->second.m_material.m_shadowColor[i];
				materialStruct->specularColor[i] = it->second.m_material.m_specularColor[i];
				materialStruct->rimLightColor[i] = it->second.m_material.m_rimLightColor[i];
				materialStruct->innerEdgeColor[i] = it->second.m_material.m_innerEdgeColor[i];
				materialStruct->outerEdgeColor[i] = it->second.m_material.m_outerEdgeColor[i];
				materialStruct->glowColor[i] = it->second.m_material.m_glowColor[i];
			}

			materialStruct->glowStr = it->second.m_material.m_glowStr;
			materialStruct->innerEdgeThickness = it->second.m_material.m_innerEdgeThickness;
			materialStruct->outerEdgeThickness = it->second.m_material.m_outerEdgeThickness;
			materialStruct->specularPower = it->second.m_material.m_specularPower;
			materialStruct->specularStrength = it->second.m_material.m_specularStrength;
			materialStruct->roughness = it->second.m_material.m_roughness;
			materialStruct->shadowOffset = it->second.m_material.m_roughness;
			materialStruct->innerFalloff = it->second.m_material.m_innerFalloff;
			materialStruct->outerFalloff = it->second.m_material.m_roughness;
			materialStruct->rimLightIntensity = it->second.m_material.m_rimLightIntensity;
			materialStruct->rimLightSize = it->second.m_material.m_rimLightSize;

			m_MaterialVector.push_back(*materialStruct);
		}
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
		valid.insert(validType, typeValue, 15);
	}

	LevelExporter::~LevelExporter()
	{
		CloseFile();
	}

	void LevelExporter::OpenFile(string fileName)
	{
		m_outfile.open(IGH::LEVELPATH + fileName + "." + IGH::LEVEL, std::ofstream::out | std::ofstream::binary);
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

	void LevelExporter::WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, unordered_map<size_t, IGH::ModelContainer>& modelMap, unordered_map<size_t, Aen::Entity*>& lightMap, string array[], string& fileName)
	{
		OpenFile(fileName);

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

		unordered_map <size_t, IGH::ModelContainer>::iterator it;

		m_ModelVector.reserve(modelMap.size());
		m_TextureVector.reserve(modelMap.size());
		m_LightVector.reserve(lightMap.size());

		for (size_t i = 0; i < entityList.size(); i++)
		{
			size_t id = entityList[i]->GetID();

			if (Aen::ComponentHandler::MeshInstanceExist(id))
			{
				modelFunc(modelStruct, entityList, modelMap, i, particleStruct);
				materialFunc(materialStruct, entityList, modelMap, i);
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
			textureFunc(textureStruct, it->second.m_texture.m_textureName, it->second.m_texture.m_normalTexture);
		}
		
		WriteToFile(roomHeader, m_outfile);
		WriteToFile(roomStruct, m_outfile);


		for (size_t i = 0; i < m_ModelVector.size(); i++)
		{
			WriteToFile(modelHeader, m_outfile);
			*modelStruct = m_ModelVector[i];
			WriteToFile(modelStruct, m_outfile);

			WriteToFile(materialHeader, m_outfile);
			*materialStruct = m_MaterialVector[i];
			WriteToFile(materialStruct, m_outfile);
		}

		for (size_t i = 0; i < m_TextureVector.size(); i++)
		{
			WriteToFile(textureHeader, m_outfile);
			*textureStruct = m_TextureVector[i];
			WriteToFile(textureStruct, m_outfile);
		}

		for (size_t i = 0; i < m_LightVector.size(); i++)
		{
			WriteToFile(lightHeader, m_outfile);
			*lightStruct = m_LightVector[i];
			WriteToFile(lightStruct, m_outfile);
		}

		for (size_t i = 0; i < m_ParticleVector.size(); i++)
		{
			WriteToFile(particleHeader, m_outfile);
			*particleStruct = m_ParticleVector[i];
			WriteToFile(particleStruct, m_outfile);
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

	IGH::LIGHTTYPES LevelExporter::Type(Aen::Entity* entity)
	{
		size_t id = entity->GetID();

		IGH::LIGHTTYPES type;
		
		if (Aen::ComponentHandler::DirectionalLightExist(id))
		{
			type = IGH::LIGHTTYPES::DIRECTIONAL;
		}
		else if (Aen::ComponentHandler::SpotLightExist(id))
		{
			type = IGH::LIGHTTYPES::SPOT;
		}
		else if (Aen::ComponentHandler::PointLightExist(id))
		{
			type = IGH::LIGHTTYPES::POINT;
		}
		else
		{
			type = IGH::LIGHTTYPES::NOLIGHT;
		}

		return type;
	}
}
