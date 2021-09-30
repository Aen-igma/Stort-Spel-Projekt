#include "PCH.h"
#include "LevelExporter.h"

namespace Aen {

	LevelExporter::LevelExporter()
	{
	}

	void LevelExporter::openFile()
	{
		outfile.open(filePath + fileName, std::ofstream::out | std::ofstream::binary);

	}

	void LevelExporter::closeFile()
	{
		outfile.close();

	}

	void LevelExporter::writeInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, vector<string>& meshObjList, vector<string>& textureFileName, vector<string>& entityType)
	{
		cout << "writeInto" << endl;
		cout << "entityList " << entityList.size() << endl;
		cout << "itemList " << itemList.size() << endl;
		cout << "meshObjList " << meshObjList.size() << endl;
		cout << "textureFileName " << textureFileName.size() << endl;
		cout << "entityType " << entityType.size() << endl;


		RoomStruct roomStruct;
		ModelStruct modelStruct;
		TextureStruct textureStruct;
		LightStruct lightStruct;
		MaterialStruct materialStruct;
		ParticleStruct particleStruct;

		m_ModelVector.reserve(meshObjList.size());
		m_TextureVector.reserve(textureFileName.size());

		int meshIndex = 0;


		for (size_t i = 0; i < entityList.size(); i++)
		{
			if (entityType[i] == "Model")
			{
				strcpy(modelStruct.name, itemList[i].c_str());
				strcpy(modelStruct.mesh, meshObjList[meshIndex].c_str());
				m_ModelVector.push_back(modelStruct);
				cout << m_ModelVector[meshIndex].name << endl;
				cout << m_ModelVector[meshIndex].mesh << endl;
				meshIndex++;
			}
			else if(entityType[i] == "Directional light")
			{
				strcpy(lightStruct.type, entityType[i].c_str());
				cout << lightStruct.type << endl;
			}
			else if (entityType[i] == "Spot light")
			{
				strcpy(lightStruct.type, entityType[i].c_str());
				cout << lightStruct.type << endl;
			}
			else if (entityType[i] == "Point light")
			{
				strcpy(lightStruct.type, entityType[i].c_str());
				cout << lightStruct.type << endl;
			}

			cout << endl;
		}
		m_ModelVector.clear();
	}
}
