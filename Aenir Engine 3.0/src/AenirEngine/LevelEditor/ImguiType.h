#pragma once
#include <string>
#include <unordered_map>

namespace IGH 
{
	using std::string;
	using std::unordered_map;

	const string OPEN = "Open";
	const string CLOSE = "Close";
	const string SAVE = "Save";
	const string COPY = "Copy";

	const string TRANSLATION = "Translate";
	const string ROTATION = "Rotate";
	const string SCALE = "Scale";



	const string FBX = "fbx";
	const string OBJ = "obj";
	const string PNG = "png";
	const string JPG = "jpg";
	const string LEVEL = "Level";

	const string RESOURCEPATH = "../Resource/";
	const string LEVELPATH = "../LevelFolder/";

	const string LIGHT = "Light";
	const string POINTLIGHT = "Point Light";
	const string SPOTLIGHT = "Spot Light";
	const string DIRECTIONALLIGHT = "Directional Light";
	const string CAMERA = "Camera";
	const string MODEL = "Model";
	const string MATERIAL = "Material";
	const string TEXTURE = "Texture";
	const string BOSS = "Boss Enemy";
	const string NORMALENEMY = "Normal Enemy";
	const string COLOR = "Color";


	static const char* ROOMTHEME[] = { "Normal Dungeon", "Skeleton", "Gothic", "Aztec" };
	static const char* ROOMTYPE[] = { "Straight", "Bend", "T Junction" ,"Four Way", "None" };
	static const char* SPECIALROOM[] = { "Normal", "Entrance", "Exit", "Boss", "Arena", "Item" };
	static const char* HITBOXTYPE[] = { "None","Static", "Dynamic" };
	static const char* ENEMYTYPE[] = { "Normal Enemy", "Boss Enemy" };
	static const char* ModelType[] = { "Prop", "Walls" };
	static const char* TOOLS[] = { "Move","Rotate","Scale" };

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
		bool rigidBody = false;
		string rigidBodyType = "";
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
		}

		ModelContainer(string materialTextureName, string materialName, string textureName, string modelName, string meshName, string type, bool rigidBody, string rigidBodyType) {
			this->m_material.m_materialTextureName = materialTextureName;
			this->m_material.m_materialName = materialName;
			this->m_texture.m_textureName = textureName;
			this->m_model.m_name = modelName;
			this->m_model.m_meshName = meshName;
			this->m_type = type;
			this->m_model.rigidBody = rigidBody;
			this->m_model.rigidBodyType = rigidBodyType;
		}

		void update(string& materialTextureName, string& materialName, string& textureName, string& modelName)
		{

			if (materialTextureName != "")
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
		}
	};

	struct RoomValues
	{
		unordered_map<string, int> map;

		void insert(string key[], int value[], int size)
		{
			for (int i = 0; i < size; i++)
			{
				map.insert(std::make_pair(key[i], value[i]));
			}
		}

	};


}

