#pragma once
#include <string>

using std::string;

namespace AenIF
{
	
	struct Room
	{
		int type;
		int special;
		int theme;
		unsigned int probability;
		int size;

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
		bool rigidBody;
		string rigidBodyType;
		bool castShadow;
	};

	struct Texture
	{
		string name;
		string textureType;
		//string blending[FILE_PATH_NAME_MAX_LENGTH];

	};
	struct Material
	{
		string materialName;
		string materialTextureName;
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

		Material()
		{
		
		}

		Material(string materialName, string materialTextureName, float baseColor[4], float shadowColor[4], 
			float specularColor[4], float rimLightColor[4], float innerEdgeColor[4], 
			float outerEdgeColor[4], float glowColor[4], float glowStr, float innerEdgeThickness, 
			float outerEdgeThickness, float specularPower, float specularStrength, float roughness, 
			float shadowOffset, float innerFalloff, float outerFalloff, float rimLightIntensity, float rimLightSize)
		{
			this->materialName = materialName;
			this->materialTextureName = materialTextureName;

			for (int i = 0; i < 4; i++)
			{
				this->baseColor[i] = baseColor[i];
				this->shadowColor[i] = shadowColor[i];
				this->specularColor[i] = specularColor[i];
				this->rimLightColor[i] = rimLightColor[i];
				this->innerEdgeColor[i] = innerEdgeColor[i];
				this->outerEdgeColor[i] = outerEdgeColor[i];
				this->glowColor[i] = glowColor[i];
			}
			
			this->glowStr = glowStr;
			this->innerEdgeThickness = innerEdgeThickness;
			this->outerEdgeThickness = outerEdgeThickness;
			this->specularPower = specularPower;
			this->specularStrength = specularStrength;
			this->roughness = roughness;
			this->shadowOffset = shadowOffset;
			this->innerFalloff = innerFalloff;
			this->outerFalloff = outerFalloff;
			this->rimLightIntensity = rimLightIntensity;
			this->rimLightSize = rimLightSize;
		}
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
