#include "PCH.h"
#include "ImGuiImporter.h"
namespace Aen
{

	void ImGuiImporter::Convert(const Aen::Vec4f inputVec, float* inputArray)
	{
		inputArray[0] = inputVec.x;
		inputArray[1] = inputVec.y;
		inputArray[2] = inputVec.z;
		inputArray[3] = inputVec.w;
	}

	void ImGuiImporter::Convert(float* inputArray, Aen::Vec4f& inputVec)
	{
		inputVec = Color(inputArray[0], inputArray[1], inputArray[2], inputArray[3]);
	}

	void ImGuiImporter::setMaterial(Aen::Material& materialOut, AenIF::Material materialIn)
	{
		Convert(materialIn.baseColor, materialOut["BaseColor"]);
		Convert(materialIn.shadowColor, materialOut["ShadowColor"]);
		Convert(materialIn.specularColor, materialOut["SpecularColor"]);
		Convert(materialIn.rimLightColor, materialOut["RimLightColor"]);
		Convert(materialIn.innerEdgeColor, materialOut["InnerEdgeColor"]);
		Convert(materialIn.outerEdgeColor, materialOut["OuterEdgeColor"]);
		Convert(materialIn.glowColor, materialOut["GlowColor"]);

		materialOut["GlowStr"] = materialIn.glowStr;
		materialOut["InnerEdgeThickness"] = static_cast<int>(materialIn.innerEdgeThickness);
		materialOut["OuterEdgeThickness"] = static_cast<int>(materialIn.outerEdgeThickness);
		materialOut["SpecularPower"] = materialIn.specularPower;
		materialOut["SpecularStrength"] = materialIn.specularStrength;
		materialOut["Roughness"] = materialIn.roughness;
		materialOut["ShadowOffset"] = materialIn.shadowOffset;
		materialOut["InnerFalloff"] = materialIn.innerFalloff;
		materialOut["OuterFalloff"] = materialIn.outerFalloff;
		materialOut["RimLightIntensity"] = materialIn.rimLightIntensity;
		materialOut["RimLightSize"] = materialIn.rimLightSize;
	}

	void ImGuiImporter::addBaseCommon(Aen::Entity*& entity, Aen::Mesh*& mesh, Aen::Material*& material, Aen::Texture*& materialTexture, AenIF::Model& model, AenIF::Texture& texture, AenIF::Material& materialIn)
	{
		string imageName = AEN_RESOURCE_DIR(texture.name);
		entity = &mp_entityHandlerPtr->CreateEntity();
		mesh = &Aen::Resource::CreateMesh(model.name);
		mesh->Load(AEN_RESOURCE_DIR(model.mesh));

		string materialName = materialIn.materialName;
		string textureName = materialIn.materialTextureName;

		if (IfExist(*m_materialList, materialIn) == false)
		{
			m_materialList->push_back(IGH::MatTexName(materialIn.materialName, materialIn.materialTextureName));

			materialTexture = &Aen::Resource::CreateTexture(textureName);
			materialTexture->LoadTexture(imageName);

			material = &Aen::Resource::CreateMaterial(materialName, true);
			setMaterial(*material, materialIn);
			material->SetDiffuseMap(*materialTexture);
		}
		else
		{
			material = &Aen::Resource::GetMaterial(materialName);
			setMaterial(*material, materialIn);
		}

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(*mesh);

		if (model.rigidBody && model.rigidBodyType != IGH::HITBOXTYPE[0]) // Check if should have rigidbody
		{
			entity->AddComponent<Aen::StaticBody>();
			entity->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);
		}

