#pragma once
#include <string>

namespace IGH 
{
	using std::string;

	const string OPEN = "Open";
	const string CLOSE = "Close";
	const string SAVE = "Save";
	const string COPY = "Copy";


	const string FBX = "fbx";
	const string OBJ = "obj";
	const string PNG = "png";
	const string JPG = "jpg";

	const string RESOURCEPATH = "../Resource/";

	const string LIGHT = "Light";
	const string POINTLIGHT = "Point Light";
	const string SPOTLIGHT = "Spot Light";
	const string DIRECTIONALLIGHT = "Directional light";
	const string CAMERA = "Camera";
	const string MODEL = "Model";
	const string MATERIAL = "Material";
	const string TEXTURE = "Texture";

	static const char* ROOMTHEME[] = { "Normal Dungeon", "Skeleton", "Gothic", "Aztec" };
	static const char* ROOMTYPE[] = { "Straight", "Bend", "T Junction" ,"Four Way", "None" };
	static const char* SPECIALROOM[] = { "Normal", "Entrance", "Exit", "Boss", "Arena", "Item" };
	static const char* HITBOXTYPE[] = { "None","Static", "Dynamic" };

}

