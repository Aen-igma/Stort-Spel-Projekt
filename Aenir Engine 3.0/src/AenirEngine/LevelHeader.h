#pragma once

#define MESH_NAME_MAX_LENGTH 64
#define MATERIAL_NAME_MAX_LENGTH 64
#define SHADING_MODEL_NAME_MAX_LENGTH 64
#define TEXTURE_NAME_MAX_LENGTH 64
#define FILE_PATH_NAME_MAX_LENGTH 128
#define TYPE_NAME 32

namespace Aen 
{
	enum TYPE
	{
		UNKNOWN = 0,
		ROOM = 1,
		MODEL = 2,
		TEXTURE = 3,
		MATERIAL = 4,
		LIGHT = 5,
		PARTICLE = 6,

	};

	struct SectionHeader
	{
		TYPE type;

		SectionHeader(TYPE _type)
		{
			type = _type;
		}
		SectionHeader()
		{
			type = UNKNOWN;
		}
	};

	struct RoomStruct
	{
		int type;
		int special;
		int theme;
		unsigned int probability;
		int size;
	};

	struct ModelStruct
	{
		char name[MESH_NAME_MAX_LENGTH]; // name for editor
		char mesh[MESH_NAME_MAX_LENGTH]; // what obj
		float translation[3];
		float rotation[3];
		float scale[3];

		char type[TYPE_NAME]; // enemy etc
		//Texture texture;
		char sound[MESH_NAME_MAX_LENGTH];
		//Material material;
		bool rigidBody;
		char rigidBodyType[TYPE_NAME];
		bool castShadow;
	};

	struct TextureStruct
	{
		// add id maybe
		char texture[TEXTURE_NAME_MAX_LENGTH];
		char normalTexture[TYPE_NAME];

		//char blending[FILE_PATH_NAME_MAX_LENGTH];
	};
	struct MaterialStruct
	{
		char materialName[MATERIAL_NAME_MAX_LENGTH];
		char materialTextureName[MATERIAL_NAME_MAX_LENGTH];
		float baseColor[4];
		float shadowColor[4];
		float specularColor[4];
		float rimLightColor[4];
		float innerEdgeColor[4];
		float outerEdgeColor[4];
		float glowColor[4];

		float glowStr;
		float innerEdgeThickness;
		float outerEdgeThickness;
		float specularPower;
		float specularStrength;
		float roughness;
		float shadowOffset;
		float innerFalloff;
		float outerFalloff;
		float rimLightIntensity;
		float rimLightSize;
	};

	struct LightStruct
	{
		char type[TYPE_NAME];
		float translation[3];
		float rotation[3];
		float direction[3];
		float intensity;
		float color[3];
		float range;
		float attenuation[3];
		float angle;
	};

	struct ParticleStruct
	{
		float translation[3];
		char type[TYPE_NAME];
	};

	struct RoomHeader : SectionHeader
	{
		RoomHeader() : SectionHeader(ROOM) {}
	};

	struct ModelHeader : SectionHeader
	{
		ModelHeader() : SectionHeader(MODEL) {}
	};

	struct TextureHeader : SectionHeader
	{
		TextureHeader() : SectionHeader(TEXTURE) {}
	};

	struct MaterialHeader : SectionHeader
	{
		MaterialHeader() : SectionHeader(MATERIAL) {}
	};

	struct LightHeader : SectionHeader
	{
		LightHeader() : SectionHeader(LIGHT) {}
	};

	struct ParticleHeader : SectionHeader
	{
		ParticleHeader() : SectionHeader(PARTICLE) {}
	};

}
