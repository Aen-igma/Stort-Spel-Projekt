#pragma once

#define MESH_NAME_MAX_LENGTH 64
#define MATERIAL_NAME_MAX_LENGTH 32
#define SHADING_MODEL_NAME_MAX_LENGTH 32
#define TEXTURE_NAME_MAX_LENGTH 24
#define FILE_PATH_NAME_MAX_LENGTH 128
#define TYPE_NAME 32

namespace Aen 
{
	enum TYPE
	{
		UNKNOWN,
		ROOM,
		MODEL,
		TEXTURE,
		MATERIAL,
		LIGHT,
		PARTICLE,

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
		char type[TYPE_NAME];
		char special[TYPE_NAME];
		char theme[TYPE_NAME];
		unsigned int probability;
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
	};

	struct TextureStruct
	{
		// add id maybe
		char name[TEXTURE_NAME_MAX_LENGTH];
		char textureType[TYPE_NAME];
		//char blending[FILE_PATH_NAME_MAX_LENGTH];
	};
	struct MaterialStruct
	{
		float diffuse[3];
		float specular[3];
		float emissive[3];
		float opacity[3];
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