		entity->SetPos(model.translation[0], model.translation[1], model.translation[2]);
		entity->SetRot(model.rotation[0], model.rotation[1], model.rotation[2]);
		entity->SetScale(model.scale[0], model.scale[1], model.scale[2]);


	}

	ImGuiImporter::ImGuiImporter()
	{
		this->m_entityList = new vector<Aen::Entity*>;
		this->m_itemList = new vector<string>;
		this->m_modelMap = new unordered_map< size_t, IGH::ModelContainer>;
		this->m_lightMap = new unordered_map< size_t, Aen::Entity*>;
		this->m_levelImporter = new AenIMP::LevelImporter();
		this->m_materialList = new vector<IGH::MatTexName>;
		m_standAlone = true;
	}

	ImGuiImporter::ImGuiImporter(vector<Aen::Entity*>* m_entityList, vector<string>* m_itemList, unordered_map<size_t, IGH::ModelContainer>* m_modelMap, unordered_map<size_t, Aen::Entity*>* m_lightMap, AenIMP::LevelImporter* m_levelImporter, vector<IGH::MatTexName>* m_materialList)
	{
		this->m_entityList = m_entityList;
		this->m_itemList = m_itemList;
		this->m_modelMap = m_modelMap;
		this->m_lightMap = m_lightMap;
		this->m_levelImporter = m_levelImporter;
		this->m_materialList = m_materialList;

		m_standAlone = false;
	}

	ImGuiImporter::~ImGuiImporter()
	{
		if (m_standAlone == true)
		{
			if (m_levelImporter != nullptr)
			{
				delete m_levelImporter;
			}

			if (m_itemList != nullptr)
			{
				delete m_itemList;
			}

			if (m_entityList != nullptr)
			{
				delete m_entityList;
			}
			if (m_modelMap != nullptr)
			{
				delete m_modelMap;
			}
			if (m_lightMap != nullptr)
			{
				delete m_lightMap;
			}
			if (m_materialList != nullptr)
			{
				delete m_materialList;
			}
		}
	}

	bool ImGuiImporter::import(AenIMP::LevelImporter& m_levelImporter, string& levelPath, float* translation, float* rotation, float* scale)
	{
		m_levelImporter.ReadFromFile(levelPath);
		
		size_t id;

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[0].GetModelVector().size(); i++)
		{
			if (m_levelImporter.GetRoomVector()[0].GetMaterialVector().size() > 0)
			{
				id = AddBase(m_levelImporter.GetRoomVector()[0].GetModelVector()[i], m_levelImporter.GetRoomVector()[0].GetTextureVector()[i], m_levelImporter.GetRoomVector()[0].GetMaterialVector()[i]);
			}
			/*else
			{
				id = AddBase(m_levelImporter->GetRoomVector()[0].GetModelVector()[i], m_levelImporter->GetRoomVector()[0].GetTextureVector()[i]);

			}*/

			float tX = 0, tY = 0, tZ = 0;
			float rX = 0, rY = 0, rZ = 0;
			float sX = 0, sY = 0, sZ = 0;

			GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].translation, tX, tY, tZ);
			GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].rotation, rX, rY, rZ);
			GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].scale, sX, sY, sZ);

			mp_entityHandlerPtr->GetEntity(id).SetPos(((tX + translation[0])) * scale[0], ((tY + translation[1])) * scale[1], ((tZ + translation[2])) * scale[2]);
			mp_entityHandlerPtr->GetEntity(id).SetRot(rX + rotation[0], rY + rotation[1], rZ + rotation[2]);
			mp_entityHandlerPtr->GetEntity(id).SetScale(scale[0] * sX, scale[1] * sY, scale[2] * sZ);

		}

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[0].GetLightVector().size(); i++)
		{
			if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == IGH::DIRECTIONALLIGHT.c_str())
			{
				size_t id = AddDirectional(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == IGH::SPOTLIGHT.c_str())
			{
				size_t id = AddSpotLight(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);

				float tX = 0, tY = 0, tZ = 0;
				float rX = 0, rY = 0, rZ = 0;

				GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].translation, tX, tY, tZ);
				GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].rotation, rX, rY, rZ);

				mp_entityHandlerPtr->GetEntity(id).SetPos(tX + translation[0], tY + translation[1], tZ + translation[2]);
				mp_entityHandlerPtr->GetEntity(id).SetRot(rX + rotation[0], rY + rotation[1], rZ + rotation[2]);
			}
			else if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == IGH::POINTLIGHT.c_str())
			{
				size_t id = AddPointLight(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);

				float tX = 0, tY = 0, tZ = 0;

				GetFloatArray(m_levelImporter.GetRoomVector()[0].GetLightVector()[i].translation, tX, tY, tZ);

				mp_entityHandlerPtr->GetEntity(id).SetPos(tX + translation[0], tY + translation[1], tZ + translation[2]);
			}
		}
		m_levelImporter.GetRoomVector().clear();

		return false;
	}

	bool ImGuiImporter::import(string& levelPath)
	{
		m_levelImporter->ReadFromFile(levelPath);
		size_t id;

		for (size_t i = 0; i < m_levelImporter->GetRoomVector()[0].GetModelVector().size(); i++)
		{
			size_t id = AddBase(m_levelImporter->GetRoomVector()[0].GetModelVector()[i],
				m_levelImporter->GetRoomVector()[0].GetTextureVector()[i], m_levelImporter->GetRoomVector()[0].GetMaterialVector()[i]);

			float tX = 0, tY = 0, tZ = 0;
			float rX = 0, rY = 0, rZ = 0;
			float sX = 0, sY = 0, sZ = 0;

			GetFloatArray(m_levelImporter->GetRoomVector()[0].GetModelVector()[i].translation, tX, tY, tZ);
			GetFloatArray(m_levelImporter->GetRoomVector()[0].GetModelVector()[i].rotation, rX, rY, rZ);
			GetFloatArray(m_levelImporter->GetRoomVector()[0].GetModelVector()[i].scale, sX, sY, sZ);

			mp_entityHandlerPtr->GetEntity(id).SetPos(tX , tY , tZ );
			mp_entityHandlerPtr->GetEntity(id).SetRot(rX , rY, rZ);
			mp_entityHandlerPtr->GetEntity(id).SetScale(sX, sY, sZ);

		}

		for (size_t i = 0; i < m_levelImporter->GetRoomVector()[0].GetLightVector().size(); i++)
		{
			if (m_levelImporter->GetRoomVector()[0].GetLightVector()[i].type == IGH::DIRECTIONALLIGHT.c_str())
			{
				size_t id = AddDirectional(m_levelImporter->GetRoomVector()[0].GetLightVector()[i]);
			}
			else if (m_levelImporter->GetRoomVector()[0].GetLightVector()[i].type == IGH::SPOTLIGHT.c_str())
			{
				size_t id = AddSpotLight(m_levelImporter->GetRoomVector()[0].GetLightVector()[i]);

				float tX = 0, tY = 0, tZ = 0;
				float rX = 0, rY = 0, rZ = 0;

				GetFloatArray(m_levelImporter->GetRoomVector()[0].GetModelVector()[i].translation, tX, tY, tZ);
				GetFloatArray(m_levelImporter->GetRoomVector()[0].GetModelVector()[i].rotation, rX, rY, rZ);

				mp_entityHandlerPtr->GetEntity(id).SetPos(tX , tY , tZ );
				mp_entityHandlerPtr->GetEntity(id).SetRot(rX , rY , rZ);
			}
			else if (m_levelImporter->GetRoomVector()[0].GetLightVector()[i].type == IGH::POINTLIGHT.c_str())
			{
				size_t id = AddPointLight(m_levelImporter->GetRoomVector()[0].GetLightVector()[i]);

				float tX = 0, tY = 0, tZ = 0;

				GetFloatArray(m_levelImporter->GetRoomVector()[0].GetLightVector()[i].translation, tX, tY, tZ);

				mp_entityHandlerPtr->GetEntity(id).SetPos(tX , tY, tZ);
			}
		}
		m_levelImporter->GetRoomVector().clear();

		return false;
	}

	bool ImGuiImporter::IfExist(vector<IGH::MatTexName>& matList, AenIF::Material& value)
	{
		bool result = false;

		for (size_t i = 0; i < matList.size(); i++)
		{
			if ((matList[i].matName == value.materialName) && (matList[i].texName == value.materialTextureName))
			{
				result = true;
			}
			else if ((matList[i].matName == value.materialName))
			{
				result = true;
			}
		}

		return result;
	}


	void ImGuiImporter::GetFloatArray(float* inputArray, float& x, float& y, float& z)
	{
		x = inputArray[0];
		y = inputArray[1];
		z = inputArray[2];
	}

	void ImGuiImporter::ReadAllFilesFromResourceFolder()
	{
		string filePath = IGH::LEVELPATH;
		string fileName = "";
		string fileType = "";

		for (const auto& entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);

			if (fileType == "Level")
			{
				m_levelImporter->ReadFromFile(filePath + fileName);
			}
		}
	}

	AenIMP::LevelImporter* ImGuiImporter::GetImporterPtr()
	{
		return m_levelImporter;
	}

	AenIF::Room ImGuiImporter::GetRoom(size_t index)
	{
		return m_levelImporter->GetRoomVector()[index].GetRoom();
	}

	vector<Aen::Entity*>* ImGuiImporter::GetEntityList()
	{
		return m_entityList;
	}

	bool ImGuiImporter::LoadLevel(int index)
	{
		if (index >= m_levelImporter->GetRoomVector().size())
			return false;
		for (size_t i = 0; i < m_levelImporter->GetRoomVector()[index].GetModelVector().size(); i++)
		{
			AddBase(m_levelImporter->GetRoomVector()[index].GetModelVector()[i], m_levelImporter->GetRoomVector()[index].GetTextureVector()[i], m_levelImporter->GetRoomVector()[index].GetMaterialVector()[index]);
		}

		for (size_t i = 0; i < m_levelImporter->GetRoomVector()[index].GetLightVector().size(); i++)
		{
			if (m_levelImporter->GetRoomVector()[index].GetLightVector()[i].type == "Directional light")
			{

				AddDirectional(m_levelImporter->GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter->GetRoomVector()[index].GetLightVector()[i].type == "Spot light")
			{
				cout << "s Light " << endl;

				AddSpotLight(m_levelImporter->GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter->GetRoomVector()[index].GetLightVector()[i].type == "Point light")
			{
				cout << "p Light " << endl;

				AddPointLight(m_levelImporter->GetRoomVector()[index].GetLightVector()[i]);
			}
		}
		return true;
	}

	bool ImGuiImporter::LoadLevel(AenIMP::CompleteRoom* roomPtr, Aen::Vec2f offset, float angle)
	{
		//if (index >= m_levelImporter.GetRoomVector().size())
		//	return false;
		for (size_t i = 0; i < roomPtr->GetModelVector().size(); i++)
		{
			cout << "Model " << endl;
			AddBase(roomPtr->GetModelVector()[i], roomPtr->GetTextureVector()[i], offset, angle, roomPtr->GetMaterialVector()[i]);
		}

		for (size_t i = 0; i < roomPtr->GetLightVector().size(); i++)
		{
			if (roomPtr->GetLightVector()[i].type == "Directional light")
			{
				cout << "d Light " << endl;

				AddDirectional(roomPtr->GetLightVector()[i]);
			}
			else if (roomPtr->GetLightVector()[i].type == "Spot light")
			{
				cout << "s Light " << endl;

				AddSpotLight(roomPtr->GetLightVector()[i], offset, angle);
			}
			else if (roomPtr->GetLightVector()[i].type == "Point light")
			{
				cout << "p Light " << endl;

				AddPointLight(roomPtr->GetLightVector()[i], offset, angle);
			}
		}
		return true;
	}

	size_t ImGuiImporter::AddBase(AenIF::Model& model, AenIF::Texture& texture, AenIF::Material& materialIn)
	{
		Aen::Entity* entity;
		Aen::Mesh* mesh;
		Aen::Material* material;
		Aen::Texture* materialTexture;

		addBaseCommon(entity, mesh, material, materialTexture, model, texture, materialIn);

		size_t id = entity->GetID();
		Aen::ComponentHandler::GetMeshInstance(static_cast<size_t>(id)).SetMaterial(*material);


		AddEnemy(entity, model);
		AddModel(entity, model.name);
		m_modelMap->insert(std::make_pair(entity->GetID(), IGH::ModelContainer(materialIn, texture.name, model.name, model.mesh, model.type, model.rigidBody, model.rigidBodyType)));
		m_modelMap->at(id).m_model.m_castShadow = model.castShadow;

		return id;
	}

	size_t ImGuiImporter::AddBase(const string& meshName, const string& objName)
	{
		string imageName = AEN_RESOURCE_DIR("Missing_Textures.png");
		string materialName = "Material" + to_string(m_entityCount);
		string textureName = "Texture" + to_string(m_entityCount);


		Aen::Entity* entity = &mp_entityHandlerPtr->CreateEntity();
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(meshName + std::to_string(m_entityCount));
		mesh.Load(AEN_RESOURCE_DIR(objName));

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);


		//AddModel(entity, meshName);
		AddModel(entity, meshName);

		m_modelMap->insert(std::make_pair(entity->GetID(), IGH::ModelContainer(textureName, materialName, imageName, meshName, objName)));

		size_t id = entity->GetID();

		return id;
	}

	void ImGuiImporter::AddModel(Aen::Entity* entity, string name)
	{
		m_entityList->push_back(entity);
		m_itemList->push_back(name + std::to_string(m_entityCount));
		//m_selected.push_back(false);
		//m_selectedEntities.push_back(-1);
		m_entityCount++;
	}

	void ImGuiImporter::AddModel(Aen::Entity* entity)
	{
		m_entityList->push_back(entity);
		m_itemList->push_back(IGH::MODEL + std::to_string(m_entityCount));
		//m_selected.push_back(false);
		//m_selectedEntities.push_back(-1);
		m_entityCount++;
	}

	void ImGuiImporter::AddEnemy(Aen::Entity* entity, AenIF::Model& model)
	{
		if (model.type == IGH::NORMALENEMY)
		{
			OutputDebugStringA(IGH::NORMALENEMY.c_str());
		}
		else if (model.type == IGH::BOSS.c_str())
		{
			OutputDebugStringA(IGH::NORMALENEMY.c_str());
		}
	}

	void ImGuiImporter::AddLight(Aen::Entity* entity)
	{
		m_entityList->push_back(entity);
		m_itemList->push_back(IGH::LIGHT + std::to_string(m_lightCount));
		m_lightCount++;
	}

	void ImGuiImporter::AddLight(Aen::Entity* entity, string type)
	{
		m_lightMap->insert(std::make_pair(entity->GetID(), entity));
		m_entityList->push_back(entity);
		m_itemList->push_back(type + std::to_string(m_lightCount));
		m_lightCount++;
	}

	size_t ImGuiImporter::AddPointLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();


		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::PointLight>().SetStrength(input.intensity);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);

		AddLight(light, IGH::POINTLIGHT);

		return light->GetID();
	}

	void ImGuiImporter::AddPointLight()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 1);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(0.0f, 0.0f, 0.0f);

		AddLight(light, IGH::POINTLIGHT);
	}

	size_t ImGuiImporter::AddSpotLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();
		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::SpotLight>().SetConeSize(input.angle);
		light->GetComponent<Aen::SpotLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::SpotLight>().SetStrength(input.intensity);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);

		AddLight(light, IGH::SPOTLIGHT);
		return light->GetID();
	}

	void ImGuiImporter::AddSpotLight()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::SpotLight>().SetConeSize(1);
		light->GetComponent<Aen::SpotLight>().SetLightDist(1, 1, 1, 1);
		light->GetComponent<Aen::SpotLight>().SetStrength(100);
		light->SetPos(0.0f, 0.0f, 0.0f);
		light->SetRot(0.0f, 0.0f, 0.0f);

		AddLight(light, IGH::SPOTLIGHT);
	}

	size_t ImGuiImporter::AddDirectional(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(input.intensity);
		light->SetRot(input.direction[0], input.direction[1], input.direction[2]);

		AddLight(light, IGH::DIRECTIONALLIGHT);
		return light->GetID();
	}

	void ImGuiImporter::AddDirectional()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(1);
		light->SetRot(0.0f, 0.0f, 0.0f);

		AddLight(light, IGH::DIRECTIONALLIGHT);
	}

	void ImGuiImporter::AddBase(AenIF::Model& model, AenIF::Texture& texture, Aen::Vec2f offset, float angle, AenIF::Material& materialIn)
	{
		Aen::Entity* entity;
		Aen::Mesh* mesh;
		Aen::Material* material;
		Aen::Texture* materialTexture;

		float s = sin(angle);
		float c = cos(angle);

		float posX = ((model.translation[0]) * c) - ((model.translation[2]) * s);
		float posZ = ((model.translation[0]) * s) + ((model.translation[2]) * c);

		AenIF::Model temp = model;

		temp.translation[0] = posX + offset.x;
		temp.translation[2] = posZ + offset.y;
		temp.rotation[1] = model.rotation[1] + (angle * 57.2957795);

		addBaseCommon(entity, mesh, material, materialTexture, temp, texture, materialIn);

		size_t id = entity->GetID();
		Aen::ComponentHandler::GetMeshInstance(static_cast<uint32_t>(id)).SetMaterial(*material);

		AddModel(entity);
		AddModel(entity, model.name);

		m_modelMap->insert(std::make_pair(entity->GetID(), IGH::ModelContainer(materialIn, texture.name, model.name, model.mesh, model.type, model.rigidBody, model.rigidBodyType)));
		m_modelMap->at(id).m_model.m_castShadow = model.castShadow;

	}

	void ImGuiImporter::AddPointLight(AenIF::Light& input, Aen::Vec2f offset, float angle)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 5);
		light->GetComponent<Aen::PointLight>().SetStrength(100);

		float s = sin(angle);
		float c = cos(angle);

		float posX = (input.translation[0] * c) - (input.translation[2] * s);
		float posY = (input.translation[0] * s) + (input.translation[2] * c);

		light->SetPos(posX + offset.x, input.translation[1], posY + offset.y);
		light->SetRot(input.rotation[0], input.rotation[1] + (angle * 57.2957795), input.rotation[2]);

		AddLight(light);
	}

	void ImGuiImporter::AddSpotLight(AenIF::Light& input, Aen::Vec2f offset, float angle)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::SpotLight>().SetConeSize(input.angle);
		light->GetComponent<Aen::SpotLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::SpotLight>().SetStrength(input.intensity);

		float s = sin(angle);
		float c = cos(angle);

		float posX = (input.translation[0] * c) - (input.translation[2] * s);
		float posY = (input.translation[0] * s) + (input.translation[2] * c);

		light->SetPos(posX + offset.x, input.translation[1], posY + offset.y);
		light->SetRot(input.rotation[0], input.rotation[1] + (angle * 57.2957795), input.rotation[2]);

		AddLight(light);

	}

}