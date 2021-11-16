#pragma once
#include <string>
#include <unordered_map>
#include "RoomFormat.h"

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

	const string TYPE = "Type";


	const string FBX = "fbx";
	const string OBJ = "obj";
	const string PNG = "png";
	const string JPG = "jpg";
	const string LEVEL = "Level";

	const string RESOURCEPATH = "../Resource/";
	const string LEVELPATH = "../LevelFolder/";
	const string NAME = "Name";
	const string CREATE = "Create";

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

	const string BASECOLOR = "BaseColor";
	const string SPECULARCOLOR = "SpecularColor";
	const string SPECULARPOWER = "SpecularPower";
	const string SPECULARSTRENGTH = "SpecularStrength";
	const string ROUGHNESS = "Roughness";

	const string DEFAULTMATERIAL = "DefaultMaterial";
	const string PARTICLE = "Particle";

	static const char* TAGS[] = { "Light","Model","Enemy", "Weapon","Chest"};
	static const char* PARTICLETAG[] = {"torch"};

	static const char* ROOMTHEME[] = { "Normal Dungeon", "Skeleton", "Gothic", "Aztec" };
	static const char* ROOMTYPE[] = { "Straight", "Bend", "T Junction" ,"Four Way", "None" };
	static const char* SPECIALROOM[] = { "Normal", "Entrance", "Exit", "Boss", "Arena", "Item" };
	static const char* HITBOXTYPE[] = { "None","Static", "Dynamic" };
	static const char* ENEMYTYPE[] = { "Normal Enemy", "Boss Enemy" };
	static const char* ModelType[] = { "Prop", "Walls" };
	static const char* TOOLS[] = { "Move","Rotate","Scale" };

	struct MatTexName
	{
		string matName;
		string texName;
		MatTexName()
		{

		}

		MatTexName(string mat,string tex)
		{
			this->matName = mat;
			this->texName = tex;
		}

	};

	
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
		bool m_castShadow = false;
	};

	struct MaterialInfo
	{
	private:

		void ConvertToFloatArray(const Aen::Vec4f inputVec, float* inputArray)
		{
			inputArray[0] = inputVec.x;
			inputArray[1] = inputVec.y;
			inputArray[2] = inputVec.z;
			inputArray[3] = inputVec.w;
		}


	public:
		
		MaterialInfo() 
		{

		}

		MaterialInfo(Aen::Material &mat, string materialTextureName, string materialName)
		{
			m_materialTextureName = materialTextureName;
			m_materialName = materialName;
			ConvertToFloatArray(mat["BaseColor"], m_baseColor);
			ConvertToFloatArray(mat["ShadowColor"], m_shadowColor);
			ConvertToFloatArray(mat["SpecularColor"], m_specularColor);
			ConvertToFloatArray(mat["RimLightColor"], m_rimLightColor);
			ConvertToFloatArray(mat["InnerEdgeColor"], m_innerEdgeColor);
			ConvertToFloatArray(mat["OuterEdgeColor"], m_outerEdgeColor);
			ConvertToFloatArray(mat["GlowColor"], m_glowColor);

			m_glowStr = mat["GlowStr"];
			m_innerEdgeThickness = mat["InnerEdgeThickness"];
			m_outerEdgeThickness = mat["OuterEdgeThickness"];
			m_specularPower = mat["SpecularPower"];
			m_specularStrength = mat["SpecularStrength"];
			m_roughness = mat["Roughness"];
			m_shadowOffset = mat["ShadowOffset"];
			m_innerFalloff = mat["InnerFalloff"];
			m_outerFalloff = mat["OuterFalloff"];
			m_rimLightIntensity = mat["RimLightIntensity"];
			m_rimLightSize = mat["RimLightSize"];
		}

		MaterialInfo(string materialName, string materialTextureName, float baseColor[4], float shadowColor[4],
			float specularColor[4], float rimLightColor[4], float innerEdgeColor[4],
			float outerEdgeColor[4], float glowColor[4], float glowStr, float innerEdgeThickness,
			float outerEdgeThickness, float specularPower, float specularStrength, float roughness,
			float shadowOffset, float innerFalloff, float outerFalloff, float rimLightIntensity, float rimLightSize)
		{
			this->m_materialName = materialName;
			this->m_materialTextureName = materialTextureName;

			for (int i = 0; i < 4; i++)
			{
				this->m_baseColor[i] = baseColor[i];
				this->m_shadowColor[i] = shadowColor[i];
				this->m_specularColor[i] = specularColor[i];
				this->m_rimLightColor[i] = rimLightColor[i];
				this->m_innerEdgeColor[i] = innerEdgeColor[i];
				this->m_outerEdgeColor[i] = outerEdgeColor[i];
				this->m_glowColor[i] = glowColor[i];
			}

			this->m_glowStr = glowStr;
			this->m_innerEdgeThickness = innerEdgeThickness;
			this->m_outerEdgeThickness = outerEdgeThickness;
			this->m_specularPower = specularPower;
			this->m_specularStrength = specularStrength;
			this->m_roughness = roughness;
			this->m_shadowOffset = shadowOffset;
			this->m_innerFalloff = innerFalloff;
			this->m_outerFalloff = outerFalloff;
			this->m_rimLightIntensity = rimLightIntensity;
			this->m_rimLightSize = rimLightSize;
		}



		string m_materialTextureName;
		string m_materialName;

		float m_baseColor[4] = { 0,0,0,0 };
		float m_shadowColor[4] = { 0,0,0,0 };
		float m_specularColor[4] = { 0,0,0,0 };
		float m_rimLightColor[4] = { 0,0,0,0 };
		float m_innerEdgeColor[4] = { 0,0,0,0 };
		float m_outerEdgeColor[4] = { 0,0,0,0 };

		float m_glowColor[4] = { 0,0,0,0 };
		float m_glowStr = 0;
		float m_innerEdgeThickness = 0;
		float m_outerEdgeThickness = 0;
		float m_specularPower = 0;
		float m_specularStrength = 0;
		float m_roughness = 0;

		float m_shadowOffset = 0;
		float m_innerFalloff = 0;
		float m_outerFalloff = 0;
		float m_rimLightIntensity = 0;
		float m_rimLightSize = 0;



		void set(float baseColor[4], float shadowColor[4], float specularColor[4], float rimLightColor[4], 
			float innerEdgeColor[4], float outerEdgeColor[4], float glowColor[4], float glowStr,
			float innerEdgeThickness, float outerEdgeThickness, float specularPower, float specularStrength,float roughness, float shadowOffset, float innerFalloff, float outerFalloff,float rimLightIntensity, float rimLightSize)
		{
			for (int i = 0; i < 4; i++)
			{
				m_baseColor[i] = baseColor[i];
				m_baseColor[i] = shadowColor[i];
				m_baseColor[i] = specularColor[i];
				m_baseColor[i] = rimLightColor[i];
				m_baseColor[i] = innerEdgeColor[i];
				m_baseColor[i] = outerEdgeColor[i];
				m_baseColor[i] = glowColor[i];
			}
			m_glowStr = glowStr;
			m_innerEdgeThickness = innerEdgeThickness;
			m_outerEdgeThickness = outerEdgeThickness;
			m_specularPower = specularPower;
			m_specularStrength = specularStrength;
			m_roughness = roughness;
			m_shadowOffset = shadowOffset;
			m_innerFalloff = innerFalloff;
			m_outerFalloff = outerFalloff;
			m_rimLightIntensity = rimLightIntensity;
			m_rimLightSize = rimLightSize;
		}

		void set(Aen::Material& material)
		{
			ConvertToFloatArray(material["BaseColor"], m_baseColor);
			ConvertToFloatArray(material["ShadowColor"], m_shadowColor);
			ConvertToFloatArray(material["SpecularColor"], m_specularColor);
			ConvertToFloatArray(material["RimLightColor"], m_rimLightColor);
			ConvertToFloatArray(material["InnerEdgeColor"], m_innerEdgeColor);
			ConvertToFloatArray(material["OuterEdgeColor"], m_outerEdgeColor);
			ConvertToFloatArray(material["GlowColor"], m_glowColor);

			m_glowStr = material["GlowStr"];
			m_innerEdgeThickness = material["InnerEdgeThickness"];
			m_outerEdgeThickness = material["OuterEdgeThickness"];
			m_specularPower = material["SpecularPower"];
			m_specularStrength = material["SpecularStrength"];
			m_roughness = material["Roughness"];
			m_shadowOffset = material["ShadowOffset"];
			m_innerFalloff = material["InnerFalloff"];
			m_outerFalloff = material["OuterFalloff"];
			m_rimLightIntensity = material["RimLightIntensity"];
			m_rimLightSize = material["RimLightSize"];
		}

		void set(AenIF::Material& material)
		{
			m_materialName = material.materialName;
			m_materialTextureName = material.materialTextureName;

			for (int i = 0; i < 4; i++)
			{
				m_baseColor[i] = material.baseColor[i];
				m_shadowColor[i] = material.shadowColor[i];
				m_specularColor[i] = material.specularColor[i];
				m_rimLightColor[i] = material.rimLightColor[i];
				m_innerEdgeColor[i] = material.innerEdgeColor[i];
				m_outerEdgeColor[i] = material.outerEdgeColor[i];
				m_glowColor[i] = material.glowColor[i];
			}

			m_glowStr = material.glowStr;
			m_innerEdgeThickness = material.innerEdgeThickness;
			m_outerEdgeThickness = material.outerEdgeThickness;
			m_specularPower = material.specularPower;
			m_specularStrength = material.specularStrength;
			m_roughness = material.roughness;
			m_shadowOffset = material.shadowOffset;
			m_innerFalloff = material.innerFalloff;
			m_outerFalloff = material.outerFalloff;
			m_rimLightIntensity = material.rimLightIntensity;
			m_rimLightSize = material.rimLightSize;
		}

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

		ModelContainer(AenIF::Material &mat, string textureName, string modelName, string meshName, string type, bool rigidBody, string rigidBodyType) {
			this->m_material.set(mat);
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

