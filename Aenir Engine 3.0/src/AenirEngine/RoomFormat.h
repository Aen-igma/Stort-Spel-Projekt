#pragma once
#include <string>

using std::string;

namespace AenIF
{
	
	struct Room
	{
		string type;
		string special;
		string theme;
		unsigned int probability;

	};

	struct Model
	{
		string name; // name for editor
		string mesh; // what obj
		float translation[3];
		float rotation[3];
		float scale[3];

		string type; // enemy etc
		//Texture texture;
		string sound;
		//Material material;
	};

	struct Texture
	{
		string name;
		string textureType;
		//string blending[FILE_PATH_NAME_MAX_LENGTH];

	};
	struct Material
	{
		float diffuse[3];
		float specular[3];
		float emissive[3];
		float opacity[3];
	};

	struct Light
	{
		string type;
		float translation[3];
		float rotation[3];
		float direction[3];
		float intensity;
		float color[3];
		float range;
		float attenuation[3];
		float angle;
	};

	struct Particle
	{
		float translation[3];
		std::string type;
	};

}
